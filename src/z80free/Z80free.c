/*
 * Copyright 2008-2009 (C) Raster Software Vigo (Sergio Costas)
 * This file is part of Z80Free, with some bits extracted
 * and fixed from libZ80 (from Gabriel Gambetta)
 *
 * Z80Free is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Z80Free is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "Z80free.h"

void Z80free_reset(Z80FREE *processor) {

	processor->PC=0;
	processor->IFF1=0;
	processor->IFF2=0;
	processor->Rm.wr.AF=0xFFFF;
	processor->Rm.wr.BC=0xFFFF;
	processor->Rm.wr.DE=0xFFFF;
	processor->Rm.wr.HL=0xFFFF;
	processor->Rm.wr.IX=0xFFFF;
	processor->Rm.wr.IY=0xFFFF;
	processor->Ra.wr.AF=0xFFFF;
	processor->Ra.wr.BC=0xFFFF;
	processor->Ra.wr.DE=0xFFFF;
	processor->Ra.wr.HL=0xFFFF;
	processor->HALT=0;
	processor->IM=0;
	processor->I=0;
	processor->Status=Z80XX;
	processor->IAddr_done=0;
	processor->INT_P=0;
	processor->NMI_P=0;

}

void Z80free_INT(Z80FREE *processor,byte value) {

	processor->INT_P=1;
	processor->empty_bus=value;

}

int Z80free_step(Z80FREE *processor) {

	int retval=0;
	do {
		retval+=Z80free_ustep(processor);
	} while(processor->Status!=Z80XX);
	return (retval);
}

int Z80free_ustep(Z80FREE *processor) {

	static byte opcode,d1;
	int retval=0;

	processor->M1 = true;
	processor->subtstates = 0;
	processor->R++;
	if (processor->Status==Z80XX) {
		if (processor->NMI_P) { // NMI triggered
			if (processor->HALT) {
				processor->HALT=0;
				processor->PC++;
			}
			processor->NMI_P=0;
			processor->subtstates += 5;
			processor->IFF1=0; // disable INTs
			Z80free_doPush(processor,processor->PC);
			processor->PC=0x0066;
			processor->Status=Z80INT;
			return(11); // we use 11 tstates for attending a NMI
		}
		if (processor->INT_P) {
			processor->INT_P=0;
			if (processor->IFF1==1) { // allow INTs only in this case
				if (processor->HALT) {
					processor->HALT=0;
					processor->PC++;
				}
				processor->Status=Z80INT;
				processor->IFF1=0;
				processor->IFF2=0;
				processor->subtstates += 7;
				Z80free_doPush(processor,processor->PC);
				if (processor->IM != 2) { // we will forget IM0 mode for now; maybe in the future...
					processor->PC = 0x0038;
					return (13);
				} else {
					processor->PC=Z80free_read16(processor, ((((word)processor->I)<<8)&0xFF00) | ((word)processor->empty_bus));
					return (19);
				}
			}
		}
	}

	if (processor->IFF1>1) // set the right status for interrupts
		processor->IFF1--;

	opcode=Z80free_Rd(processor->PC);
	processor->subtstates += 4; // 4 tstates are needed to read the opcode
	processor->PC++;
	switch(processor->Status) {
	case Z80INT:
		processor->Status=Z80XX;
	case Z80XX:
		if (opcode==0xCB) {
			processor->Status=Z80CB;
			return 4;
		} else if (opcode==0xED) {
			processor->Status=Z80ED;
			return 4;
		} else if (opcode==0xDD) {
			processor->Status=Z80DD;
			return 4;
		} else if (opcode==0xFD) {
			processor->Status=Z80FD;
			return 4;
		} else {
			processor->M1 = false;
			return(Z80free_codes(processor,opcode));
		}
	break;
	case Z80CB:
		processor->Status=Z80XX;
		processor->M1 = false;
		return(Z80free_codesCB(processor,opcode));
	break;
	case Z80ED:
		processor->Status=Z80XX;
		processor->M1 = false;
		return(Z80free_codesED(processor,opcode));
	break;
	case Z80DD:
		if (opcode==0xDD) {
			return 4;
		}
		if (opcode==0xFD) {
			processor->Status=Z80FD;
			return 4;
		}
		processor->Status=Z80XX;
		if (opcode==0xCB) {
			d1=Z80free_Rd(processor->PC++);
			processor->subtstates += 8; // 8 tstates are needed: 3 to read the d value and 5 internal
			processor->M1 = false;
			retval+=Z80free_codesDDCB(processor,d1);
		} else {
			processor->M1 = false;
			retval+=Z80free_codesDD(processor,opcode);
		}
		processor->IAddr_done=0;
		return (retval);
	break;
	case Z80FD:
		if (opcode==0xDD) {
			processor->Status=Z80DD;
			return 4;
		}
		if (opcode==0xFD) {
			return 4;
		}
		processor->Status=Z80XX;
		if (opcode==0xCB) {
			d1=Z80free_Rd(processor->PC++);
			processor->subtstates += 8; // 8 tstates are needed: 3 to read the d value and 5 internal
			processor->M1 = false;
			retval+=Z80free_codesFDCB(processor,d1);
		} else {
			processor->M1 = false;
			retval+=Z80free_codesFD(processor,opcode);
		}
		processor->IAddr_done=0;
		return (retval);
	break;
	}
	return 0;
}

byte Z80free_Rd_Internal (Z80FREE *processor,word Addr) {
	byte value = Z80free_Rd(Addr);
	processor->subtstates += 3;
	return value;
}

void Z80free_Wr_Internal (Z80FREE *processor,word Addr, byte Value) {
	Z80free_Wr(Addr,Value);
	processor->subtstates += 3;
}

byte Z80free_In_Internal (Z80FREE *processor,word Port) {
	byte value = Z80free_In(Port);
	processor->subtstates += 4;
	return value;
}

void Z80free_Out_Internal (Z80FREE *processor,word Port, byte Value) {
	Z80free_Out(Port,Value);
	processor->subtstates += 4;
}

/* ---------------------------------------------------------
 *  Flag operations
 * ---------------------------------------------------------
 */

