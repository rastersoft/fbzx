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

int Z80free_codesFD (Z80FREE *processor,byte opcode) {
	static byte tmp1;
	static word tmp2;

	switch(opcode) {
	case 0: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 1: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 2: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 3: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 4: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 5: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 6: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 7: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 8: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 9: // ADD IY,BC
		processor->Rm.wr.IY=Z80free_doArithmetic16(processor,processor->Rm.wr.IY,processor->Rm.wr.BC,0,0);
		return (11);
	break;
	case 10: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 11: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 12: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 13: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 14: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 15: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 16: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 17: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 18: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 19: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 20: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 21: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 22: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 23: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 24: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 25: // ADD IY,DE
		processor->Rm.wr.IY=Z80free_doArithmetic16(processor,processor->Rm.wr.IY,processor->Rm.wr.DE,0,0);
		return (11);
	break;
	case 26: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 27: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 28: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 29: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 30: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 31: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 32: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 33: // LD IY,nn
		processor->Rm.wr.IY=Z80free_read_param_16(processor);
		return (10);
	break;
	case 34: // LD (nn),IY
		Z80free_write16(processor, Z80free_read_param_16(processor),processor->Rm.wr.IY);
		return (16);
	break;
	case 35: // INC IY
		processor->Rm.wr.IY=Z80free_doIncDec16(processor,processor->Rm.wr.IY,0);
		return (6);
	break;
	case 36: // INC IYH
		processor->Rm.br.IYh=Z80free_doIncDec(processor,processor->Rm.br.IYh,0);
		return (4);
	break;
	case 37: // DEC IYH
		processor->Rm.br.IYh=Z80free_doIncDec(processor,processor->Rm.br.IYh,1);
		return (4);
	break;
	case 38: // LD IYH,n
		processor->Rm.br.IYh=Z80free_read_param_8(processor);
		return (7);
	break;
	case 39: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 40: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 41: // ADD IY,IY
		processor->Rm.wr.IY=Z80free_doArithmetic16(processor,processor->Rm.wr.IY,processor->Rm.wr.IY,0,0);
		return (11);
	break;
	case 42: // LD IY,(nn)
		processor->Rm.wr.IY=Z80free_read16(processor, Z80free_read_param_16(processor));
		return (16);
	break;
	case 43: // DEC IY
		processor->Rm.wr.IY=Z80free_doIncDec16(processor,processor->Rm.wr.IY,1);
		return (6);
	break;
	case 44: // INC IYL
		processor->Rm.br.IYl=Z80free_doIncDec(processor,processor->Rm.br.IYl,0);
		return (4);
	break;
	case 45: // DEC IYL
		processor->Rm.br.IYl=Z80free_doIncDec(processor,processor->Rm.br.IYl,1);
		return (4);
	break;
	case 46: // LD IYL,n
		processor->Rm.br.IYl=Z80free_read_param_8(processor);
		return (7);
	break;
	case 47: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 48: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 49: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 50: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 51: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 52: // INC b(IY+d)
		Z80free_Wr_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY),Z80free_doIncDec(processor,Z80free_Rd_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY)),0));
		return (19);
	break;
	case 53: // DEC b(IY+d)
		Z80free_Wr_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY),Z80free_doIncDec(processor,Z80free_Rd_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY)),1));
		return (19);
	break;
	case 54: // LD2 IY+d,n
		tmp2=Z80free_addr_relative(processor,processor->Rm.wr.IY);
		Z80free_Wr_Internal(processor,tmp2,Z80free_read_param_8(processor));
		return (15);
	break;
	case 55: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 56: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 57: // ADD IY,SP
		processor->Rm.wr.IY=Z80free_doArithmetic16(processor,processor->Rm.wr.IY,processor->Rm.wr.SP,0,0);
		return (11);
	break;
	case 58: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 59: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 60: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 61: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 62: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 63: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 64: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 65: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 66: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 67: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 68: // LD B,IYH
		processor->Rm.br.B=processor->Rm.br.IYh;
		return (4);
	break;
	case 69: // LD B,IYL
		processor->Rm.br.B=processor->Rm.br.IYl;
		return (4);
	break;
	case 70: // LD B,(IY+d)
		processor->Rm.br.B=Z80free_Rd_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY));
		return (15);
	break;
	case 71: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 72: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 73: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 74: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 75: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 76: // LD C,IYH
		processor->Rm.br.C=processor->Rm.br.IYh;
		return (4);
	break;
	case 77: // LD C,IYL
		processor->Rm.br.C=processor->Rm.br.IYl;
		return (4);
	break;
	case 78: // LD C,(IY+d)
		processor->Rm.br.C=Z80free_Rd_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY));
		return (15);
	break;
	case 79: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 80: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 81: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 82: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 83: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 84: // LD D,IYH
		processor->Rm.br.D=processor->Rm.br.IYh;
		return (4);
	break;
	case 85: // LD D,IYL
		processor->Rm.br.D=processor->Rm.br.IYl;
		return (4);
	break;
	case 86: // LD D,(IY+d)
		processor->Rm.br.D=Z80free_Rd_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY));
		return (15);
	break;
	case 87: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 88: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 89: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 90: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 91: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 92: // LD E,IYH
		processor->Rm.br.E=processor->Rm.br.IYh;
		return (4);
	break;
	case 93: // LD E,IYL
		processor->Rm.br.E=processor->Rm.br.IYl;
		return (4);
	break;
	case 94: // LD E,(IY+d)
		processor->Rm.br.E=Z80free_Rd_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY));
		return (15);
	break;
	case 95: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 96: // LD IYH,B
		processor->Rm.br.IYh=processor->Rm.br.B;
		return (4);
	break;
	case 97: // LD IYH,C
		processor->Rm.br.IYh=processor->Rm.br.C;
		return (4);
	break;
	case 98: // LD IYH,D
		processor->Rm.br.IYh=processor->Rm.br.D;
		return (4);
	break;
	case 99: // LD IYH,E
		processor->Rm.br.IYh=processor->Rm.br.E;
		return (4);
	break;
	case 100: // LD IYH,IYH
		processor->Rm.br.IYh=processor->Rm.br.IYh;
		return (4);
	break;
	case 101: // LD IYH,IYL
		processor->Rm.br.IYh=processor->Rm.br.IYl;
		return (4);
	break;
	case 102: // LD H,(IY+d)
		processor->Rm.br.H=Z80free_Rd_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY));
		return (15);
	break;
	case 103: // LD IYH,A
		processor->Rm.br.IYh=processor->Rm.br.A;
		return (4);
	break;
	case 104: // LD IYL,B
		processor->Rm.br.IYl=processor->Rm.br.B;
		return (4);
	break;
	case 105: // LD IYL,C
		processor->Rm.br.IYl=processor->Rm.br.C;
		return (4);
	break;
	case 106: // LD IYL,D
		processor->Rm.br.IYl=processor->Rm.br.D;
		return (4);
	break;
	case 107: // LD IYL,E
		processor->Rm.br.IYl=processor->Rm.br.E;
		return (4);
	break;
	case 108: // LD IYL,IYH
		processor->Rm.br.IYl=processor->Rm.br.IYh;
		return (4);
	break;
	case 109: // LD IYL,IYL
		processor->Rm.br.IYl=processor->Rm.br.IYl;
		return (4);
	break;
	case 110: // LD L,(IY+d)
		processor->Rm.br.L=Z80free_Rd_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY));
		return (15);
	break;
	case 111: // LD IYL,A
		processor->Rm.br.IYl=processor->Rm.br.A;
		return (4);
	break;
	case 112: // LD (IY+d),B
		Z80free_Wr_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY),processor->Rm.br.B);
		return (15);
	break;
	case 113: // LD (IY+d),C
		Z80free_Wr_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY),processor->Rm.br.C);
		return (15);
	break;
	case 114: // LD (IY+d),D
		Z80free_Wr_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY),processor->Rm.br.D);
		return (15);
	break;
	case 115: // LD (IY+d),E
		Z80free_Wr_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY),processor->Rm.br.E);
		return (15);
	break;
	case 116: // LD (IY+d),H
		Z80free_Wr_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY),processor->Rm.br.H);
		return (15);
	break;
	case 117: // LD (IY+d),L
		Z80free_Wr_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY),processor->Rm.br.L);
		return (15);
	break;
	case 118: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 119: // LD (IY+d),A
		Z80free_Wr_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY),processor->Rm.br.A);
		return (15);
	break;
	case 120: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 121: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 122: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 123: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 124: // LD A,IYH
		processor->Rm.br.A=processor->Rm.br.IYh;
		return (4);
	break;
	case 125: // LD A,IYL
		processor->Rm.br.A=processor->Rm.br.IYl;
		return (4);
	break;
	case 126: // LD A,(IY+d)
		processor->Rm.br.A=Z80free_Rd_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY));
		return (15);
	break;
	case 127: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 128: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 129: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 130: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 131: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 132: // ADD A,IYH
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.IYh,0,0);
		return (4);
	break;
	case 133: // ADD A,IYL
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.IYl,0,0);
		return (4);
	break;
	case 134: // ADD A,(IY+d)
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,Z80free_Rd_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY)),0,0);
		return (15);
	break;
	case 135: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 136: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 137: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 138: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 139: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 140: // ADC A,IYH
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.IYh,1,0);
		return (4);
	break;
	case 141: // ADC A,IYL
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.IYl,1,0);
		return (4);
	break;
	case 142: // ADC A,(IY+d)
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,Z80free_Rd_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY)),1,0);
		return (15);
	break;
	case 143: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 144: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 145: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 146: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 147: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 148: // SUB IYH
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.IYh,0,1);
		return (4);
	break;
	case 149: // SUB IYL
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.IYl,0,1);
		return (4);
	break;
	case 150: // SUB b(IY+d)
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,Z80free_Rd_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY)),0,1);
		return (15);
	break;
	case 151: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 152: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 153: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 154: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 155: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 156: // SBC A,IYH
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.IYh,1,1);
		return (4);
	break;
	case 157: // SBC A,IYL
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,processor->Rm.br.IYl,1,1);
		return (4);
	break;
	case 158: // SBC A,(IY+d)
		processor->Rm.br.A=Z80free_doArithmetic(processor,processor->Rm.br.A,Z80free_Rd_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY)),1,1);
		return (15);
	break;
	case 159: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 160: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 161: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 162: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 163: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 164: // AND IYH
		Z80free_doAND(processor,processor->Rm.br.IYh);
		return (4);
	break;
	case 165: // AND IYL
		Z80free_doAND(processor,processor->Rm.br.IYl);
		return (4);
	break;
	case 166: // AND b(IY+d)
		Z80free_doAND(processor,Z80free_Rd_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY)));
		return (15);
	break;
	case 167: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 168: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 169: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 170: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 171: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 172: // XOR IYH
		Z80free_doXOR(processor,processor->Rm.br.IYh);
		return (4);
	break;
	case 173: // XOR IYL
		Z80free_doXOR(processor,processor->Rm.br.IYl);
		return (4);
	break;
	case 174: // XOR b(IY+d)
		Z80free_doXOR(processor,Z80free_Rd_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY)));
		return (15);
	break;
	case 175: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 176: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 177: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 178: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 179: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 180: // OR IYH
		Z80free_doOR(processor,processor->Rm.br.IYh);
		return (4);
	break;
	case 181: // OR IYL
		Z80free_doOR(processor,processor->Rm.br.IYl);
		return (4);
	break;
	case 182: // OR b(IY+d)
		Z80free_doOR(processor,Z80free_Rd_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY)));
		return (15);
	break;
	case 183: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 184: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 185: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 186: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 187: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 188: // CP IYH
		tmp1=processor->Rm.br.IYh;
		Z80free_doArithmetic(processor,processor->Rm.br.A,tmp1,0,1);
		Z80free_adjustFlags(processor,tmp1);
		return (4);
	break;
	case 189: // CP IYL
		tmp1=processor->Rm.br.IYl;
		Z80free_doArithmetic(processor,processor->Rm.br.A,tmp1,0,1);
		Z80free_adjustFlags(processor,tmp1);
		return (4);
	break;
	case 190: // CP b(IY+d)
		tmp1=Z80free_Rd_Internal(processor,Z80free_addr_relative(processor,processor->Rm.wr.IY));
		Z80free_doArithmetic(processor,processor->Rm.br.A,tmp1,0,1);
		Z80free_adjustFlags(processor,tmp1);
		return (15);
	break;
	case 191: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 192: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 193: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 194: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 195: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 196: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 197: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 198: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 199: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 200: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 201: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 202: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 204: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 205: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 206: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 207: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 208: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 209: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 210: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 211: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 212: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 213: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 214: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 215: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 216: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 217: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 218: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 219: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 220: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 221: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 222: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 223: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 224: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 225: // POP IY
		processor->Rm.wr.IY=Z80free_doPop(processor);
		return (10);
	break;
	case 226: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 227: // EX (SP),IY
		tmp2=Z80free_read16(processor, processor->Rm.wr.SP);
		Z80free_write16(processor, processor->Rm.wr.SP,processor->Rm.wr.IY);
		processor->Rm.wr.IY=tmp2;
		return (19);
	break;
	case 228: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 229: // PUSH IY
		Z80free_doPush(processor,processor->Rm.wr.IY);
		return (11);
	break;
	case 230: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 231: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 232: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 233: // JP IY
		tmp2=processor->Rm.wr.IY;
		processor->PC=tmp2;
		return (4);
	break;
	case 234: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 235: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 236: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 237: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 238: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 239: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 240: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 241: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 242: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 243: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 244: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 245: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 246: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 247: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 248: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 249: // LD SP,IY
		processor->Rm.wr.SP=processor->Rm.wr.IY;
		return (6);
	break;
	case 250: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 251: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 252: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 253: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 254: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	case 255: // DEFAULT
		return (Z80free_codes(processor,opcode));
	break;
	}

	return -1;
}
