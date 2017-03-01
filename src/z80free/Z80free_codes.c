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

int Z80free_codes (Z80FREE *processor,byte opcode) {
	static byte tmp1;
	static word tmp2;

	switch(opcode) {
	case 0: // NOP
		return (4);
	break;
	case 1: // LD BC,nn
		processor->Rm.wr.BC=Z80free_read_param_16(processor);
		return (10);
	break;
	case 2: // LD (BC),A
		Z80free_Wr_Internal(processor,processor->Rm.wr.BC,processor->Rm.br.A);
		return (7);
	break;
	case 3: // INC BC
		processor->Rm.wr.BC=Z80free_doIncDec16(processor,processor->Rm.wr.BC,0);
		return (6);
	break;
	case 4: // INC B
		processor->Rm.br.B=Z80free_doIncDec(processor,processor->Rm.br.B,0);
		return (4);
	break;
	case 5: // DEC B
		processor->Rm.br.B=Z80free_doIncDec(processor,processor->Rm.br.B,1);
		return (4);
	break;
	case 6: // LD B,n
		processor->Rm.br.B=Z80free_read_param_8(processor);
		return (7);
	break;
	case 7: // RLCA
		processor->Rm.br.A=Z80free_doRLC(processor,0,processor->Rm.br.A);
		return (4);
	break;
	case 8: // EX AF,AF'
		tmp2=processor->Rm.wr.AF;
		processor->Rm.wr.AF=processor->Ra.wr.AF;
		processor->Ra.wr.AF=tmp2;
		return (4);
	break;
	case 9: // ADD HL,BC
		processor->Rm.wr.HL=Z80free_doArithmetic16(processor,processor->Rm.wr.HL,processor->Rm.wr.BC,0,0);
		return (11);
	break;
	case 10: // LD A,(BC)
		processor->Rm.br.A=Z80free_Rd_Internal(processor,processor->Rm.wr.BC);
		return (7);
	break;
	case 11: // DEC BC
		processor->Rm.wr.BC=Z80free_doIncDec16(processor,processor->Rm.wr.BC,1);
		return (6);
	break;
	case 12: // INC C
		processor->Rm.br.C=Z80free_doIncDec(processor,processor->Rm.br.C,0);
		return (4);
	break;
	case 13: // DEC C
		processor->Rm.br.C=Z80free_doIncDec(processor,processor->Rm.br.C,1);
		return (4);
	break;
	case 14: // LD C,n
		processor->Rm.br.C=Z80free_read_param_8(processor);
		return (7);
	break;
	case 15: // RRCA
		processor->Rm.br.A=Z80free_doRRC(processor,0,processor->Rm.br.A);
		return (4);
	break;
	case 16: // DJNZ n
		tmp1=Z80free_read_param_8(processor);
		processor->Rm.br.B--;
		if (processor->Rm.br.B) {
			Z80free_jump_relative(processor,tmp1);
			return (13);
		} else {
			return (8);
		}
	break;
	case 17: // LD DE,nn
		processor->Rm.wr.DE=Z80free_read_param_16(processor);
		return (10);
	break;
	case 18: // LD (DE),A
		Z80free_Wr_Internal(processor,processor->Rm.wr.DE,processor->Rm.br.A);
		return (7);
	break;
	case 19: // INC DE
		processor->Rm.wr.DE=Z80free_doIncDec16(processor,processor->Rm.wr.DE,0);
		return (6);
	break;
	case 20: // INC D
		processor->Rm.br.D=Z80free_doIncDec(processor,processor->Rm.br.D,0);
		return (4);
	break;
	case 21: // DEC D
		processor->Rm.br.D=Z80free_doIncDec(processor,processor->Rm.br.D,1);
		return (4);
	break;
	case 22: // LD D,n
		processor->Rm.br.D=Z80free_read_param_8(processor);
		return (7);
	break;
	case 23: // RLA
		processor->Rm.br.A=Z80free_doRL(processor,0,processor->Rm.br.A);
		return (4);
	break;
	case 24: // JR n
		tmp1=Z80free_read_param_8(processor);
		Z80free_jump_relative(processor,tmp1);
		return (12);
	break;
	case 25: // ADD HL,DE
		processor->Rm.wr.HL=Z80free_doArithmetic16(processor,processor->Rm.wr.HL,processor->Rm.wr.DE,0,0);
		return (11);
	break;
	case 26: // LD A,(DE)
		processor->Rm.br.A=Z80free_Rd_Internal(processor,processor->Rm.wr.DE);
		return (7);
	break;
	case 27: // DEC DE
		processor->Rm.wr.DE=Z80free_doIncDec16(processor,processor->Rm.wr.DE,1);
		return (6);
	break;
	case 28: // INC E
		processor->Rm.br.E=Z80free_doIncDec(processor,processor->Rm.br.E,0);
		return (4);
	break;
	case 29: // DEC E
		processor->Rm.br.E=Z80free_doIncDec(processor,processor->Rm.br.E,1);
		return (4);
	break;
	case 30: // LD E,n
		processor->Rm.br.E=Z80free_read_param_8(processor);
		return (7);
	break;
	case 31: // RRA
		processor->Rm.br.A=Z80free_doRR(processor,0,processor->Rm.br.A);
		return (4);
	break;
	case 32: // JR NZ,n
		tmp1=Z80free_read_param_8(processor);
		if (0==(F_Z&processor->Rm.br.F)) {
			Z80free_jump_relative(processor,tmp1);
			return (12);
		} else {
			return (7);
		}
	break;
	case 33: // LD HL,nn
		processor->Rm.wr.HL=Z80free_read_param_16(processor);
		return (10);
	break;
	case 34: // LD (nn),HL
		Z80free_write16(processor, Z80free_read_param_16(processor),processor->Rm.wr.HL);
		return (16);
	break;
	case 35: // INC HL
		processor->Rm.wr.HL=Z80free_doIncDec16(processor,processor->Rm.wr.HL,0);
		return (6);
	break;
	case 36: // INC H
		processor->Rm.br.H=Z80free_doIncDec(processor,processor->Rm.br.H,0);
		return (4);
	break;
	case 37: // DEC H
		processor->Rm.br.H=Z80free_doIncDec(processor,processor->Rm.br.H,1);
		return (4);
	break;
	case 38: // LD H,n
		processor->Rm.br.H=Z80free_read_param_8(processor);
		return (7);
	break;
	case 39: // DAA
		Z80free_doDAA(processor);
		return (4);
	break;
	case 40: // JR Z,n
		tmp1=Z80free_read_param_8(processor);
		if (F_Z&processor->Rm.br.F) {
			Z80free_jump_relative(processor,tmp1);
			return (12);
		} else {
			return (7);
		}
	break;
	case 41: // ADD HL,HL
		processor->Rm.wr.HL=Z80free_doArithmetic16(processor,processor->Rm.wr.HL,processor->Rm.wr.HL,0,0);
		return (11);
	break;
	case 42: // LD HL,(nn)
		processor->Rm.wr.HL=Z80free_read16(processor, Z80free_read_param_16(processor));
		return (16);
	break;
	case 43: // DEC HL
		processor->Rm.wr.HL=Z80free_doIncDec16(processor,processor->Rm.wr.HL,1);
		return (6);
	break;
	case 44: // INC L
		processor->Rm.br.L=Z80free_doIncDec(processor,processor->Rm.br.L,0);
		return (4);
	break;
	case 45: // DEC L
		processor->Rm.br.L=Z80free_doIncDec(processor,processor->Rm.br.L,1);
		return (4);
	break;
	case 46: // LD L,n
		processor->Rm.br.L=Z80free_read_param_8(processor);
		return (7);
	break;
	case 47: // CPL
		processor->Rm.br.A=255-processor->Rm.br.A;
		Z80free_adjustFlags(processor,processor->Rm.br.A);
		Z80free_setFlag(processor,F_H);
		Z80free_setFlag(processor,F_N);
		return (4);
	break;
	case 48: // JR NC,n
		tmp1=Z80free_read_param_8(processor);
		if (0==(F_C&processor->Rm.br.F)) {
			Z80free_jump_relative(processor,tmp1);
			return (12);
		} else {
			return (7);
		}
	break;
	case 49: // LD SP,nn
		processor->Rm.wr.SP=Z80free_read_param_16(processor);
		return (10);
	break;
	case 50: // LD (nn),A
		Z80free_Wr_Internal(processor,Z80free_read_param_16(processor),processor->Rm.br.A);
		return (13);
	break;
	case 51: // INC SP
		processor->Rm.wr.SP=Z80free_doIncDec16(processor,processor->Rm.wr.SP,0);
		return (6);
	break;
	case 52: // INC b(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doIncDec(processor,Z80free_Rd_Internal(processor,processor->Rm.wr.HL),0));
		return (11);
	break;
	case 53: // DEC b(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doIncDec(processor,Z80free_Rd_Internal(processor,processor->Rm.wr.HL),1));
		return (11);
	break;
	case 54: // LD (HL),n
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_read_param_8(processor));
		return (10);
	break;
	case 55: // SCF
		Z80free_setFlag(processor,F_C);
		Z80free_resFlag(processor,F_H);
		Z80free_resFlag(processor,F_N);
		Z80free_adjustFlags(processor,processor->Rm.br.A);
		return (4);
	break;
	case 56: // JR CF,n
		tmp1=Z80free_read_param_8(processor);
		if (F_C&processor->Rm.br.F) {
			Z80free_jump_relative(processor,tmp1);
			return (12);
		} else {
			return (7);
		}
	break;
	case 57: // ADD HL,SP
		processor->Rm.wr.HL=Z80free_doArithmetic16(processor,processor->Rm.wr.HL,processor->Rm.wr.SP,0,0);
		return (11);
	break;
	case 58: // LD A,(nn)
		processor->Rm.br.A=Z80free_Rd_Internal(processor,Z80free_read_param_16(processor));
		return (13);
	break;
	case 59: // DEC SP
		processor->Rm.wr.SP=Z80free_doIncDec16(processor,processor->Rm.wr.SP,1);
		return (6);
	break;
	case 60: // INC A
		processor->Rm.br.A=Z80free_doIncDec(processor,processor->Rm.br.A,0);
		return (4);
	break;
	case 61: // DEC A
		processor->Rm.br.A=Z80free_doIncDec(processor,processor->Rm.br.A,1);
		return (4);
	break;
	case 62: // LD A,n
		processor->Rm.br.A=Z80free_read_param_8(processor);
		return (7);
	break;
	case 63: // CCF
		Z80free_valFlag(processor,F_C,(~processor->Rm.br.F)&F_C);
		Z80free_resFlag(processor,F_N);
		Z80free_adjustFlags(processor,processor->Rm.br.A);
		return (4);
	break;
	case 64: // LD B,B
		processor->Rm.br.B=processor->Rm.br.B;
		return (4);
	break;
	case 65: // LD B,C
		processor->Rm.br.B=processor->Rm.br.C;
		return (4);
	break;
	case 66: // LD B,D
		processor->Rm.br.B=processor->Rm.br.D;
		return (4);
	break;
	case 67: // LD B,E
		processor->Rm.br.B=processor->Rm.br.E;
		return (4);
	break;
	case 68: // LD B,H
		processor->Rm.br.B=processor->Rm.br.H;
		return (4);
	break;
	case 69: // LD B,L
		processor->Rm.br.B=processor->Rm.br.L;
		return (4);
	break;
	case 70: // LD B,(HL)
		processor->Rm.br.B=Z80free_Rd_Internal(processor,processor->Rm.wr.HL);
		return (7);
	break;
	case 71: // LD B,A
		processor->Rm.br.B=processor->Rm.br.A;
		return (4);
	break;
	case 72: // LD C,B
		processor->Rm.br.C=processor->Rm.br.B;
		return (4);
	break;
	case 73: // LD C,C
		processor->Rm.br.C=processor->Rm.br.C;
		return (4);
	break;
	case 74: // LD C,D
		processor->Rm.br.C=processor->Rm.br.D;
		return (4);
	break;
	case 75: // LD C,E
		processor->Rm.br.C=processor->Rm.br.E;
		return (4);
	break;
	case 76: // LD C,H
		processor->Rm.br.C=processor->Rm.br.H;
		return (4);
	break;
	case 77: // LD C,L
		processor->Rm.br.C=processor->Rm.br.L;
		return (4);
	break;
	case 78: // LD C,(HL)
		processor->Rm.br.C=Z80free_Rd_Internal(processor,processor->Rm.wr.HL);
		return (7);
	break;
	case 79: // LD C,A
		processor->Rm.br.C=processor->Rm.br.A;
		return (4);
	break;
	case 80: // LD D,B
		processor->Rm.br.D=processor->Rm.br.B;
		return (4);
	break;
	case 81: // LD D,C
		processor->Rm.br.D=processor->Rm.br.C;
		return (4);
	break;
	case 82: // LD D,D
		processor->Rm.br.D=processor->Rm.br.D;
		return (4);
	break;
	case 83: // LD D,E
		processor->Rm.br.D=processor->Rm.br.E;
		return (4);
	break;
	case 84: // LD D,H
		processor->Rm.br.D=processor->Rm.br.H;
		return (4);
	break;
	case 85: // LD D,L
		processor->Rm.br.D=processor->Rm.br.L;
		return (4);
	break;
	case 86: // LD D,(HL)
		processor->Rm.br.D=Z80free_Rd_Internal(processor,processor->Rm.wr.HL);
		return (7);
	break;
	case 87: // LD D,A
		processor->Rm.br.D=processor->Rm.br.A;
		return (4);
	break;
	case 88: // LD E,B
		processor->Rm.br.E=processor->Rm.br.B;
		return (4);
	break;
	case 89: // LD E,C
		processor->Rm.br.E=processor->Rm.br.C;
		return (4);
	break;
	case 90: // LD E,D
		processor->Rm.br.E=processor->Rm.br.D;
		return (4);
	break;
	case 91: // LD E,E
		processor->Rm.br.E=processor->Rm.br.E;
		return (4);
	break;
	case 92: // LD E,H
		processor->Rm.br.E=processor->Rm.br.H;
		return (4);
	break;
	case 93: // LD E,L
		processor->Rm.br.E=processor->Rm.br.L;
		return (4);
	break;
	case 94: // LD E,(HL)
		processor->Rm.br.E=Z80free_Rd_Internal(processor,processor->Rm.wr.HL);
		return (7);
	break;
	case 95: // LD E,A
		processor->Rm.br.E=processor->Rm.br.A;
		return (4);
	break;
	case 96: // LD H,B
		processor->Rm.br.H=processor->Rm.br.B;
		return (4);
	break;
	case 97: // LD H,C
		processor->Rm.br.H=processor->Rm.br.C;
		return (4);
	break;
	case 98: // LD H,D
		processor->Rm.br.H=processor->Rm.br.D;
		return (4);
	break;
	case 99: // LD H,E
		processor->Rm.br.H=processor->Rm.br.E;
		return (4);
	break;
	case 100: // LD H,H
		processor->Rm.br.H=processor->Rm.br.H;
		return (4);
	break;
	case 101: // LD H,L
		processor->Rm.br.H=processor->Rm.br.L;
		return (4);
	break;
	case 102: // LD H,(HL)
		processor->Rm.br.H=Z80free_Rd_Internal(processor,processor->Rm.wr.HL);
		return (7);
	break;
	case 103: // LD H,A
		processor->Rm.br.H=processor->Rm.br.A;
		return (4);
	break;
	case 104: // LD L,B
		processor->Rm.br.L=processor->Rm.br.B;
		return (4);
	break;
	case 105: // LD L,C
		processor->Rm.br.L=processor->Rm.br.C;
		return (4);
	break;
	case 106: // LD L,D
		processor->Rm.br.L=processor->Rm.br.D;
		return (4);
	break;
	case 107: // LD L,E
		processor->Rm.br.L=processor->Rm.br.E;
		return (4);
	break;
	case 108: // LD L,H
		processor->Rm.br.L=processor->Rm.br.H;
		return (4);
	break;
	case 109: // LD L,L
		processor->Rm.br.L=processor->Rm.br.L;
		return (4);
	break;
	case 110: // LD L,(HL)
		processor->Rm.br.L=Z80free_Rd_Internal(processor,processor->Rm.wr.HL);
		return (7);
	break;
	case 111: // LD L,A
		processor->Rm.br.L=processor->Rm.br.A;
		return (4);
	break;
	case 112: // LD (HL),B
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,processor->Rm.br.B);
		return (7);
	break;
	case 113: // LD (HL),C
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,processor->Rm.br.C);
		return (7);
	break;
	case 114: // LD (HL),D
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,processor->Rm.br.D);
		return (7);
	break;
	case 115: // LD (HL),E
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,processor->Rm.br.E);
		return (7);
	break;
	case 116: // LD (HL),H
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,processor->Rm.br.H);
		return (7);
	break;
	case 117: // LD (HL),L
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,processor->Rm.br.L);
		return (7);
	break;
	case 118: // HALT
		processor->PC--;
		processor->HALT=1;
		return (4);
	break;
	case 119: // LD (HL),A
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,processor->Rm.br.A);
		return (7);
	break;
	case 120: // LD A,B
		processor->Rm.br.A=processor->Rm.br.B;
		return (4);
	break;
	case 121: // LD A,C
		processor->Rm.br.A=processor->Rm.br.C;
		return (4);
	break;
	case 122: // LD A,D
		processor->Rm.br.A=processor->Rm.br.D;
		return (4);
	break;
	case 123: // LD A,E
		processor->Rm.br.A=processor->Rm.br.E;
		return (4);
	break;
	case 124: // LD A,H
		processor->Rm.br.A=processor->Rm.br.H;
		return (4);
	break;
	case 125: // LD A,L
		processor->Rm.br.A=processor->Rm.br.L;
		return (4);
	break;
	case 126: // LD A,(HL)
		processor->Rm.br.A=Z80free_Rd_Internal(processor,processor->Rm.wr.HL);
		return (7);
	break;
	case 127: // LD A,A
		processor->Rm.br.A=processor->Rm.br.A;
		return (4);
	break;
	case 128: // ADD A,B
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.B,0,0);
		return (4);
	break;
	case 129: // ADD A,C
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.C,0,0);
		return (4);
	break;
	case 130: // ADD A,D
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.D,0,0);
		return (4);
	break;
	case 131: // ADD A,E
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.E,0,0);
		return (4);
	break;
	case 132: // ADD A,H
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.H,0,0);
		return (4);
	break;
	case 133: // ADD A,L
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.L,0,0);
		return (4);
	break;
	case 134: // ADD A,(HL)
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,Z80free_Rd_Internal(processor,processor->Rm.wr.HL),0,0);
		return (7);
	break;
	case 135: // ADD A,A
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.A,0,0);
		return (4);
	break;
	case 136: // ADC A,B
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.B,1,0);
		return (4);
	break;
	case 137: // ADC A,C
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.C,1,0);
		return (4);
	break;
	case 138: // ADC A,D
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.D,1,0);
		return (4);
	break;
	case 139: // ADC A,E
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.E,1,0);
		return (4);
	break;
	case 140: // ADC A,H
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.H,1,0);
		return (4);
	break;
	case 141: // ADC A,L
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.L,1,0);
		return (4);
	break;
	case 142: // ADC A,(HL)
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,Z80free_Rd_Internal(processor,processor->Rm.wr.HL),1,0);
		return (7);
	break;
	case 143: // ADC A,A
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.A,1,0);
		return (4);
	break;
	case 144: // SUB B
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.B,0,1);
		return (4);
	break;
	case 145: // SUB C
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.C,0,1);
		return (4);
	break;
	case 146: // SUB D
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.D,0,1);
		return (4);
	break;
	case 147: // SUB E
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.E,0,1);
		return (4);
	break;
	case 148: // SUB H
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.H,0,1);
		return (4);
	break;
	case 149: // SUB L
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.L,0,1);
		return (4);
	break;
	case 150: // SUB b(HL)
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,Z80free_Rd_Internal(processor,processor->Rm.wr.HL),0,1);
		return (7);
	break;
	case 151: // SUB A
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.A,0,1);
		return (4);
	break;
	case 152: // SBC A,B
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.B,1,1);
		return (4);
	break;
	case 153: // SBC A,C
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.C,1,1);
		return (4);
	break;
	case 154: // SBC A,D
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.D,1,1);
		return (4);
	break;
	case 155: // SBC A,E
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.E,1,1);
		return (4);
	break;
	case 156: // SBC A,H
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.H,1,1);
		return (4);
	break;
	case 157: // SBC A,L
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.L,1,1);
		return (4);
	break;
	case 158: // SBC A,(HL)
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,Z80free_Rd_Internal(processor,processor->Rm.wr.HL),1,1);
		return (7);
	break;
	case 159: // SBC A,A
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.A,1,1);
		return (4);
	break;
	case 160: // AND B
		Z80free_doAND(processor,processor->Rm.br.B);
		return (4);
	break;
	case 161: // AND C
		Z80free_doAND(processor,processor->Rm.br.C);
		return (4);
	break;
	case 162: // AND D
		Z80free_doAND(processor,processor->Rm.br.D);
		return (4);
	break;
	case 163: // AND E
		Z80free_doAND(processor,processor->Rm.br.E);
		return (4);
	break;
	case 164: // AND H
		Z80free_doAND(processor,processor->Rm.br.H);
		return (4);
	break;
	case 165: // AND L
		Z80free_doAND(processor,processor->Rm.br.L);
		return (4);
	break;
	case 166: // AND b(HL)
		Z80free_doAND(processor,Z80free_Rd_Internal(processor,processor->Rm.wr.HL));
		return (7);
	break;
	case 167: // AND A
		Z80free_doAND(processor,processor->Rm.br.A);
		return (4);
	break;
	case 168: // XOR B
		Z80free_doXOR(processor,processor->Rm.br.B);
		return (4);
	break;
	case 169: // XOR C
		Z80free_doXOR(processor,processor->Rm.br.C);
		return (4);
	break;
	case 170: // XOR D
		Z80free_doXOR(processor,processor->Rm.br.D);
		return (4);
	break;
	case 171: // XOR E
		Z80free_doXOR(processor,processor->Rm.br.E);
		return (4);
	break;
	case 172: // XOR H
		Z80free_doXOR(processor,processor->Rm.br.H);
		return (4);
	break;
	case 173: // XOR L
		Z80free_doXOR(processor,processor->Rm.br.L);
		return (4);
	break;
	case 174: // XOR b(HL)
		Z80free_doXOR(processor,Z80free_Rd_Internal(processor,processor->Rm.wr.HL));
		return (7);
	break;
	case 175: // XOR A
		Z80free_doXOR(processor,processor->Rm.br.A);
		return (4);
	break;
	case 176: // OR B
		Z80free_doOR(processor,processor->Rm.br.B);
		return (4);
	break;
	case 177: // OR C
		Z80free_doOR(processor,processor->Rm.br.C);
		return (4);
	break;
	case 178: // OR D
		Z80free_doOR(processor,processor->Rm.br.D);
		return (4);
	break;
	case 179: // OR E
		Z80free_doOR(processor,processor->Rm.br.E);
		return (4);
	break;
	case 180: // OR H
		Z80free_doOR(processor,processor->Rm.br.H);
		return (4);
	break;
	case 181: // OR L
		Z80free_doOR(processor,processor->Rm.br.L);
		return (4);
	break;
	case 182: // OR b(HL)
		Z80free_doOR(processor,Z80free_Rd_Internal(processor,processor->Rm.wr.HL));
		return (7);
	break;
	case 183: // OR A
		Z80free_doOR(processor,processor->Rm.br.A);
		return (4);
	break;
	case 184: // CP B
		tmp1=processor->Rm.br.B;
		Z80free_doArithmetic(processor,processor->Rm.br.A,tmp1,0,1);
		Z80free_adjustFlags(processor,tmp1);
		return (4);
	break;
	case 185: // CP C
		tmp1=processor->Rm.br.C;
		Z80free_doArithmetic(processor,processor->Rm.br.A,tmp1,0,1);
		Z80free_adjustFlags(processor,tmp1);
		return (4);
	break;
	case 186: // CP D
		tmp1=processor->Rm.br.D;
		Z80free_doArithmetic(processor,processor->Rm.br.A,tmp1,0,1);
		Z80free_adjustFlags(processor,tmp1);
		return (4);
	break;
	case 187: // CP E
		tmp1=processor->Rm.br.E;
		Z80free_doArithmetic(processor,processor->Rm.br.A,tmp1,0,1);
		Z80free_adjustFlags(processor,tmp1);
		return (4);
	break;
	case 188: // CP H
		tmp1=processor->Rm.br.H;
		Z80free_doArithmetic(processor,processor->Rm.br.A,tmp1,0,1);
		Z80free_adjustFlags(processor,tmp1);
		return (4);
	break;
	case 189: // CP L
		tmp1=processor->Rm.br.L;
		Z80free_doArithmetic(processor,processor->Rm.br.A,tmp1,0,1);
		Z80free_adjustFlags(processor,tmp1);
		return (4);
	break;
	case 190: // CP b(HL)
		tmp1=Z80free_Rd_Internal(processor,processor->Rm.wr.HL);
		Z80free_doArithmetic(processor,processor->Rm.br.A,tmp1,0,1);
		Z80free_adjustFlags(processor,tmp1);
		return (7);
	break;
	case 191: // CP A
		tmp1=processor->Rm.br.A;
		Z80free_doArithmetic(processor,processor->Rm.br.A,tmp1,0,1);
		Z80free_adjustFlags(processor,tmp1);
		return (4);
	break;
	case 192: // RET NZ
		if (0==(F_Z&processor->Rm.br.F)) {
			processor->PC=Z80free_doPop(processor);
			return (11);
		} else {
			return (5);
		}
	break;
	case 193: // POP BC
		processor->Rm.wr.BC=Z80free_doPop(processor);
		return (10);
	break;
	case 194: // JP NZ,nn
		tmp2=Z80free_read_param_16(processor);
		if (0==(F_Z&processor->Rm.br.F)) {
			processor->PC=tmp2;
			return (10);
		} else {
			return (10);
		}
	break;
	case 195: // JP nn
		tmp2=Z80free_read_param_16(processor);
		processor->PC=tmp2;
		return (10);
	break;
	case 196: // CALL NZ,nn
		tmp2=Z80free_read_param_16(processor);
		if (0==(F_Z&processor->Rm.br.F)) {
			Z80free_doPush(processor,processor->PC);
			processor->PC=tmp2;
			return (17);
		} else {
			return (10);
		}
	break;
	case 197: // PUSH BC
		Z80free_doPush(processor,processor->Rm.wr.BC);
		return (11);
	break;
	case 198: // ADD A,n
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,Z80free_read_param_8(processor),0,0);
		return (7);
	break;
	case 199: // RST 0H
		Z80free_doPush(processor,processor->PC);
		processor->PC=0;
		return (11);
	break;
	case 200: // RET Z
		if (F_Z&processor->Rm.br.F) {
			processor->PC=Z80free_doPop(processor);
			return (11);
		} else {
			return (5);
		}
	break;
	case 201: // RET
		processor->PC=Z80free_doPop(processor);
		return (10);
	break;
	case 202: // JP Z,nn
		tmp2=Z80free_read_param_16(processor);
		if (F_Z&processor->Rm.br.F) {
			processor->PC=tmp2;
			return (10);
		} else {
			return (10);
		}
	break;
	case 204: // CALL Z,nn
		tmp2=Z80free_read_param_16(processor);
		if (F_Z&processor->Rm.br.F) {
			Z80free_doPush(processor,processor->PC);
			processor->PC=tmp2;
			return (17);
		} else {
			return (10);
		}
	break;
	case 205: // CALL nn
		tmp2=Z80free_read_param_16(processor);
		Z80free_doPush(processor,processor->PC);
		processor->PC=tmp2;
		return (17);
	break;
	case 206: // ADC A,n
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,Z80free_read_param_8(processor),1,0);
		return (7);
	break;
	case 207: // RST 8H
		Z80free_doPush(processor,processor->PC);
		processor->PC=8;
		return (11);
	break;
	case 208: // RET NC
		if (0==(F_C&processor->Rm.br.F)) {
			processor->PC=Z80free_doPop(processor);
			return (11);
		} else {
			return (5);
		}
	break;
	case 209: // POP DE
		processor->Rm.wr.DE=Z80free_doPop(processor);
		return (10);
	break;
	case 210: // JP NC,nn
		tmp2=Z80free_read_param_16(processor);
		if (0==(F_C&processor->Rm.br.F)) {
			processor->PC=tmp2;
			return (10);
		} else {
			return (10);
		}
	break;
	case 211: // OUT n,A
		tmp1=processor->Rm.br.A;
		tmp2=((word)tmp1)<<8;
		Z80free_Out_Internal(processor,tmp2+((word)Z80free_read_param_8(processor)),tmp1);
		return (11);
	break;
	case 212: // CALL NC,nn
		tmp2=Z80free_read_param_16(processor);
		if (0==(F_C&processor->Rm.br.F)) {
			Z80free_doPush(processor,processor->PC);
			processor->PC=tmp2;
			return (17);
		} else {
			return (10);
		}
	break;
	case 213: // PUSH DE
		Z80free_doPush(processor,processor->Rm.wr.DE);
		return (11);
	break;
	case 214: // SUB n
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,Z80free_read_param_8(processor),0,1);
		return (7);
	break;
	case 215: // RST 10H
		Z80free_doPush(processor,processor->PC);
		processor->PC=16;
		return (11);
	break;
	case 216: // RET CF
		if (F_C&processor->Rm.br.F) {
			processor->PC=Z80free_doPop(processor);
			return (11);
		} else {
			return (5);
		}
	break;
	case 217: // EXX
		tmp2=processor->Rm.wr.BC;
		processor->Rm.wr.BC=processor->Ra.wr.BC;
		processor->Ra.wr.BC=tmp2;
		tmp2=processor->Rm.wr.DE;
		processor->Rm.wr.DE=processor->Ra.wr.DE;
		processor->Ra.wr.DE=tmp2;
		tmp2=processor->Rm.wr.HL;
		processor->Rm.wr.HL=processor->Ra.wr.HL;
		processor->Ra.wr.HL=tmp2;
		return (4);
	break;
	case 218: // JP CF,nn
		tmp2=Z80free_read_param_16(processor);
		if (F_C&processor->Rm.br.F) {
			processor->PC=tmp2;
			return (10);
		} else {
			return (10);
		}
	break;
	case 219: // IN A,n
		tmp2=((word)processor->Rm.br.A)<<8;
		processor->Rm.br.A=Z80free_In_Internal(processor,tmp2+((word)Z80free_read_param_8(processor)));
		return (11);
	break;
	case 220: // CALL CF,nn
		tmp2=Z80free_read_param_16(processor);
		if (F_C&processor->Rm.br.F) {
			Z80free_doPush(processor,processor->PC);
			processor->PC=tmp2;
			return (17);
		} else {
			return (10);
		}
	break;
	case 222: // SBC A,n
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,Z80free_read_param_8(processor),1,1);
		return (7);
	break;
	case 223: // RST 18H
		Z80free_doPush(processor,processor->PC);
		processor->PC=24;
		return (11);
	break;
	case 224: // RET PO
		if (0==(F_PV&processor->Rm.br.F)) {
			processor->PC=Z80free_doPop(processor);
			return (11);
		} else {
			return (5);
		}
	break;
	case 225: // POP HL
		processor->Rm.wr.HL=Z80free_doPop(processor);
		return (10);
	break;
	case 226: // JP PO,nn
		tmp2=Z80free_read_param_16(processor);
		if (0==(F_PV&processor->Rm.br.F)) {
			processor->PC=tmp2;
			return (10);
		} else {
			return (10);
		}
	break;
	case 227: // EX (SP),HL
		tmp2=Z80free_read16(processor, processor->Rm.wr.SP);
		Z80free_write16(processor, processor->Rm.wr.SP,processor->Rm.wr.HL);
		processor->Rm.wr.HL=tmp2;
		return (19);
	break;
	case 228: // CALL PO,nn
		tmp2=Z80free_read_param_16(processor);
		if (0==(F_PV&processor->Rm.br.F)) {
			Z80free_doPush(processor,processor->PC);
			processor->PC=tmp2;
			return (17);
		} else {
			return (10);
		}
	break;
	case 229: // PUSH HL
		Z80free_doPush(processor,processor->Rm.wr.HL);
		return (11);
	break;
	case 230: // AND n
		Z80free_doAND(processor,Z80free_read_param_8(processor));
		return (7);
	break;
	case 231: // RST 20H
		Z80free_doPush(processor,processor->PC);
		processor->PC=32;
		return (11);
	break;
	case 232: // RET PE
		if (F_PV&processor->Rm.br.F) {
			processor->PC=Z80free_doPop(processor);
			return (11);
		} else {
			return (5);
		}
	break;
	case 233: // JP HL
		tmp2=processor->Rm.wr.HL;
		processor->PC=tmp2;
		return (4);
	break;
	case 234: // JP PE,nn
		tmp2=Z80free_read_param_16(processor);
		if (F_PV&processor->Rm.br.F) {
			processor->PC=tmp2;
			return (10);
		} else {
			return (10);
		}
	break;
	case 235: // EX DE,HL
		tmp2=processor->Rm.wr.DE;
		processor->Rm.wr.DE=processor->Rm.wr.HL;
		processor->Rm.wr.HL=tmp2;
		return (4);
	break;
	case 236: // CALL PE,nn
		tmp2=Z80free_read_param_16(processor);
		if (F_PV&processor->Rm.br.F) {
			Z80free_doPush(processor,processor->PC);
			processor->PC=tmp2;
			return (17);
		} else {
			return (10);
		}
	break;
	case 238: // XOR n
		Z80free_doXOR(processor,Z80free_read_param_8(processor));
		return (7);
	break;
	case 239: // RST 28H
		Z80free_doPush(processor,processor->PC);
		processor->PC=40;
		return (11);
	break;
	case 240: // RET P
		if (0==(F_S&processor->Rm.br.F)) {
			processor->PC=Z80free_doPop(processor);
			return (11);
		} else {
			return (5);
		}
	break;
	case 241: // POP AF
		processor->Rm.wr.AF=Z80free_doPop(processor);
		return (10);
	break;
	case 242: // JP P,nn
		tmp2=Z80free_read_param_16(processor);
		if (0==(F_S&processor->Rm.br.F)) {
			processor->PC=tmp2;
			return (10);
		} else {
			return (10);
		}
	break;
	case 243: // DI
		processor->IFF1=0;
		processor->IFF2=0;
		return (4);
	break;
	case 244: // CALL P,nn
		tmp2=Z80free_read_param_16(processor);
		if (0==(F_S&processor->Rm.br.F)) {
			Z80free_doPush(processor,processor->PC);
			processor->PC=tmp2;
			return (17);
		} else {
			return (10);
		}
	break;
	case 245: // PUSH AF
		Z80free_doPush(processor,processor->Rm.wr.AF);
		return (11);
	break;
	case 246: // OR n
		Z80free_doOR(processor,Z80free_read_param_8(processor));
		return (7);
	break;
	case 247: // RST 30H
		Z80free_doPush(processor,processor->PC);
		processor->PC=48;
		return (11);
	break;
	case 248: // RET M
		if (F_S&processor->Rm.br.F) {
			processor->PC=Z80free_doPop(processor);
			return (11);
		} else {
			return (5);
		}
	break;
	case 249: // LD SP,HL
		processor->Rm.wr.SP=processor->Rm.wr.HL;
		return (6);
	break;
	case 250: // JP M,nn
		tmp2=Z80free_read_param_16(processor);
		if (F_S&processor->Rm.br.F) {
			processor->PC=tmp2;
			return (10);
		} else {
			return (10);
		}
	break;
	case 251: // EI
		processor->IFF1=2; /* this allows to delay one instruction the interrupts*/
		processor->IFF2=1;
		return (4);
	break;
	case 252: // CALL M,nn
		tmp2=Z80free_read_param_16(processor);
		if (F_S&processor->Rm.br.F) {
			Z80free_doPush(processor,processor->PC);
			processor->PC=tmp2;
			return (17);
		} else {
			return (10);
		}
	break;
	case 254: // CP n
		tmp1=Z80free_read_param_8(processor);
		Z80free_doArithmetic(processor,processor->Rm.br.A,tmp1,0,1);
		Z80free_adjustFlags(processor,tmp1);
		return (7);
	break;
	case 255: // RST 38H
		Z80free_doPush(processor,processor->PC);
		processor->PC=56;
		return (11);
	break;
	}

	return -1;
}