/** Sets a flag */
void  Z80free_setFlag(Z80FREE *processor, byte flag) {

	processor->Rm.br.F |= flag;
}

/** Resets a flag */
void Z80free_resFlag(Z80FREE *processor, byte flag) {

	processor->Rm.br.F &= ~flag;
}

/** Puts a value in a flag */
void Z80free_valFlag(Z80FREE *processor, byte flag, int val) {

	if (val)
		processor->Rm.br.F |= flag;
	else
		processor->Rm.br.F &= ~flag;
}

/** Returns a flag */
int Z80free_getFlag(Z80FREE *processor, byte flag) {

	return (processor->Rm.br.F & flag) != 0;
}


/* ---------------------------------------------------------
 *  Flag adjustments
 * ---------------------------------------------------------
 */

int Z80free_parityBit[256] = {

	1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
	0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
	0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
	1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
	0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
	1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
	1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
	0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
	0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
	1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
	1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
	0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
	1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
	0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
	0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
	1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1 };


void Z80free_adjustFlags (Z80FREE *processor, byte val) {

	Z80free_valFlag(processor,F_5, (val & F_5) != 0);
	Z80free_valFlag(processor,F_3, (val & F_3) != 0);
}


void Z80free_adjustFlagSZP (Z80FREE *processor, byte val) {

	Z80free_valFlag(processor,F_S, (val & 0x80) != 0);
	Z80free_valFlag(processor,F_Z, (val == 0));
	Z80free_valFlag(processor,F_PV, Z80free_parityBit[val]);
}


// Adjust flags after AND, OR, XOR
void Z80free_adjustLogicFlag (Z80FREE *processor, int flagH) {

	Z80free_valFlag(processor,F_S, (processor->Rm.br.A & 0x80) != 0);
	Z80free_valFlag(processor,F_Z, (processor->Rm.br.A == 0));
	Z80free_valFlag(processor,F_H, flagH);
	Z80free_valFlag(processor,F_N, 0);
	Z80free_valFlag(processor,F_C, 0);
	Z80free_valFlag(processor,F_PV, Z80free_parityBit[processor->Rm.br.A]);

	Z80free_adjustFlags(processor, processor->Rm.br.A);
}


/* ---------------------------------------------------------
 *  Generic operations
 * ---------------------------------------------------------
 */

