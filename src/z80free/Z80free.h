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


#ifndef Z80FREE_H
#define Z80FREE_H

#include <endian.h>
#include <stdbool.h>
#ifndef Z80_H
typedef unsigned short int word;
typedef unsigned char byte;
#endif

extern int Z80free_parityBit[256];

#define F_S      0x80
#define F_Z      0x40
#define F_5      0x20
#define F_H      0x10
#define F_3      0x08
#define F_PV     0x04
#define F_N      0x02
#define F_C      0x01

//enum Z80free_prefix {Z80XX, Z80CB, Z80DD, Z80ED, Z80FD};

typedef union {
	/** Word registers. */
	struct
	{
		word AF, BC, DE, HL, IX, IY, SP;
	} wr;

	/** Byte registers. SP can be accessed partially to simplify the load/save code. */

#if __BYTE_ORDER == __LITTLE_ENDIAN
	struct
	{
		byte F,A, C,B, E,D, L,H, IXl,IXh, IYl,IYh, P,S;
	} br;
#else
	struct
	{
		byte A,F, B,C, D,E, H,L, IXh,IXl, IYh,IYl, S,P;
	} br;
#endif

} Z80FRegs;

enum Z80Status {Z80XX, Z80CB, Z80DD, Z80ED, Z80FD, Z80INT};

/** A Z80 execution context. */
typedef struct
{
	Z80FRegs Ra;			/* Alternate register set (R) */
	Z80FRegs Rm;			/* Main register set (R) */
	word	PC;				/* Program counter */
	byte	R;				/* Refresh */
	byte	R2;				/* Upper bit for Refresh */
	byte	I;
	byte	IFF1;			/* Interrupt Flipflop 1. If it's 2, decrement it and don't allow INT */
	byte	IFF2;			/* Interrupt Flipflop 2 */
	byte	IM;				/* Interrupt mode */
	byte	HALT;			/* HALT status */
	byte	INT_P;			/* INT pending */
	byte	NMI_P;			/* NMI pending */
	byte	empty_bus;		/* value for empty bus when procesing a maskable int */
	word	IAddr;			/* address with offset for IX+d and IY+d */
	byte	IAddr_done;		/* if 1, IAddr contains a valid data */
	enum    Z80Status Status;
	bool    M1;             /* true if the processor is in M1 state */
	word    subtstates;     /* counts the number of tstates since the starting of the instruction */
} Z80FREE;

/* internal Z80 methods */

void Z80free_setFlag(Z80FREE *processor, byte flag);
void Z80free_resFlag(Z80FREE *processor, byte flag);
void Z80free_valFlag(Z80FREE *processor, byte flag, int val);
int Z80free_getFlag(Z80FREE *processor, byte flag);
void Z80free_adjustFlags (Z80FREE *processor, byte val);
void Z80free_adjustFlagSZP (Z80FREE *processor, byte val);
void Z80free_adjustLogicFlag (Z80FREE *processor, int flagH);
byte Z80free_doArithmetic (Z80FREE *processor, byte value1,byte value2, int withCarry, int isSub);
word Z80free_doArithmetic16 (Z80FREE *processor, word value1,word value2, int withCarry, int isSub);
void Z80free_doAND (Z80FREE *processor, byte value);
void Z80free_doOR (Z80FREE *processor, byte value);
void Z80free_doXOR (Z80FREE *processor, byte value);
void Z80free_doBIT (Z80FREE *processor, int b, byte val);
byte Z80free_doSetRes (Z80FREE *processor, int bit, int pos, byte val);
byte Z80free_doIncDec (Z80FREE *processor, byte val, int isDec);
word Z80free_doIncDec16 (Z80FREE *processor, word val, int isDec);
byte Z80free_doRLC (Z80FREE *processor, int adjFlags, byte val);
byte Z80free_doRL (Z80FREE *processor, int adjFlags, byte val);
byte Z80free_doRRC (Z80FREE *processor, int adjFlags, byte val);
byte Z80free_doRR (Z80FREE *processor, int adjFlags, byte val);
byte Z80free_doSL (Z80FREE *processor, int isArith, byte val);
byte Z80free_doSR (Z80FREE *processor, int isArith, byte val);
void Z80free_doPush (Z80FREE *processor, word val);
word Z80free_doPop (Z80FREE *processor);
void Z80free_doDAA (Z80FREE *processor);
byte Z80free_readR(Z80FREE *processor);
void Z80free_setR(Z80FREE *processor,byte value);
byte Z80free_readI(Z80FREE *processor);
void Z80free_doRRD(Z80FREE *processor);
void Z80free_doRLD(Z80FREE *processor);

void Z80free_jump_relative(Z80FREE *processor,byte relvar);
word Z80free_addr_relative(Z80FREE *processor,word address);
word Z80free_addr_relativeXDCB(Z80FREE *processor,word address,byte d1);
byte Z80free_read_param_8(Z80FREE *z80);
word Z80free_read_param_16(Z80FREE *z80);
word Z80free_read16 (Z80FREE *processor, word addr);
void Z80free_write16 (Z80FREE *processor, word addr,word val);

/* external Z80 methods */

void Z80free_reset(Z80FREE *);
int Z80free_step(Z80FREE *);
int Z80free_ustep(Z80FREE *);
void Z80free_INT(Z80FREE *,byte);

byte Z80free_Rd_Internal (Z80FREE *processor,word Addr);
void Z80free_Wr_Internal (Z80FREE *processor,word Addr, byte Value);
byte Z80free_In_Internal (Z80FREE *processor,word Port);
void Z80free_Out_Internal (Z80FREE *processor,word Port, byte Value);

byte Z80free_Rd (word Addr);
void Z80free_Wr (word Addr, byte Value);
byte Z80free_In (word Port);
void Z80free_Out (word Port, byte Value);

/* Opcode functions */

int Z80free_codes (Z80FREE *processor,byte opcode);
int Z80free_codesCB (Z80FREE *processor,byte opcode);
int Z80free_codesDD (Z80FREE *processor,byte opcode);
int Z80free_codesED (Z80FREE *processor,byte opcode);
int Z80free_codesFD (Z80FREE *processor,byte opcode);
int Z80free_codesDDCB (Z80FREE *processor,byte d1);
int Z80free_codesFDCB (Z80FREE *processor,byte d1);

#endif
