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

int Z80free_codesCB (Z80FREE *processor,byte opcode) {

	switch(opcode) {
	case 0: // RLC B
		processor->Rm.br.B=Z80free_doRLC(processor,1,processor->Rm.br.B);
		return (4);
	break;
	case 1: // RLC C
		processor->Rm.br.C=Z80free_doRLC(processor,1,processor->Rm.br.C);
		return (4);
	break;
	case 2: // RLC D
		processor->Rm.br.D=Z80free_doRLC(processor,1,processor->Rm.br.D);
		return (4);
	break;
	case 3: // RLC E
		processor->Rm.br.E=Z80free_doRLC(processor,1,processor->Rm.br.E);
		return (4);
	break;
	case 4: // RLC H
		processor->Rm.br.H=Z80free_doRLC(processor,1,processor->Rm.br.H);
		return (4);
	break;
	case 5: // RLC L
		processor->Rm.br.L=Z80free_doRLC(processor,1,processor->Rm.br.L);
		return (4);
	break;
	case 6: // RLC b(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doRLC(processor,1,Z80free_Rd_Internal(processor,processor->Rm.wr.HL)));
		return (11);
	break;
	case 7: // RLC A
		processor->Rm.br.A=Z80free_doRLC(processor,1,processor->Rm.br.A);
		return (4);
	break;
	case 8: // RRC B
		processor->Rm.br.B=Z80free_doRRC(processor,1,processor->Rm.br.B);
		return (4);
	break;
	case 9: // RRC C
		processor->Rm.br.C=Z80free_doRRC(processor,1,processor->Rm.br.C);
		return (4);
	break;
	case 10: // RRC D
		processor->Rm.br.D=Z80free_doRRC(processor,1,processor->Rm.br.D);
		return (4);
	break;
	case 11: // RRC E
		processor->Rm.br.E=Z80free_doRRC(processor,1,processor->Rm.br.E);
		return (4);
	break;
	case 12: // RRC H
		processor->Rm.br.H=Z80free_doRRC(processor,1,processor->Rm.br.H);
		return (4);
	break;
	case 13: // RRC L
		processor->Rm.br.L=Z80free_doRRC(processor,1,processor->Rm.br.L);
		return (4);
	break;
	case 14: // RRC b(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doRRC(processor,1,Z80free_Rd_Internal(processor,processor->Rm.wr.HL)));
		return (11);
	break;
	case 15: // RRC A
		processor->Rm.br.A=Z80free_doRRC(processor,1,processor->Rm.br.A);
		return (4);
	break;
	case 16: // RL B
		processor->Rm.br.B=Z80free_doRL(processor,1,processor->Rm.br.B);
		return (4);
	break;
	case 17: // RL C
		processor->Rm.br.C=Z80free_doRL(processor,1,processor->Rm.br.C);
		return (4);
	break;
	case 18: // RL D
		processor->Rm.br.D=Z80free_doRL(processor,1,processor->Rm.br.D);
		return (4);
	break;
	case 19: // RL E
		processor->Rm.br.E=Z80free_doRL(processor,1,processor->Rm.br.E);
		return (4);
	break;
	case 20: // RL H
		processor->Rm.br.H=Z80free_doRL(processor,1,processor->Rm.br.H);
		return (4);
	break;
	case 21: // RL L
		processor->Rm.br.L=Z80free_doRL(processor,1,processor->Rm.br.L);
		return (4);
	break;
	case 22: // RL b(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doRL(processor,1,Z80free_Rd_Internal(processor,processor->Rm.wr.HL)));
		return (11);
	break;
	case 23: // RL A
		processor->Rm.br.A=Z80free_doRL(processor,1,processor->Rm.br.A);
		return (4);
	break;
	case 24: // RR B
		processor->Rm.br.B=Z80free_doRR(processor,1,processor->Rm.br.B);
		return (4);
	break;
	case 25: // RR C
		processor->Rm.br.C=Z80free_doRR(processor,1,processor->Rm.br.C);
		return (4);
	break;
	case 26: // RR D
		processor->Rm.br.D=Z80free_doRR(processor,1,processor->Rm.br.D);
		return (4);
	break;
	case 27: // RR E
		processor->Rm.br.E=Z80free_doRR(processor,1,processor->Rm.br.E);
		return (4);
	break;
	case 28: // RR H
		processor->Rm.br.H=Z80free_doRR(processor,1,processor->Rm.br.H);
		return (4);
	break;
	case 29: // RR L
		processor->Rm.br.L=Z80free_doRR(processor,1,processor->Rm.br.L);
		return (4);
	break;
	case 30: // RR b(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doRR(processor,1,Z80free_Rd_Internal(processor,processor->Rm.wr.HL)));
		return (11);
	break;
	case 31: // RR A
		processor->Rm.br.A=Z80free_doRR(processor,1,processor->Rm.br.A);
		return (4);
	break;
	case 32: // SLA B
		processor->Rm.br.B=Z80free_doSL(processor,0,processor->Rm.br.B);
		return (4);
	break;
	case 33: // SLA C
		processor->Rm.br.C=Z80free_doSL(processor,0,processor->Rm.br.C);
		return (4);
	break;
	case 34: // SLA D
		processor->Rm.br.D=Z80free_doSL(processor,0,processor->Rm.br.D);
		return (4);
	break;
	case 35: // SLA E
		processor->Rm.br.E=Z80free_doSL(processor,0,processor->Rm.br.E);
		return (4);
	break;
	case 36: // SLA H
		processor->Rm.br.H=Z80free_doSL(processor,0,processor->Rm.br.H);
		return (4);
	break;
	case 37: // SLA L
		processor->Rm.br.L=Z80free_doSL(processor,0,processor->Rm.br.L);
		return (4);
	break;
	case 38: // SLA b(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doSL(processor,0,Z80free_Rd_Internal(processor,processor->Rm.wr.HL)));
		return (11);
	break;
	case 39: // SLA A
		processor->Rm.br.A=Z80free_doSL(processor,0,processor->Rm.br.A);
		return (4);
	break;
	case 40: // SRA B
		processor->Rm.br.B=Z80free_doSR(processor,1,processor->Rm.br.B);
		return (4);
	break;
	case 41: // SRA C
		processor->Rm.br.C=Z80free_doSR(processor,1,processor->Rm.br.C);
		return (4);
	break;
	case 42: // SRA D
		processor->Rm.br.D=Z80free_doSR(processor,1,processor->Rm.br.D);
		return (4);
	break;
	case 43: // SRA E
		processor->Rm.br.E=Z80free_doSR(processor,1,processor->Rm.br.E);
		return (4);
	break;
	case 44: // SRA H
		processor->Rm.br.H=Z80free_doSR(processor,1,processor->Rm.br.H);
		return (4);
	break;
	case 45: // SRA L
		processor->Rm.br.L=Z80free_doSR(processor,1,processor->Rm.br.L);
		return (4);
	break;
	case 46: // SRA b(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doSR(processor,1,Z80free_Rd_Internal(processor,processor->Rm.wr.HL)));
		return (11);
	break;
	case 47: // SRA A
		processor->Rm.br.A=Z80free_doSR(processor,1,processor->Rm.br.A);
		return (4);
	break;
	case 48: // SLL B
		processor->Rm.br.B=Z80free_doSL(processor,1,processor->Rm.br.B);
		return (4);
	break;
	case 49: // SLL C
		processor->Rm.br.C=Z80free_doSL(processor,1,processor->Rm.br.C);
		return (4);
	break;
	case 50: // SLL D
		processor->Rm.br.D=Z80free_doSL(processor,1,processor->Rm.br.D);
		return (4);
	break;
	case 51: // SLL E
		processor->Rm.br.E=Z80free_doSL(processor,1,processor->Rm.br.E);
		return (4);
	break;
	case 52: // SLL H
		processor->Rm.br.H=Z80free_doSL(processor,1,processor->Rm.br.H);
		return (4);
	break;
	case 53: // SLL L
		processor->Rm.br.L=Z80free_doSL(processor,1,processor->Rm.br.L);
		return (4);
	break;
	case 54: // SLL b(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doSL(processor,1,Z80free_Rd_Internal(processor,processor->Rm.wr.HL)));
		return (11);
	break;
	case 55: // SLL A
		processor->Rm.br.A=Z80free_doSL(processor,1,processor->Rm.br.A);
		return (4);
	break;
	case 56: // SRL B
		processor->Rm.br.B=Z80free_doSR(processor,0,processor->Rm.br.B);
		return (4);
	break;
	case 57: // SRL C
		processor->Rm.br.C=Z80free_doSR(processor,0,processor->Rm.br.C);
		return (4);
	break;
	case 58: // SRL D
		processor->Rm.br.D=Z80free_doSR(processor,0,processor->Rm.br.D);
		return (4);
	break;
	case 59: // SRL E
		processor->Rm.br.E=Z80free_doSR(processor,0,processor->Rm.br.E);
		return (4);
	break;
	case 60: // SRL H
		processor->Rm.br.H=Z80free_doSR(processor,0,processor->Rm.br.H);
		return (4);
	break;
	case 61: // SRL L
		processor->Rm.br.L=Z80free_doSR(processor,0,processor->Rm.br.L);
		return (4);
	break;
	case 62: // SRL b(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doSR(processor,0,Z80free_Rd_Internal(processor,processor->Rm.wr.HL)));
		return (11);
	break;
	case 63: // SRL A
		processor->Rm.br.A=Z80free_doSR(processor,0,processor->Rm.br.A);
		return (4);
	break;
	case 64: // BIT 0,B
		Z80free_doBIT(processor,0,processor->Rm.br.B);
		return (4);
	break;
	case 65: // BIT 0,C
		Z80free_doBIT(processor,0,processor->Rm.br.C);
		return (4);
	break;
	case 66: // BIT 0,D
		Z80free_doBIT(processor,0,processor->Rm.br.D);
		return (4);
	break;
	case 67: // BIT 0,E
		Z80free_doBIT(processor,0,processor->Rm.br.E);
		return (4);
	break;
	case 68: // BIT 0,H
		Z80free_doBIT(processor,0,processor->Rm.br.H);
		return (4);
	break;
	case 69: // BIT 0,L
		Z80free_doBIT(processor,0,processor->Rm.br.L);
		return (4);
	break;
	case 70: // BIT 0,(HL)
		Z80free_doBIT(processor,0,Z80free_Rd_Internal(processor,processor->Rm.wr.HL));
		return (8);
	break;
	case 71: // BIT 0,A
		Z80free_doBIT(processor,0,processor->Rm.br.A);
		return (4);
	break;
	case 72: // BIT 1,B
		Z80free_doBIT(processor,1,processor->Rm.br.B);
		return (4);
	break;
	case 73: // BIT 1,C
		Z80free_doBIT(processor,1,processor->Rm.br.C);
		return (4);
	break;
	case 74: // BIT 1,D
		Z80free_doBIT(processor,1,processor->Rm.br.D);
		return (4);
	break;
	case 75: // BIT 1,E
		Z80free_doBIT(processor,1,processor->Rm.br.E);
		return (4);
	break;
	case 76: // BIT 1,H
		Z80free_doBIT(processor,1,processor->Rm.br.H);
		return (4);
	break;
	case 77: // BIT 1,L
		Z80free_doBIT(processor,1,processor->Rm.br.L);
		return (4);
	break;
	case 78: // BIT 1,(HL)
		Z80free_doBIT(processor,1,Z80free_Rd_Internal(processor,processor->Rm.wr.HL));
		return (8);
	break;
	case 79: // BIT 1,A
		Z80free_doBIT(processor,1,processor->Rm.br.A);
		return (4);
	break;
	case 80: // BIT 2,B
		Z80free_doBIT(processor,2,processor->Rm.br.B);
		return (4);
	break;
	case 81: // BIT 2,C
		Z80free_doBIT(processor,2,processor->Rm.br.C);
		return (4);
	break;
	case 82: // BIT 2,D
		Z80free_doBIT(processor,2,processor->Rm.br.D);
		return (4);
	break;
	case 83: // BIT 2,E
		Z80free_doBIT(processor,2,processor->Rm.br.E);
		return (4);
	break;
	case 84: // BIT 2,H
		Z80free_doBIT(processor,2,processor->Rm.br.H);
		return (4);
	break;
	case 85: // BIT 2,L
		Z80free_doBIT(processor,2,processor->Rm.br.L);
		return (4);
	break;
	case 86: // BIT 2,(HL)
		Z80free_doBIT(processor,2,Z80free_Rd_Internal(processor,processor->Rm.wr.HL));
		return (8);
	break;
	case 87: // BIT 2,A
		Z80free_doBIT(processor,2,processor->Rm.br.A);
		return (4);
	break;
	case 88: // BIT 3,B
		Z80free_doBIT(processor,3,processor->Rm.br.B);
		return (4);
	break;
	case 89: // BIT 3,C
		Z80free_doBIT(processor,3,processor->Rm.br.C);
		return (4);
	break;
	case 90: // BIT 3,D
		Z80free_doBIT(processor,3,processor->Rm.br.D);
		return (4);
	break;
	case 91: // BIT 3,E
		Z80free_doBIT(processor,3,processor->Rm.br.E);
		return (4);
	break;
	case 92: // BIT 3,H
		Z80free_doBIT(processor,3,processor->Rm.br.H);
		return (4);
	break;
	case 93: // BIT 3,L
		Z80free_doBIT(processor,3,processor->Rm.br.L);
		return (4);
	break;
	case 94: // BIT 3,(HL)
		Z80free_doBIT(processor,3,Z80free_Rd_Internal(processor,processor->Rm.wr.HL));
		return (8);
	break;
	case 95: // BIT 3,A
		Z80free_doBIT(processor,3,processor->Rm.br.A);
		return (4);
	break;
	case 96: // BIT 4,B
		Z80free_doBIT(processor,4,processor->Rm.br.B);
		return (4);
	break;
	case 97: // BIT 4,C
		Z80free_doBIT(processor,4,processor->Rm.br.C);
		return (4);
	break;
	case 98: // BIT 4,D
		Z80free_doBIT(processor,4,processor->Rm.br.D);
		return (4);
	break;
	case 99: // BIT 4,E
		Z80free_doBIT(processor,4,processor->Rm.br.E);
		return (4);
	break;
	case 100: // BIT 4,H
		Z80free_doBIT(processor,4,processor->Rm.br.H);
		return (4);
	break;
	case 101: // BIT 4,L
		Z80free_doBIT(processor,4,processor->Rm.br.L);
		return (4);
	break;
	case 102: // BIT 4,(HL)
		Z80free_doBIT(processor,4,Z80free_Rd_Internal(processor,processor->Rm.wr.HL));
		return (8);
	break;
	case 103: // BIT 4,A
		Z80free_doBIT(processor,4,processor->Rm.br.A);
		return (4);
	break;
	case 104: // BIT 5,B
		Z80free_doBIT(processor,5,processor->Rm.br.B);
		return (4);
	break;
	case 105: // BIT 5,C
		Z80free_doBIT(processor,5,processor->Rm.br.C);
		return (4);
	break;
	case 106: // BIT 5,D
		Z80free_doBIT(processor,5,processor->Rm.br.D);
		return (4);
	break;
	case 107: // BIT 5,E
		Z80free_doBIT(processor,5,processor->Rm.br.E);
		return (4);
	break;
	case 108: // BIT 5,H
		Z80free_doBIT(processor,5,processor->Rm.br.H);
		return (4);
	break;
	case 109: // BIT 5,L
		Z80free_doBIT(processor,5,processor->Rm.br.L);
		return (4);
	break;
	case 110: // BIT 5,(HL)
		Z80free_doBIT(processor,5,Z80free_Rd_Internal(processor,processor->Rm.wr.HL));
		return (8);
	break;
	case 111: // BIT 5,A
		Z80free_doBIT(processor,5,processor->Rm.br.A);
		return (4);
	break;
	case 112: // BIT 6,B
		Z80free_doBIT(processor,6,processor->Rm.br.B);
		return (4);
	break;
	case 113: // BIT 6,C
		Z80free_doBIT(processor,6,processor->Rm.br.C);
		return (4);
	break;
	case 114: // BIT 6,D
		Z80free_doBIT(processor,6,processor->Rm.br.D);
		return (4);
	break;
	case 115: // BIT 6,E
		Z80free_doBIT(processor,6,processor->Rm.br.E);
		return (4);
	break;
	case 116: // BIT 6,H
		Z80free_doBIT(processor,6,processor->Rm.br.H);
		return (4);
	break;
	case 117: // BIT 6,L
		Z80free_doBIT(processor,6,processor->Rm.br.L);
		return (4);
	break;
	case 118: // BIT 6,(HL)
		Z80free_doBIT(processor,6,Z80free_Rd_Internal(processor,processor->Rm.wr.HL));
		return (8);
	break;
	case 119: // BIT 6,A
		Z80free_doBIT(processor,6,processor->Rm.br.A);
		return (4);
	break;
	case 120: // BIT 7,B
		Z80free_doBIT(processor,7,processor->Rm.br.B);
		return (4);
	break;
	case 121: // BIT 7,C
		Z80free_doBIT(processor,7,processor->Rm.br.C);
		return (4);
	break;
	case 122: // BIT 7,D
		Z80free_doBIT(processor,7,processor->Rm.br.D);
		return (4);
	break;
	case 123: // BIT 7,E
		Z80free_doBIT(processor,7,processor->Rm.br.E);
		return (4);
	break;
	case 124: // BIT 7,H
		Z80free_doBIT(processor,7,processor->Rm.br.H);
		return (4);
	break;
	case 125: // BIT 7,L
		Z80free_doBIT(processor,7,processor->Rm.br.L);
		return (4);
	break;
	case 126: // BIT 7,(HL)
		Z80free_doBIT(processor,7,Z80free_Rd_Internal(processor,processor->Rm.wr.HL));
		return (8);
	break;
	case 127: // BIT 7,A
		Z80free_doBIT(processor,7,processor->Rm.br.A);
		return (4);
	break;
	case 128: // RES 0,B
		processor->Rm.br.B=Z80free_doSetRes(processor,0,0,processor->Rm.br.B);
		return (4);
	break;
	case 129: // RES 0,C
		processor->Rm.br.C=Z80free_doSetRes(processor,0,0,processor->Rm.br.C);
		return (4);
	break;
	case 130: // RES 0,D
		processor->Rm.br.D=Z80free_doSetRes(processor,0,0,processor->Rm.br.D);
		return (4);
	break;
	case 131: // RES 0,E
		processor->Rm.br.E=Z80free_doSetRes(processor,0,0,processor->Rm.br.E);
		return (4);
	break;
	case 132: // RES 0,H
		processor->Rm.br.H=Z80free_doSetRes(processor,0,0,processor->Rm.br.H);
		return (4);
	break;
	case 133: // RES 0,L
		processor->Rm.br.L=Z80free_doSetRes(processor,0,0,processor->Rm.br.L);
		return (4);
	break;
	case 134: // RES 0,(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doSetRes(processor,0,0,Z80free_Rd_Internal(processor,processor->Rm.wr.HL)));
		return (11);
	break;
	case 135: // RES 0,A
		processor->Rm.br.A=Z80free_doSetRes(processor,0,0,processor->Rm.br.A);
		return (4);
	break;
	case 136: // RES 1,B
		processor->Rm.br.B=Z80free_doSetRes(processor,0,1,processor->Rm.br.B);
		return (4);
	break;
	case 137: // RES 1,C
		processor->Rm.br.C=Z80free_doSetRes(processor,0,1,processor->Rm.br.C);
		return (4);
	break;
	case 138: // RES 1,D
		processor->Rm.br.D=Z80free_doSetRes(processor,0,1,processor->Rm.br.D);
		return (4);
	break;
	case 139: // RES 1,E
		processor->Rm.br.E=Z80free_doSetRes(processor,0,1,processor->Rm.br.E);
		return (4);
	break;
	case 140: // RES 1,H
		processor->Rm.br.H=Z80free_doSetRes(processor,0,1,processor->Rm.br.H);
		return (4);
	break;
	case 141: // RES 1,L
		processor->Rm.br.L=Z80free_doSetRes(processor,0,1,processor->Rm.br.L);
		return (4);
	break;
	case 142: // RES 1,(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doSetRes(processor,0,1,Z80free_Rd_Internal(processor,processor->Rm.wr.HL)));
		return (11);
	break;
	case 143: // RES 1,A
		processor->Rm.br.A=Z80free_doSetRes(processor,0,1,processor->Rm.br.A);
		return (4);
	break;
	case 144: // RES 2,B
		processor->Rm.br.B=Z80free_doSetRes(processor,0,2,processor->Rm.br.B);
		return (4);
	break;
	case 145: // RES 2,C
		processor->Rm.br.C=Z80free_doSetRes(processor,0,2,processor->Rm.br.C);
		return (4);
	break;
	case 146: // RES 2,D
		processor->Rm.br.D=Z80free_doSetRes(processor,0,2,processor->Rm.br.D);
		return (4);
	break;
	case 147: // RES 2,E
		processor->Rm.br.E=Z80free_doSetRes(processor,0,2,processor->Rm.br.E);
		return (4);
	break;
	case 148: // RES 2,H
		processor->Rm.br.H=Z80free_doSetRes(processor,0,2,processor->Rm.br.H);
		return (4);
	break;
	case 149: // RES 2,L
		processor->Rm.br.L=Z80free_doSetRes(processor,0,2,processor->Rm.br.L);
		return (4);
	break;
	case 150: // RES 2,(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doSetRes(processor,0,2,Z80free_Rd_Internal(processor,processor->Rm.wr.HL)));
		return (11);
	break;
	case 151: // RES 2,A
		processor->Rm.br.A=Z80free_doSetRes(processor,0,2,processor->Rm.br.A);
		return (4);
	break;
	case 152: // RES 3,B
		processor->Rm.br.B=Z80free_doSetRes(processor,0,3,processor->Rm.br.B);
		return (4);
	break;
	case 153: // RES 3,C
		processor->Rm.br.C=Z80free_doSetRes(processor,0,3,processor->Rm.br.C);
		return (4);
	break;
	case 154: // RES 3,D
		processor->Rm.br.D=Z80free_doSetRes(processor,0,3,processor->Rm.br.D);
		return (4);
	break;
	case 155: // RES 3,E
		processor->Rm.br.E=Z80free_doSetRes(processor,0,3,processor->Rm.br.E);
		return (4);
	break;
	case 156: // RES 3,H
		processor->Rm.br.H=Z80free_doSetRes(processor,0,3,processor->Rm.br.H);
		return (4);
	break;
	case 157: // RES 3,L
		processor->Rm.br.L=Z80free_doSetRes(processor,0,3,processor->Rm.br.L);
		return (4);
	break;
	case 158: // RES 3,(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doSetRes(processor,0,3,Z80free_Rd_Internal(processor,processor->Rm.wr.HL)));
		return (11);
	break;
	case 159: // RES 3,A
		processor->Rm.br.A=Z80free_doSetRes(processor,0,3,processor->Rm.br.A);
		return (4);
	break;
	case 160: // RES 4,B
		processor->Rm.br.B=Z80free_doSetRes(processor,0,4,processor->Rm.br.B);
		return (4);
	break;
	case 161: // RES 4,C
		processor->Rm.br.C=Z80free_doSetRes(processor,0,4,processor->Rm.br.C);
		return (4);
	break;
	case 162: // RES 4,D
		processor->Rm.br.D=Z80free_doSetRes(processor,0,4,processor->Rm.br.D);
		return (4);
	break;
	case 163: // RES 4,E
		processor->Rm.br.E=Z80free_doSetRes(processor,0,4,processor->Rm.br.E);
		return (4);
	break;
	case 164: // RES 4,H
		processor->Rm.br.H=Z80free_doSetRes(processor,0,4,processor->Rm.br.H);
		return (4);
	break;
	case 165: // RES 4,L
		processor->Rm.br.L=Z80free_doSetRes(processor,0,4,processor->Rm.br.L);
		return (4);
	break;
	case 166: // RES 4,(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doSetRes(processor,0,4,Z80free_Rd_Internal(processor,processor->Rm.wr.HL)));
		return (11);
	break;
	case 167: // RES 4,A
		processor->Rm.br.A=Z80free_doSetRes(processor,0,4,processor->Rm.br.A);
		return (4);
	break;
	case 168: // RES 5,B
		processor->Rm.br.B=Z80free_doSetRes(processor,0,5,processor->Rm.br.B);
		return (4);
	break;
	case 169: // RES 5,C
		processor->Rm.br.C=Z80free_doSetRes(processor,0,5,processor->Rm.br.C);
		return (4);
	break;
	case 170: // RES 5,D
		processor->Rm.br.D=Z80free_doSetRes(processor,0,5,processor->Rm.br.D);
		return (4);
	break;
	case 171: // RES 5,E
		processor->Rm.br.E=Z80free_doSetRes(processor,0,5,processor->Rm.br.E);
		return (4);
	break;
	case 172: // RES 5,H
		processor->Rm.br.H=Z80free_doSetRes(processor,0,5,processor->Rm.br.H);
		return (4);
	break;
	case 173: // RES 5,L
		processor->Rm.br.L=Z80free_doSetRes(processor,0,5,processor->Rm.br.L);
		return (4);
	break;
	case 174: // RES 5,(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doSetRes(processor,0,5,Z80free_Rd_Internal(processor,processor->Rm.wr.HL)));
		return (11);
	break;
	case 175: // RES 5,A
		processor->Rm.br.A=Z80free_doSetRes(processor,0,5,processor->Rm.br.A);
		return (4);
	break;
	case 176: // RES 6,B
		processor->Rm.br.B=Z80free_doSetRes(processor,0,6,processor->Rm.br.B);
		return (4);
	break;
	case 177: // RES 6,C
		processor->Rm.br.C=Z80free_doSetRes(processor,0,6,processor->Rm.br.C);
		return (4);
	break;
	case 178: // RES 6,D
		processor->Rm.br.D=Z80free_doSetRes(processor,0,6,processor->Rm.br.D);
		return (4);
	break;
	case 179: // RES 6,E
		processor->Rm.br.E=Z80free_doSetRes(processor,0,6,processor->Rm.br.E);
		return (4);
	break;
	case 180: // RES 6,H
		processor->Rm.br.H=Z80free_doSetRes(processor,0,6,processor->Rm.br.H);
		return (4);
	break;
	case 181: // RES 6,L
		processor->Rm.br.L=Z80free_doSetRes(processor,0,6,processor->Rm.br.L);
		return (4);
	break;
	case 182: // RES 6,(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doSetRes(processor,0,6,Z80free_Rd_Internal(processor,processor->Rm.wr.HL)));
		return (11);
	break;
	case 183: // RES 6,A
		processor->Rm.br.A=Z80free_doSetRes(processor,0,6,processor->Rm.br.A);
		return (4);
	break;
	case 184: // RES 7,B
		processor->Rm.br.B=Z80free_doSetRes(processor,0,7,processor->Rm.br.B);
		return (4);
	break;
	case 185: // RES 7,C
		processor->Rm.br.C=Z80free_doSetRes(processor,0,7,processor->Rm.br.C);
		return (4);
	break;
	case 186: // RES 7,D
		processor->Rm.br.D=Z80free_doSetRes(processor,0,7,processor->Rm.br.D);
		return (4);
	break;
	case 187: // RES 7,E
		processor->Rm.br.E=Z80free_doSetRes(processor,0,7,processor->Rm.br.E);
		return (4);
	break;
	case 188: // RES 7,H
		processor->Rm.br.H=Z80free_doSetRes(processor,0,7,processor->Rm.br.H);
		return (4);
	break;
	case 189: // RES 7,L
		processor->Rm.br.L=Z80free_doSetRes(processor,0,7,processor->Rm.br.L);
		return (4);
	break;
	case 190: // RES 7,(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doSetRes(processor,0,7,Z80free_Rd_Internal(processor,processor->Rm.wr.HL)));
		return (11);
	break;
	case 191: // RES 7,A
		processor->Rm.br.A=Z80free_doSetRes(processor,0,7,processor->Rm.br.A);
		return (4);
	break;
	case 192: // SET 0,B
		processor->Rm.br.B=Z80free_doSetRes(processor,1,0,processor->Rm.br.B);
		return (4);
	break;
	case 193: // SET 0,C
		processor->Rm.br.C=Z80free_doSetRes(processor,1,0,processor->Rm.br.C);
		return (4);
	break;
	case 194: // SET 0,D
		processor->Rm.br.D=Z80free_doSetRes(processor,1,0,processor->Rm.br.D);
		return (4);
	break;
	case 195: // SET 0,E
		processor->Rm.br.E=Z80free_doSetRes(processor,1,0,processor->Rm.br.E);
		return (4);
	break;
	case 196: // SET 0,H
		processor->Rm.br.H=Z80free_doSetRes(processor,1,0,processor->Rm.br.H);
		return (4);
	break;
	case 197: // SET 0,L
		processor->Rm.br.L=Z80free_doSetRes(processor,1,0,processor->Rm.br.L);
		return (4);
	break;
	case 198: // SET 0,(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doSetRes(processor,1,0,Z80free_Rd_Internal(processor,processor->Rm.wr.HL)));
		return (11);
	break;
	case 199: // SET 0,A
		processor->Rm.br.A=Z80free_doSetRes(processor,1,0,processor->Rm.br.A);
		return (4);
	break;
	case 200: // SET 1,B
		processor->Rm.br.B=Z80free_doSetRes(processor,1,1,processor->Rm.br.B);
		return (4);
	break;
	case 201: // SET 1,C
		processor->Rm.br.C=Z80free_doSetRes(processor,1,1,processor->Rm.br.C);
		return (4);
	break;
	case 202: // SET 1,D
		processor->Rm.br.D=Z80free_doSetRes(processor,1,1,processor->Rm.br.D);
		return (4);
	break;
	case 203: // SET 1,E
		processor->Rm.br.E=Z80free_doSetRes(processor,1,1,processor->Rm.br.E);
		return (4);
	break;
	case 204: // SET 1,H
		processor->Rm.br.H=Z80free_doSetRes(processor,1,1,processor->Rm.br.H);
		return (4);
	break;
	case 205: // SET 1,L
		processor->Rm.br.L=Z80free_doSetRes(processor,1,1,processor->Rm.br.L);
		return (4);
	break;
	case 206: // SET 1,(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doSetRes(processor,1,1,Z80free_Rd_Internal(processor,processor->Rm.wr.HL)));
		return (11);
	break;
	case 207: // SET 1,A
		processor->Rm.br.A=Z80free_doSetRes(processor,1,1,processor->Rm.br.A);
		return (4);
	break;
	case 208: // SET 2,B
		processor->Rm.br.B=Z80free_doSetRes(processor,1,2,processor->Rm.br.B);
		return (4);
	break;
	case 209: // SET 2,C
		processor->Rm.br.C=Z80free_doSetRes(processor,1,2,processor->Rm.br.C);
		return (4);
	break;
	case 210: // SET 2,D
		processor->Rm.br.D=Z80free_doSetRes(processor,1,2,processor->Rm.br.D);
		return (4);
	break;
	case 211: // SET 2,E
		processor->Rm.br.E=Z80free_doSetRes(processor,1,2,processor->Rm.br.E);
		return (4);
	break;
	case 212: // SET 2,H
		processor->Rm.br.H=Z80free_doSetRes(processor,1,2,processor->Rm.br.H);
		return (4);
	break;
	case 213: // SET 2,L
		processor->Rm.br.L=Z80free_doSetRes(processor,1,2,processor->Rm.br.L);
		return (4);
	break;
	case 214: // SET 2,(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doSetRes(processor,1,2,Z80free_Rd_Internal(processor,processor->Rm.wr.HL)));
		return (11);
	break;
	case 215: // SET 2,A
		processor->Rm.br.A=Z80free_doSetRes(processor,1,2,processor->Rm.br.A);
		return (4);
	break;
	case 216: // SET 3,B
		processor->Rm.br.B=Z80free_doSetRes(processor,1,3,processor->Rm.br.B);
		return (4);
	break;
	case 217: // SET 3,C
		processor->Rm.br.C=Z80free_doSetRes(processor,1,3,processor->Rm.br.C);
		return (4);
	break;
	case 218: // SET 3,D
		processor->Rm.br.D=Z80free_doSetRes(processor,1,3,processor->Rm.br.D);
		return (4);
	break;
	case 219: // SET 3,E
		processor->Rm.br.E=Z80free_doSetRes(processor,1,3,processor->Rm.br.E);
		return (4);
	break;
	case 220: // SET 3,H
		processor->Rm.br.H=Z80free_doSetRes(processor,1,3,processor->Rm.br.H);
		return (4);
	break;
	case 221: // SET 3,L
		processor->Rm.br.L=Z80free_doSetRes(processor,1,3,processor->Rm.br.L);
		return (4);
	break;
	case 222: // SET 3,(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doSetRes(processor,1,3,Z80free_Rd_Internal(processor,processor->Rm.wr.HL)));
		return (11);
	break;
	case 223: // SET 3,A
		processor->Rm.br.A=Z80free_doSetRes(processor,1,3,processor->Rm.br.A);
		return (4);
	break;
	case 224: // SET 4,B
		processor->Rm.br.B=Z80free_doSetRes(processor,1,4,processor->Rm.br.B);
		return (4);
	break;
	case 225: // SET 4,C
		processor->Rm.br.C=Z80free_doSetRes(processor,1,4,processor->Rm.br.C);
		return (4);
	break;
	case 226: // SET 4,D
		processor->Rm.br.D=Z80free_doSetRes(processor,1,4,processor->Rm.br.D);
		return (4);
	break;
	case 227: // SET 4,E
		processor->Rm.br.E=Z80free_doSetRes(processor,1,4,processor->Rm.br.E);
		return (4);
	break;
	case 228: // SET 4,H
		processor->Rm.br.H=Z80free_doSetRes(processor,1,4,processor->Rm.br.H);
		return (4);
	break;
	case 229: // SET 4,L
		processor->Rm.br.L=Z80free_doSetRes(processor,1,4,processor->Rm.br.L);
		return (4);
	break;
	case 230: // SET 4,(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doSetRes(processor,1,4,Z80free_Rd_Internal(processor,processor->Rm.wr.HL)));
		return (11);
	break;
	case 231: // SET 4,A
		processor->Rm.br.A=Z80free_doSetRes(processor,1,4,processor->Rm.br.A);
		return (4);
	break;
	case 232: // SET 5,B
		processor->Rm.br.B=Z80free_doSetRes(processor,1,5,processor->Rm.br.B);
		return (4);
	break;
	case 233: // SET 5,C
		processor->Rm.br.C=Z80free_doSetRes(processor,1,5,processor->Rm.br.C);
		return (4);
	break;
	case 234: // SET 5,D
		processor->Rm.br.D=Z80free_doSetRes(processor,1,5,processor->Rm.br.D);
		return (4);
	break;
	case 235: // SET 5,E
		processor->Rm.br.E=Z80free_doSetRes(processor,1,5,processor->Rm.br.E);
		return (4);
	break;
	case 236: // SET 5,H
		processor->Rm.br.H=Z80free_doSetRes(processor,1,5,processor->Rm.br.H);
		return (4);
	break;
	case 237: // SET 5,L
		processor->Rm.br.L=Z80free_doSetRes(processor,1,5,processor->Rm.br.L);
		return (4);
	break;
	case 238: // SET 5,(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doSetRes(processor,1,5,Z80free_Rd_Internal(processor,processor->Rm.wr.HL)));
		return (11);
	break;
	case 239: // SET 5,A
		processor->Rm.br.A=Z80free_doSetRes(processor,1,5,processor->Rm.br.A);
		return (4);
	break;
	case 240: // SET 6,B
		processor->Rm.br.B=Z80free_doSetRes(processor,1,6,processor->Rm.br.B);
		return (4);
	break;
	case 241: // SET 6,C
		processor->Rm.br.C=Z80free_doSetRes(processor,1,6,processor->Rm.br.C);
		return (4);
	break;
	case 242: // SET 6,D
		processor->Rm.br.D=Z80free_doSetRes(processor,1,6,processor->Rm.br.D);
		return (4);
	break;
	case 243: // SET 6,E
		processor->Rm.br.E=Z80free_doSetRes(processor,1,6,processor->Rm.br.E);
		return (4);
	break;
	case 244: // SET 6,H
		processor->Rm.br.H=Z80free_doSetRes(processor,1,6,processor->Rm.br.H);
		return (4);
	break;
	case 245: // SET 6,L
		processor->Rm.br.L=Z80free_doSetRes(processor,1,6,processor->Rm.br.L);
		return (4);
	break;
	case 246: // SET 6,(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doSetRes(processor,1,6,Z80free_Rd_Internal(processor,processor->Rm.wr.HL)));
		return (11);
	break;
	case 247: // SET 6,A
		processor->Rm.br.A=Z80free_doSetRes(processor,1,6,processor->Rm.br.A);
		return (4);
	break;
	case 248: // SET 7,B
		processor->Rm.br.B=Z80free_doSetRes(processor,1,7,processor->Rm.br.B);
		return (4);
	break;
	case 249: // SET 7,C
		processor->Rm.br.C=Z80free_doSetRes(processor,1,7,processor->Rm.br.C);
		return (4);
	break;
	case 250: // SET 7,D
		processor->Rm.br.D=Z80free_doSetRes(processor,1,7,processor->Rm.br.D);
		return (4);
	break;
	case 251: // SET 7,E
		processor->Rm.br.E=Z80free_doSetRes(processor,1,7,processor->Rm.br.E);
		return (4);
	break;
	case 252: // SET 7,H
		processor->Rm.br.H=Z80free_doSetRes(processor,1,7,processor->Rm.br.H);
		return (4);
	break;
	case 253: // SET 7,L
		processor->Rm.br.L=Z80free_doSetRes(processor,1,7,processor->Rm.br.L);
		return (4);
	break;
	case 254: // SET 7,(HL)
		Z80free_Wr_Internal(processor,processor->Rm.wr.HL,Z80free_doSetRes(processor,1,7,Z80free_Rd_Internal(processor,processor->Rm.wr.HL)));
		return (11);
	break;
	case 255: // SET 7,A
		processor->Rm.br.A=Z80free_doSetRes(processor,1,7,processor->Rm.br.A);
		return (4);
	break;
	}

	return -1;
}