byte Z80free_readR(Z80FREE *processor) {
	Z80free_adjustFlagSZP(processor,(0x7F&processor->R)|(0x80&processor->R2));
	Z80free_valFlag(processor,F_PV,processor->IFF2);
	Z80free_valFlag(processor,F_N, 0);
	Z80free_valFlag(processor,F_H, 0);
	Z80free_adjustFlags(processor,(0x7F&processor->R)|(0x80&processor->R2));
	return ((0x7F&processor->R)|(0x80&processor->R2));
}

void Z80free_setR(Z80FREE *processor,byte value) {
	processor->R=value;
	processor->R2=value;
}

byte Z80free_readI(Z80FREE *processor) {

	Z80free_adjustFlagSZP(processor,processor->I);
	Z80free_valFlag(processor,F_PV,processor->IFF2);
	Z80free_valFlag(processor,F_N, 0);
	Z80free_valFlag(processor,F_H, 0);
	Z80free_adjustFlags(processor,processor->I);
	return (processor->I);
}


/** Do an arithmetic operation (ADD, SUB, ADC, SBC y CP) */
byte Z80free_doArithmetic (Z80FREE *processor, byte value1,byte value2, int withCarry, int isSub) {

	static word res; /* To detect carry */
	static byte carry;

	if (withCarry && Z80free_getFlag(processor,F_C))
		carry=1;
	else
		carry=0;

	if (isSub) {
		Z80free_setFlag(processor,F_N);
		res = ((word)value1) - ((word)value2) - ((word)carry);
		Z80free_valFlag(processor,F_H, ((value1 ^ value2 ^ res) & 0x10) != 0);
		Z80free_valFlag(processor,F_PV, (((value1 ^ value2)&0x080) && (((res^value1)&0x080))));
	} else {
		Z80free_resFlag(processor,F_N);
		res = ((word)value1) + ((word)value2) + ((word)carry);
		Z80free_valFlag(processor,F_H, (((value1 & 0x0F) + (value2 & 0x0F)+carry) & 0x10) != 0);
		Z80free_valFlag(processor,F_PV, ((((value1 ^ value2)&0x080)==0) && ((res^value1)&0x080)));
	}

	Z80free_valFlag(processor,F_S, ((res & 0x080) != 0));
	Z80free_valFlag(processor,F_C, ((res & 0x0100) != 0));
	Z80free_valFlag(processor,F_Z, ((res&0xFF) == 0));

	Z80free_adjustFlags(processor, res&0xFF);

	return (byte)(res & 0xFF);
}

word Z80free_doArithmetic16 (Z80FREE *processor, word value1,word value2, int withCarry, int isSub) {

	static word tmp;
	static byte Ftmp;

	Ftmp=processor->Rm.br.F; // store the F register to restore the unchanged bits when doing operations without carry

	tmp=(word)Z80free_doArithmetic(processor,(byte)(value1&0x00FF),(byte)(value2&0x00FF),withCarry,isSub);
	tmp|=((word)Z80free_doArithmetic(processor,(byte)((value1>>8)&0x00FF),(byte)((value2>>8)&0x00FF),1,isSub))<<8;
	if (tmp) {
		Z80free_resFlag(processor,F_Z);
	} else {
		Z80free_setFlag(processor,F_Z);
	}

	if (tmp&0x08000) {
		Z80free_setFlag(processor,F_S);
	} else {
		Z80free_resFlag(processor,F_S);
	}

	if (!(withCarry|isSub)) {
		processor->Rm.br.F &= 0x3B; // preserve the new values of C, N, 3, H and 5
		processor->Rm.br.F |= (Ftmp & 0xC4); // set the old values of P/V, Z and S
	}

	return (tmp);
}

void Z80free_doAND (Z80FREE *processor, byte value) {

	processor->Rm.br.A &= value;
	Z80free_adjustLogicFlag(processor, 1);
}


void Z80free_doOR (Z80FREE *processor, byte value) {

	processor->Rm.br.A |= value;
	Z80free_adjustLogicFlag(processor, 0);
}


void Z80free_doXOR (Z80FREE *processor, byte value) {

	processor->Rm.br.A ^= value;
	Z80free_adjustLogicFlag(processor, 0);
}


