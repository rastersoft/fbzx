/*
 * Copyright (C) 2008-2009 Sergio Costas (Raster Software Vigo)
 * This file is part of Z80Free
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

int Z80free_codesED (Z80FREE *processor,byte opcode) {
	static byte tmp1;
	static byte tmp2;

	switch(opcode) {
	case 0: // NOP
		return (4);
	break;
	case 1: // NOP
		return (4);
	break;
	case 2: // NOP
		return (4);
	break;
	case 3: // NOP
		return (4);
	break;
	case 4: // NOP
		return (4);
	break;
	case 5: // NOP
		return (4);
	break;
	case 6: // NOP
		return (4);
	break;
	case 7: // NOP
		return (4);
	break;
	case 8: // NOP
		return (4);
	break;
	case 9: // NOP
		return (4);
	break;
	case 10: // NOP
		return (4);
	break;
	case 11: // NOP
		return (4);
	break;
	case 12: // NOP
		return (4);
	break;
	case 13: // NOP
		return (4);
	break;
	case 14: // NOP
		return (4);
	break;
	case 15: // NOP
		return (4);
	break;
	case 16: // NOP
		return (4);
	break;
	case 17: // NOP
		return (4);
	break;
	case 18: // NOP
		return (4);
	break;
	case 19: // NOP
		return (4);
	break;
	case 20: // NOP
		return (4);
	break;
	case 21: // NOP
		return (4);
	break;
	case 22: // NOP
		return (4);
	break;
	case 23: // NOP
		return (4);
	break;
	case 24: // NOP
		return (4);
	break;
	case 25: // NOP
		return (4);
	break;
	case 26: // NOP
		return (4);
	break;
	case 27: // NOP
		return (4);
	break;
	case 28: // NOP
		return (4);
	break;
	case 29: // NOP
		return (4);
	break;
	case 30: // NOP
		return (4);
	break;
	case 31: // NOP
		return (4);
	break;
	case 32: // NOP
		return (4);
	break;
	case 33: // NOP
		return (4);
	break;
	case 34: // NOP
		return (4);
	break;
	case 35: // NOP
		return (4);
	break;
	case 36: // NOP
		return (4);
	break;
	case 37: // NOP
		return (4);
	break;
	case 38: // NOP
		return (4);
	break;
	case 39: // NOP
		return (4);
	break;
	case 40: // NOP
		return (4);
	break;
	case 41: // NOP
		return (4);
	break;
	case 42: // NOP
		return (4);
	break;
	case 43: // NOP
		return (4);
	break;
	case 44: // NOP
		return (4);
	break;
	case 45: // NOP
		return (4);
	break;
	case 46: // NOP
		return (4);
	break;
	case 47: // NOP
		return (4);
	break;
	case 48: // NOP
		return (4);
	break;
	case 49: // NOP
		return (4);
	break;
	case 50: // NOP
		return (4);
	break;
	case 51: // NOP
		return (4);
	break;
	case 52: // NOP
		return (4);
	break;
	case 53: // NOP
		return (4);
	break;
	case 54: // NOP
		return (4);
	break;
	case 55: // NOP
		return (4);
	break;
	case 56: // NOP
		return (4);
	break;
	case 57: // NOP
		return (4);
	break;
	case 58: // NOP
		return (4);
	break;
	case 59: // NOP
		return (4);
	break;
	case 60: // NOP
		return (4);
	break;
	case 61: // NOP
		return (4);
	break;
	case 62: // NOP
		return (4);
	break;
	case 63: // NOP
		return (4);
	break;
	case 64: // IN_BC B
		processor->Rm.br.B=Z80free_In(processor->Rm.wr.BC);
		Z80free_adjustFlagSZP (processor,processor->Rm.br.B);
		Z80free_resFlag(processor,F_H|F_N);
		return (8);
	break;
	case 65: // OUT_BC B
		Z80free_Out(processor->Rm.wr.BC,processor->Rm.br.B);
		return (8);
	break;
	case 66: // SBC HL,BC
		processor->Rm.wr.HL=Z80free_doArithmetic16(processor,processor->Rm.wr.HL,processor->Rm.wr.BC,1,1);
		return (11);
	break;
	case 67: // LD (nn),BC
		Z80free_write16(Z80free_read_param_16(processor),processor->Rm.wr.BC);
		return (16);
	break;
	case 68: // NEG
		processor->Rm.br.A=Z80free_doArithmetic(processor,0,processor->Rm.br.A,0,1);
		return (4);
	break;
	case 69: // RETN
		processor->IFF1=(processor->IFF2 ? 2 : 0);
		processor->PC=Z80free_doPop(processor);
		return (10);
	break;
	case 70: // IM 0
		processor->IM=0;
		return (4);
	break;
	case 71: // LD I,A
		processor->I=processor->Rm.br.A;
		return (5);
	break;
	case 72: // IN_BC C
		processor->Rm.br.C=Z80free_In(processor->Rm.wr.BC);
		Z80free_adjustFlagSZP (processor,processor->Rm.br.C);
		Z80free_resFlag(processor,F_H|F_N);
		return (8);
	break;
	case 73: // OUT_BC C
		Z80free_Out(processor->Rm.wr.BC,processor->Rm.br.C);
		return (8);
	break;
	case 74: // ADC HL,BC
		processor->Rm.wr.HL=Z80free_doArithmetic16(processor,processor->Rm.wr.HL,processor->Rm.wr.BC,1,0);
		return (11);
	break;
	case 75: // LD BC,(nn)
		processor->Rm.wr.BC=Z80free_read16(Z80free_read_param_16(processor));
		return (16);
	break;
	case 76: // NEG
		processor->Rm.br.A=Z80free_doArithmetic(processor,0,processor->Rm.br.A,0,1);
		return (4);
	break;
	case 77: // RETI
		processor->IFF1=2;
		processor->IFF2=1;
		processor->PC=Z80free_doPop(processor);
		return (10);
	break;
	case 78: // IM 1
		processor->IM=1;
		return (4);
	break;
	case 79: // LD R,A
		Z80free_setR(processor,processor->Rm.br.A);
		return (5);
	break;
	case 80: // IN_BC D
		processor->Rm.br.D=Z80free_In(processor->Rm.wr.BC);
		Z80free_adjustFlagSZP (processor,processor->Rm.br.D);
		Z80free_resFlag(processor,F_H|F_N);
		return (8);
	break;
	case 81: // OUT_BC D
		Z80free_Out(processor->Rm.wr.BC,processor->Rm.br.D);
		return (8);
	break;
	case 82: // SBC HL,DE
		processor->Rm.wr.HL=Z80free_doArithmetic16(processor,processor->Rm.wr.HL,processor->Rm.wr.DE,1,1);
		return (11);
	break;
	case 83: // LD (nn),DE
		Z80free_write16(Z80free_read_param_16(processor),processor->Rm.wr.DE);
		return (16);
	break;
	case 84: // NEG
		processor->Rm.br.A=Z80free_doArithmetic(processor,0,processor->Rm.br.A,0,1);
		return (4);
	break;
	case 85: // RETN
		processor->IFF1=(processor->IFF2 ? 2 : 0);
		processor->PC=Z80free_doPop(processor);
		return (10);
	break;
	case 86: // IM 1
		processor->IM=1;
		return (4);
	break;
	case 87: // LD A,I
		processor->Rm.br.A=Z80free_readI(processor);
		return (5);
	break;
	case 88: // IN_BC E
		processor->Rm.br.E=Z80free_In(processor->Rm.wr.BC);
		Z80free_adjustFlagSZP (processor,processor->Rm.br.E);
		Z80free_resFlag(processor,F_H|F_N);
		return (8);
	break;
	case 89: // OUT_BC E
		Z80free_Out(processor->Rm.wr.BC,processor->Rm.br.E);
		return (8);
	break;
	case 90: // ADC HL,DE
		processor->Rm.wr.HL=Z80free_doArithmetic16(processor,processor->Rm.wr.HL,processor->Rm.wr.DE,1,0);
		return (11);
	break;
	case 91: // LD DE,(nn)
		processor->Rm.wr.DE=Z80free_read16(Z80free_read_param_16(processor));
		return (16);
	break;
	case 92: // NEG
		processor->Rm.br.A=Z80free_doArithmetic(processor,0,processor->Rm.br.A,0,1);
		return (4);
	break;
	case 93: // RETN
		processor->IFF1=(processor->IFF2 ? 2 : 0);
		processor->PC=Z80free_doPop(processor);
		return (10);
	break;
	case 94: // IM 2
		processor->IM=2;
		return (4);
	break;
	case 95: // LD A,R
		processor->Rm.br.A=Z80free_readR(processor);
		return (5);
	break;
	case 96: // IN_BC H
		processor->Rm.br.H=Z80free_In(processor->Rm.wr.BC);
		Z80free_adjustFlagSZP (processor,processor->Rm.br.H);
		Z80free_resFlag(processor,F_H|F_N);
		return (8);
	break;
	case 97: // OUT_BC H
		Z80free_Out(processor->Rm.wr.BC,processor->Rm.br.H);
		return (8);
	break;
	case 98: // SBC HL,HL
		processor->Rm.wr.HL=Z80free_doArithmetic16(processor,processor->Rm.wr.HL,processor->Rm.wr.HL,1,1);
		return (11);
	break;
	case 99: // LD (nn),HL
		Z80free_write16(Z80free_read_param_16(processor),processor->Rm.wr.HL);
		return (16);
	break;
	case 100: // NEG
		processor->Rm.br.A=Z80free_doArithmetic(processor,0,processor->Rm.br.A,0,1);
		return (4);
	break;
	case 101: // RETN
		processor->IFF1=(processor->IFF2 ? 2 : 0);
		processor->PC=Z80free_doPop(processor);
		return (10);
	break;
	case 102: // IM 0
		processor->IM=0;
		return (4);
	break;
	case 103: // RRD
		Z80free_doRRD(processor);
		return (14);
	break;
	case 104: // IN_BC L
		processor->Rm.br.L=Z80free_In(processor->Rm.wr.BC);
		Z80free_adjustFlagSZP (processor,processor->Rm.br.L);
		Z80free_resFlag(processor,F_H|F_N);
		return (8);
	break;
	case 105: // OUT_BC L
		Z80free_Out(processor->Rm.wr.BC,processor->Rm.br.L);
		return (8);
	break;
	case 106: // ADC HL,HL
		processor->Rm.wr.HL=Z80free_doArithmetic16(processor,processor->Rm.wr.HL,processor->Rm.wr.HL,1,0);
		return (11);
	break;
	case 107: // LD HL,(nn)
		processor->Rm.wr.HL=Z80free_read16(Z80free_read_param_16(processor));
		return (16);
	break;
	case 108: // NEG
		processor->Rm.br.A=Z80free_doArithmetic(processor,0,processor->Rm.br.A,0,1);
		return (4);
	break;
	case 109: // RETN
		processor->IFF1=(processor->IFF2 ? 2 : 0);
		processor->PC=Z80free_doPop(processor);
		return (10);
	break;
	case 110: // IM 0
		processor->IM=0;
		return (4);
	break;
	case 111: // RLD
		Z80free_doRLD(processor);
		return (14);
	break;
	case 112: // IN_BC
		tmp1=Z80free_In(processor->Rm.wr.BC);
		Z80free_adjustFlagSZP (processor,tmp1);
		Z80free_resFlag(processor,F_H|F_N);
		return (8);
	break;
	case 113: // OUT_BC 0
		Z80free_Out(processor->Rm.wr.BC,0);
		return (8);
	break;
	case 114: // SBC HL,SP
		processor->Rm.wr.HL=Z80free_doArithmetic16(processor,processor->Rm.wr.HL,processor->Rm.wr.SP,1,1);
		return (11);
	break;
	case 115: // LD (nn),SP
		Z80free_write16(Z80free_read_param_16(processor),processor->Rm.wr.SP);
		return (16);
	break;
	case 116: // NEG
		processor->Rm.br.A=Z80free_doArithmetic(processor,0,processor->Rm.br.A,0,1);
		return (4);
	break;
	case 117: // RETN
		processor->IFF1=(processor->IFF2 ? 2 : 0);
		processor->PC=Z80free_doPop(processor);
		return (10);
	break;
	case 118: // IM 1
		processor->IM=1;
		return (4);
	break;
	case 119: // NOP
		return (4);
	break;
	case 120: // IN_BC A
		processor->Rm.br.A=Z80free_In(processor->Rm.wr.BC);
		Z80free_adjustFlagSZP (processor,processor->Rm.br.A);
		Z80free_resFlag(processor,F_H|F_N);
		return (8);
	break;
	case 121: // OUT_BC A
		Z80free_Out(processor->Rm.wr.BC,processor->Rm.br.A);
		return (8);
	break;
	case 122: // ADC HL,SP
		processor->Rm.wr.HL=Z80free_doArithmetic16(processor,processor->Rm.wr.HL,processor->Rm.wr.SP,1,0);
		return (11);
	break;
	case 123: // LD SP,(nn)
		processor->Rm.wr.SP=Z80free_read16(Z80free_read_param_16(processor));
		return (16);
	break;
	case 124: // NEG
		processor->Rm.br.A=Z80free_doArithmetic(processor,0,processor->Rm.br.A,0,1);
		return (4);
	break;
	case 125: // RETN
		processor->IFF1=(processor->IFF2 ? 2 : 0);
		processor->PC=Z80free_doPop(processor);
		return (10);
	break;
	case 126: // IM 2
		processor->IM=2;
		return (4);
	break;
	case 127: // NOP
		return (4);
	break;
	case 128: // NOP
		return (4);
	break;
	case 129: // NOP
		return (4);
	break;
	case 130: // NOP
		return (4);
	break;
	case 131: // NOP
		return (4);
	break;
	case 132: // NOP
		return (4);
	break;
	case 133: // NOP
		return (4);
	break;
	case 134: // NOP
		return (4);
	break;
	case 135: // NOP
		return (4);
	break;
	case 136: // NOP
		return (4);
	break;
	case 137: // NOP
		return (4);
	break;
	case 138: // NOP
		return (4);
	break;
	case 139: // NOP
		return (4);
	break;
	case 140: // NOP
		return (4);
	break;
	case 141: // NOP
		return (4);
	break;
	case 142: // NOP
		return (4);
	break;
	case 143: // NOP
		return (4);
	break;
	case 144: // NOP
		return (4);
	break;
	case 145: // NOP
		return (4);
	break;
	case 146: // NOP
		return (4);
	break;
	case 147: // NOP
		return (4);
	break;
	case 148: // NOP
		return (4);
	break;
	case 149: // NOP
		return (4);
	break;
	case 150: // NOP
		return (4);
	break;
	case 151: // NOP
		return (4);
	break;
	case 152: // NOP
		return (4);
	break;
	case 153: // NOP
		return (4);
	break;
	case 154: // NOP
		return (4);
	break;
	case 155: // NOP
		return (4);
	break;
	case 156: // NOP
		return (4);
	break;
	case 157: // NOP
		return (4);
	break;
	case 158: // NOP
		return (4);
	break;
	case 159: // NOP
		return (4);
	break;
	case 160: // LDI
		tmp1=Z80free_Rd(processor->Rm.wr.HL++);
		Z80free_Wr(processor->Rm.wr.DE++,tmp1);
		tmp1+=processor->Rm.br.A;
		Z80free_valFlag(processor,F_3,tmp1&0x08);
		Z80free_valFlag(processor,F_5,tmp1&0x02);
		processor->Rm.wr.BC--;
		Z80free_resFlag(processor,F_H|F_N);
		if (processor->Rm.wr.BC)
			Z80free_setFlag(processor,F_PV);
		else
			Z80free_resFlag(processor,F_PV);
		return (12);
	break;
	case 161: // CPI
		tmp2=F_C&processor->Rm.br.F;
		tmp1=Z80free_doArithmetic(processor,processor->Rm.br.A,Z80free_Rd(processor->Rm.wr.HL++),0,1);
		if (processor->Rm.br.F&F_H)
			tmp1--;
		Z80free_valFlag(processor,F_3,tmp1&0x08);
		Z80free_valFlag(processor,F_5,tmp1&0x02);
		Z80free_setFlag(processor,F_N);
		Z80free_valFlag(processor,F_C,tmp2);
		processor->Rm.wr.BC--;
		if (processor->Rm.wr.BC)
			Z80free_setFlag(processor,F_PV);
		else
			Z80free_resFlag(processor,F_PV);
		return (12);
	break;
	case 162: // INI
		/*INI, IND, INIR and INDR first decrement B and then uses it*/
		Z80free_Wr(processor->Rm.wr.HL,Z80free_In(processor->Rm.wr.BC));
		processor->Rm.wr.HL++;
		processor->Rm.br.B=Z80free_doIncDec(processor,processor->Rm.br.B,1);
		Z80free_valFlag(processor,F_N, 1);
		Z80free_valFlag(processor,F_Z, (processor->Rm.br.B == 0));
		return (12);
	break;
	case 163: // OUTI
		/*OUTI, OUTD, OTIR and OTDR first uses B and then decrement it*/
		Z80free_Out(processor->Rm.wr.BC,Z80free_Rd(processor->Rm.wr.HL));
		processor->Rm.wr.HL++;
		processor->Rm.br.B=Z80free_doIncDec(processor,processor->Rm.br.B,1);
		return (12);
	break;
	case 164: // NOP
		return (4);
	break;
	case 165: // NOP
		return (4);
	break;
	case 166: // NOP
		return (4);
	break;
	case 167: // NOP
		return (4);
	break;
	case 168: // LDD
		tmp1=Z80free_Rd(processor->Rm.wr.HL--);
		Z80free_Wr(processor->Rm.wr.DE--,tmp1);
		tmp1+=processor->Rm.br.A;
		Z80free_valFlag(processor,F_3,tmp1&0x08);
		Z80free_valFlag(processor,F_5,tmp1&0x02);
		processor->Rm.wr.BC--;
		Z80free_resFlag(processor,F_H|F_N);
		if (processor->Rm.wr.BC)
			Z80free_setFlag(processor,F_PV);
		else
			Z80free_resFlag(processor,F_PV);
		return (12);
	break;
	case 169: // CPD
		tmp2=F_C&processor->Rm.br.F;
		tmp1=Z80free_doArithmetic(processor,processor->Rm.br.A,Z80free_Rd(processor->Rm.wr.HL--),0,1);
		if (processor->Rm.br.F&F_H)
			tmp1--;
		Z80free_valFlag(processor,F_3,tmp1&0x08);
		Z80free_valFlag(processor,F_5,tmp1&0x02);
		Z80free_setFlag(processor,F_N);
		Z80free_valFlag(processor,F_C,tmp2);
		processor->Rm.wr.BC--;
		if (processor->Rm.wr.BC)
			Z80free_setFlag(processor,F_PV);
		else
			Z80free_resFlag(processor,F_PV);
		return (12);
	break;
	case 170: // IND
		Z80free_Wr(processor->Rm.wr.HL,Z80free_In(processor->Rm.wr.BC));
		processor->Rm.br.B=Z80free_doIncDec(processor,processor->Rm.br.B,1);
		processor->Rm.wr.HL--;
		return (12);
	break;
	case 171: // OUTD
		Z80free_Out(processor->Rm.wr.BC,Z80free_Rd(processor->Rm.wr.HL));
		processor->Rm.wr.HL--;
		processor->Rm.br.B=Z80free_doIncDec(processor,processor->Rm.br.B,1);
		return (12);
	break;
	case 172: // NOP
		return (4);
	break;
	case 173: // NOP
		return (4);
	break;
	case 174: // NOP
		return (4);
	break;
	case 175: // NOP
		return (4);
	break;
	case 176: // LDIR
		tmp1=Z80free_Rd(processor->Rm.wr.HL++);
		Z80free_Wr(processor->Rm.wr.DE++,tmp1);
		processor->Rm.wr.BC--;
		if (processor->Rm.wr.BC) {
			processor->PC-=2;
			return (17);
		} else {
			Z80free_resFlag(processor,F_H|F_N|F_PV);
		tmp1+=processor->Rm.br.A;
		Z80free_valFlag(processor,F_3,tmp1&0x08);
		Z80free_valFlag(processor,F_5,tmp1&0x02);
			return (12);
		}
	break;
	case 177: // CPIR
		tmp2=F_C&processor->Rm.br.F;
		Z80free_doArithmetic(processor,processor->Rm.br.A,Z80free_Rd(processor->Rm.wr.HL++),0,1);
		processor->Rm.wr.BC--;
		if ((processor->Rm.wr.BC)&&(!(processor->Rm.br.F&F_Z))) {
			processor->PC-=2;
			return (17);
		} else {
			Z80free_resFlag(processor,F_H|F_N|F_PV|F_3|F_5);
		Z80free_valFlag(processor,F_C,tmp2);
			return (12);
		}
	break;
	case 178: // INIR
		Z80free_Wr(processor->Rm.wr.HL,Z80free_In(processor->Rm.wr.BC));
		processor->Rm.br.B=Z80free_doIncDec(processor,processor->Rm.br.B,1);
		processor->Rm.wr.HL++;
		if (processor->Rm.br.B) {
			processor->PC-=2;
			return (17);
		} else {
			return (12);
		}
	break;
	case 179: // OTIR
		Z80free_Out(processor->Rm.wr.BC,Z80free_Rd(processor->Rm.wr.HL));
		processor->Rm.wr.HL++;
		processor->Rm.br.B=Z80free_doIncDec(processor,processor->Rm.br.B,1);
		if (processor->Rm.br.B) {
			processor->PC-=2;
			return (17);
		} else {
			return (12);
		}
	break;
	case 180: // NOP
		return (4);
	break;
	case 181: // NOP
		return (4);
	break;
	case 182: // NOP
		return (4);
	break;
	case 183: // NOP
		return (4);
	break;
	case 184: // LDDR
		tmp1=Z80free_Rd(processor->Rm.wr.HL--);
		Z80free_Wr(processor->Rm.wr.DE--,tmp1);
		processor->Rm.wr.BC--;
		if (processor->Rm.wr.BC) {
			processor->PC-=2;
			return (17);
		} else {
			Z80free_resFlag(processor,F_H|F_N|F_PV);
		tmp1+=processor->Rm.br.A;
		Z80free_valFlag(processor,F_3,tmp1&0x08);
		Z80free_valFlag(processor,F_5,tmp1&0x02);
			return (12);
		}
	break;
	case 185: // CPDR
		tmp2=F_C&processor->Rm.br.F;
		Z80free_doArithmetic(processor,processor->Rm.br.A,Z80free_Rd(processor->Rm.wr.HL--),0,1);
		processor->Rm.wr.BC--;
		if ((processor->Rm.wr.BC)&&(!(processor->Rm.br.F&F_Z))) {
			processor->PC-=2;
			return (17);
		} else {
			Z80free_resFlag(processor,F_H|F_N|F_PV|F_3|F_5);
		Z80free_valFlag(processor,F_C,tmp2);
			return (12);
		}
	break;
	case 186: // INDR
		Z80free_Wr(processor->Rm.wr.HL,Z80free_In(processor->Rm.wr.BC));
		processor->Rm.br.B=Z80free_doIncDec(processor,processor->Rm.br.B,1);
		processor->Rm.wr.HL--;
		if (processor->Rm.br.B) {
			processor->PC-=2;
			return (17);
		} else {
			return (12);
		}
	break;
	case 187: // OTDR
		Z80free_Out(processor->Rm.wr.BC,Z80free_Rd(processor->Rm.wr.HL));
		processor->Rm.wr.HL--;
		processor->Rm.br.B=Z80free_doIncDec(processor,processor->Rm.br.B,1);
		if (processor->Rm.br.B) {
			processor->PC-=2;
			return (17);
		} else {
			return (12);
		}
	break;
	case 188: // NOP
		return (4);
	break;
	case 189: // NOP
		return (4);
	break;
	case 190: // NOP
		return (4);
	break;
	case 191: // NOP
		return (4);
	break;
	case 192: // NOP
		return (4);
	break;
	case 193: // NOP
		return (4);
	break;
	case 194: // NOP
		return (4);
	break;
	case 195: // NOP
		return (4);
	break;
	case 196: // NOP
		return (4);
	break;
	case 197: // NOP
		return (4);
	break;
	case 198: // NOP
		return (4);
	break;
	case 199: // NOP
		return (4);
	break;
	case 200: // NOP
		return (4);
	break;
	case 201: // NOP
		return (4);
	break;
	case 202: // NOP
		return (4);
	break;
	case 203: // NOP
		return (4);
	break;
	case 204: // NOP
		return (4);
	break;
	case 205: // NOP
		return (4);
	break;
	case 206: // NOP
		return (4);
	break;
	case 207: // NOP
		return (4);
	break;
	case 208: // NOP
		return (4);
	break;
	case 209: // NOP
		return (4);
	break;
	case 210: // NOP
		return (4);
	break;
	case 211: // NOP
		return (4);
	break;
	case 212: // NOP
		return (4);
	break;
	case 213: // NOP
		return (4);
	break;
	case 214: // NOP
		return (4);
	break;
	case 215: // NOP
		return (4);
	break;
	case 216: // NOP
		return (4);
	break;
	case 217: // NOP
		return (4);
	break;
	case 218: // NOP
		return (4);
	break;
	case 219: // NOP
		return (4);
	break;
	case 220: // NOP
		return (4);
	break;
	case 221: // NOP
		return (4);
	break;
	case 222: // NOP
		return (4);
	break;
	case 223: // NOP
		return (4);
	break;
	case 224: // NOP
		return (4);
	break;
	case 225: // NOP
		return (4);
	break;
	case 226: // NOP
		return (4);
	break;
	case 227: // NOP
		return (4);
	break;
	case 228: // NOP
		return (4);
	break;
	case 229: // NOP
		return (4);
	break;
	case 230: // NOP
		return (4);
	break;
	case 231: // NOP
		return (4);
	break;
	case 232: // NOP
		return (4);
	break;
	case 233: // NOP
		return (4);
	break;
	case 234: // NOP
		return (4);
	break;
	case 235: // NOP
		return (4);
	break;
	case 236: // NOP
		return (4);
	break;
	case 237: // NOP
		return (4);
	break;
	case 238: // NOP
		return (4);
	break;
	case 239: // NOP
		return (4);
	break;
	case 240: // NOP
		return (4);
	break;
	case 241: // NOP
		return (4);
	break;
	case 242: // NOP
		return (4);
	break;
	case 243: // NOP
		return (4);
	break;
	case 244: // NOP
		return (4);
	break;
	case 245: // NOP
		return (4);
	break;
	case 246: // NOP
		return (4);
	break;
	case 247: // NOP
		return (4);
	break;
	case 248: // NOP
		return (4);
	break;
	case 249: // NOP
		return (4);
	break;
	case 250: // NOP
		return (4);
	break;
	case 251: // NOP
		return (4);
	break;
	case 252: // NOP
		return (4);
	break;
	case 253: // NOP
		return (4);
	break;
	case 254: // NOP
		return (4);
	break;
	case 255: // NOP
		return (4);
	break;
	}

	return -1;
}