void Z80free_doBIT (Z80FREE *processor, int b, byte val) {

	if (val & (1 << b))
		Z80free_resFlag(processor,F_Z | F_PV);
	else
		Z80free_setFlag(processor,F_Z | F_PV);

	Z80free_setFlag(processor,F_H);
	Z80free_resFlag(processor,F_N);

	if ((b == 7) && !Z80free_getFlag(processor,F_Z))
		Z80free_setFlag(processor,F_S);
	else
		Z80free_resFlag(processor,F_S);

	/*Z80free_resFlag(processor,F_5);
	if ((b == 5) && !Z80free_getFlag(processor,F_Z))
		Z80free_setFlag(processor,F_5);

	Z80free_resFlag(processor,F_3);
	if ((b == 3) && !Z80free_getFlag(processor,F_Z))
		Z80free_setFlag(processor,F_3);*/
	Z80free_adjustFlags(processor,val);
}


byte Z80free_doSetRes (Z80FREE *processor, int bit, int pos, byte val) {

	if (bit)
		val |= (1 << pos);
	else
		val &= ~(1 << pos);
	return val;
}



byte Z80free_doIncDec (Z80FREE *processor, byte val, int isDec) {

	if (isDec) {
		Z80free_valFlag(processor,F_PV, (val & 0x80) && !((val - 1) & 0x80));
		val--;
		Z80free_valFlag(processor,F_H, (val & 0x0F)==0x0F);
	} else {
		Z80free_valFlag(processor,F_PV, !(val & 0x80) && ((val + 1) & 0x80));
		val++;
		Z80free_valFlag(processor,F_H, (val & 0x0F)==0);
	}

	Z80free_valFlag(processor,F_S, ((val & 0x80) != 0));
	Z80free_valFlag(processor,F_Z, (val == 0));
	Z80free_valFlag(processor,F_N, isDec);

	Z80free_adjustFlags(processor, val);

	return val;
}


word Z80free_doIncDec16 (Z80FREE *processor, word val, int isDec) {

	if (isDec) {
		val--;
	} else {
		val++;
	}

	return val;
}


byte Z80free_doRLC (Z80FREE *processor, int adjFlags, byte val) {

	Z80free_valFlag(processor,F_C, (val & 0x80) != 0);
	val <<= 1;
	val |= (byte)Z80free_getFlag(processor,F_C);

	Z80free_adjustFlags(processor, val);
	Z80free_resFlag(processor,F_H | F_N);

	if (adjFlags)
		Z80free_adjustFlagSZP(processor, val);

	return val;
}


byte Z80free_doRL (Z80FREE *processor, int adjFlags, byte val) {

	int CY = Z80free_getFlag(processor,F_C);
	Z80free_valFlag(processor,F_C, (val & 0x80) != 0);
	val <<= 1;
	val |= (byte)CY;

	Z80free_adjustFlags(processor, val);
	Z80free_resFlag(processor,F_H | F_N);

	if (adjFlags)
		Z80free_adjustFlagSZP(processor, val);

	return val;
}


byte Z80free_doRRC (Z80FREE *processor, int adjFlags, byte val) {

	Z80free_valFlag(processor,F_C, (val & 0x01) != 0);
	val >>= 1;
	val |= ((byte)Z80free_getFlag(processor,F_C) << 7);

	Z80free_adjustFlags(processor, val);
	Z80free_resFlag(processor,F_H | F_N);

	if (adjFlags)
		Z80free_adjustFlagSZP(processor, val);

	return val;
}


byte Z80free_doRR (Z80FREE *processor, int adjFlags, byte val) {

	int CY = Z80free_getFlag(processor,F_C);
	Z80free_valFlag(processor,F_C, (val & 0x01));
	val >>= 1;
	val |= (CY << 7);

	Z80free_adjustFlags(processor, val);
	Z80free_resFlag(processor,F_H | F_N);

	if (adjFlags)
		Z80free_adjustFlagSZP(processor, val);

	return val;
}


byte Z80free_doSL (Z80FREE *processor, int isArith, byte val) {

	Z80free_valFlag(processor,F_C, (val & 0x80) != 0);
	val <<= 1;

	if (isArith)
		val |= 1;

	Z80free_adjustFlags(processor, val);
	Z80free_resFlag(processor,F_H | F_N);
	Z80free_adjustFlagSZP(processor, val);

	return val;
}


byte Z80free_doSR (Z80FREE *processor, int isArith, byte val) {

	static int b;

	b = (val & 0x80);

	Z80free_valFlag(processor,F_C, (val & 0x01) != 0);
	val >>= 1;

	if (isArith)
		val |= (byte)b;

	Z80free_adjustFlags(processor, val);
	Z80free_resFlag(processor,F_H | F_N);
	Z80free_adjustFlagSZP(processor, val);

	return val;
}

void Z80free_doRLD(Z80FREE *processor) {

	static byte tmp,tmp2;

	tmp=processor->Rm.br.A;
	tmp2=Z80free_Rd_Internal(processor,processor->Rm.wr.HL);
	processor->Rm.br.A&=0xF0;
	processor->Rm.br.A|=(0x0F&(tmp2>>4));
	Z80free_Wr_Internal(processor,processor->Rm.wr.HL,((tmp2<<4)&0xF0)|(tmp&0x0F));

	Z80free_resFlag(processor,F_H | F_N);
	Z80free_adjustFlagSZP(processor, processor->Rm.br.A);

}

void Z80free_doRRD(Z80FREE *processor) {

	static byte tmp,tmp2;

	tmp=processor->Rm.br.A;
	tmp2=Z80free_Rd_Internal(processor,processor->Rm.wr.HL);
	processor->Rm.br.A&=0xF0;
	processor->Rm.br.A|=(0x0F&tmp2);
	Z80free_Wr_Internal(processor,processor->Rm.wr.HL,((tmp2>>4)&0x0F)|((tmp<<4)&0xF0));

	Z80free_resFlag(processor,F_H | F_N);
	Z80free_adjustFlagSZP(processor, processor->Rm.br.A);

}

void Z80free_doPush (Z80FREE *processor, word val) {

	processor->Rm.wr.SP--;
	Z80free_Wr_Internal(processor,processor->Rm.wr.SP, (byte)((val>>8) & 0xFF));
	processor->Rm.wr.SP--;
	Z80free_Wr_Internal(processor,processor->Rm.wr.SP, (byte)(val & 0xFF));
}


word Z80free_doPop (Z80FREE *processor) {

	static word val;

	val = Z80free_read16(processor, processor->Rm.wr.SP);
	processor->Rm.wr.SP+=2;

	return val;
}


/* The DAA opcode
 * According to the value in A and the flags set, add a value to A
 *
 * Flags set   Byte (0..9)(0..9)
 * --------------------------------------------
 * (None)   + &00
 * Carry:+ &60
 * Subtract:+ &00
 * Subtract+Carry:+ &A0
 * Half-carry:+ &06
 * Half-carry+Carry:+ &66
 * Half-carry+Subtract:+ &FA
 * Half-carry+Subtract+Carry:+ &9A
 *
 * Flags set   Byte (0..9)(A..F)
 * --------------------------------------------
 * (None)   + &06
 * Carry:+ &66
 * Subtract:+ &00
 * Subtract+Carry:+ &a0
 * Half-carry:+ &06
 * Half-carry+Carry:+ &66
 * Half-carry+Subtract:+ &fa
 * Half-carry+Subtract+Carry:+ &9A
 *
 * Flags set   Byte (A..F)(0..9)
 * --------------------------------------------
 * (None)   + &60
 * Carry:+ &60
 * Subtract:+ &00
 * Subtract+Carry:+ &A0
 * Half-carry:+ &66
 * Half-carry+Carry:+ &66
 * Half-carry+Subtract:+ &fa
 * Half-carry+Subtract+Carry:+ &9A
 *
 * Flags set   Byte (A..F)(A..F)
 * --------------------------------------------
 * (None)   + &66
 * Carry:+ &66
 * Subtract:+ &00
 * Subtract+Carry:+ &a0
 * Half-carry:+ &66
 * Half-carry+Carry:+ &66
 * Half-carry+Subtract:+ &fa
 * Half-carry+Subtract+Carry:+ &9A
 */

static int Z80free_DAA_BYTETYPE1[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 };
static int Z80free_DAA_BYTETYPE2[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2 };

static byte Z80free_DAA_ADJUSTMENT[4][6] = {
	{0x00,0x06,0x00,0x66,0x60,0x66},
	{0x06,0x06,0x06,0x66,0x66,0x66},
	{0x60,0x66,0x60,0x66,0x60,0x66},
	{0x66,0x66,0x66,0x66,0x66,0x66} };

void Z80free_doDAA (Z80FREE *processor) {

	byte oldval;
	int byteType;
	int flagMask = 0;

	/* (0..8)(0..9) = 0 */
	/* (0..8)(A..F) = 1 */
	/*  (9)  (0..9) = 2 */
	/*  (9)  (A..F) = 3 */
	/* (A..F)(0..9) = 4 */
	/* (A..F)(A..F) = 5 */
	byteType = Z80free_DAA_BYTETYPE1[(processor->Rm.br.A&0x0F)] | ((Z80free_DAA_BYTETYPE2[(processor->Rm.br.A >> 4)&0x0F]) << 1);
	oldval=(processor->Rm.br.A&0x0F);

	if (Z80free_getFlag(processor,F_C))
		flagMask |= 2;
	if (Z80free_getFlag(processor,F_H))
		flagMask |= 1;

	if (processor->Rm.br.F&F_N) {
		processor->Rm.br.A -= Z80free_DAA_ADJUSTMENT[flagMask][byteType];
	} else {
		processor->Rm.br.A += Z80free_DAA_ADJUSTMENT[flagMask][byteType];
	}
	if ((byteType<3)&&((processor->Rm.br.F&F_C)==0)) {
		Z80free_resFlag(processor,F_C);
	} else {
		Z80free_setFlag(processor,F_C);
	}

	if ((processor->Rm.br.F&F_N)==0) {
		if (oldval>9) {
			Z80free_setFlag(processor,F_H);
		} else {
			Z80free_resFlag(processor,F_H);
		}
	} else {
		if (processor->Rm.br.F&F_H) {
			if (oldval>5) {
				Z80free_resFlag(processor,F_H);
			} else {
				Z80free_setFlag(processor,F_H);
			}
		}
	}

	processor->Rm.br.F&=0x57;
	processor->Rm.br.F|=(processor->Rm.br.A&0xA4);
	if (processor->Rm.br.A) {
		Z80free_resFlag(processor,F_Z);
	} else {
		Z80free_setFlag(processor,F_Z);
	}
	Z80free_valFlag(processor,F_PV, Z80free_parityBit[processor->Rm.br.A]);
}

void Z80free_jump_relative(Z80FREE *processor,byte relvar) {

	static word rel2;

	rel2=(word)relvar;
	if (relvar&0x080) {
			rel2|=0xFF00;
	}
	processor->PC+=rel2;
}

word Z80free_addr_relative(Z80FREE *processor,word address) {

	static word rel2;

	if (processor->IAddr_done) {
		return (processor->IAddr);
	}
	processor->IAddr_done=1;
	rel2 = (word)Z80free_read_param_8(processor);
	processor->subtstates += 5; // 5 tstates are needed: 3 to read the d value and 5 internal
	if (rel2&0x080) {
			rel2|=0xFF00;
	}
	processor->IAddr=address+rel2;
	return (address+rel2);
}

word Z80free_addr_relativeXDCB(Z80FREE *processor,word address,byte d1) {

	static word rel2;

	rel2=(word)d1;
	if (rel2&0x080) {
			rel2|=0xFF00;
	}
	processor->IAddr=address+rel2;
	return (address+rel2);
}

void Z80free_write16 (Z80FREE *processor, word addr,word val) {

	Z80free_Wr_Internal(processor,addr, (byte)(val & 0xFF));
	val >>= 8;
	addr++;
	Z80free_Wr_Internal(processor,addr, (byte)(val & 0xFF));
}

word Z80free_read16 (Z80FREE *processor, word addr) {

	static word v1;
	v1=((word)Z80free_Rd_Internal(processor,addr))&0x00FF;
	addr++;
	return (v1 + ((((word)Z80free_Rd_Internal(processor,addr)) << 8)&0xFF00));
}

byte Z80free_read_param_8(Z80FREE *processor) {

	byte data = Z80free_Rd_Internal(processor,processor->PC++);
	return(data);

}

word Z80free_read_param_16(Z80FREE *processor) {

	static word value;
	value = Z80free_read16(processor, processor->PC);
	processor->PC+=2;
	return (value);
}
