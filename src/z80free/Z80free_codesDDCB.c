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

int Z80free_codesDDCB (Z80FREE *processor,byte d1) {
	static byte tmp1;
	static byte opcode;
	opcode=Z80free_Rd(processor->PC++);

	switch(opcode) {
	case 0: // LD_RLC B,(IX+d)
		tmp1=Z80free_doRLC(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.B=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 1: // LD_RLC C,(IX+d)
		tmp1=Z80free_doRLC(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.C=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 2: // LD_RLC D,(IX+d)
		tmp1=Z80free_doRLC(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.D=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 3: // LD_RLC E,(IX+d)
		tmp1=Z80free_doRLC(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.E=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 4: // LD_RLC H,(IX+d)
		tmp1=Z80free_doRLC(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.H=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 5: // LD_RLC L,(IX+d)
		tmp1=Z80free_doRLC(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.L=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 6: // RLC b(IX+d)
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),Z80free_doRLC(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))));
		return (19);
	break;
	case 7: // LD_RLC A,(IX+d)
		tmp1=Z80free_doRLC(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.A=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 8: // LD_RRC B,(IX+d)
		tmp1=Z80free_doRRC(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.B=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 9: // LD_RRC C,(IX+d)
		tmp1=Z80free_doRRC(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.C=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 10: // LD_RRC D,(IX+d)
		tmp1=Z80free_doRRC(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.D=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 11: // LD_RRC E,(IX+d)
		tmp1=Z80free_doRRC(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.E=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 12: // LD_RRC H,(IX+d)
		tmp1=Z80free_doRRC(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.H=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 13: // LD_RRC L,(IX+d)
		tmp1=Z80free_doRRC(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.L=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 14: // RRC b(IX+d)
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),Z80free_doRRC(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))));
		return (19);
	break;
	case 15: // LD_RRC A,(IX+d)
		tmp1=Z80free_doRRC(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.A=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 16: // LD_RL B,(IX+d)
		tmp1=Z80free_doRL(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.B=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 17: // LD_RL C,(IX+d)
		tmp1=Z80free_doRL(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.C=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 18: // LD_RL D,(IX+d)
		tmp1=Z80free_doRL(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.D=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 19: // LD_RL E,(IX+d)
		tmp1=Z80free_doRL(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.E=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 20: // LD_RL H,(IX+d)
		tmp1=Z80free_doRL(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.H=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 21: // LD_RL L,(IX+d)
		tmp1=Z80free_doRL(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.L=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 22: // RL b(IX+d)
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),Z80free_doRL(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))));
		return (19);
	break;
	case 23: // LD_RL A,(IX+d)
		tmp1=Z80free_doRL(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.A=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 24: // LD_RR B,(IX+d)
		tmp1=Z80free_doRR(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.B=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 25: // LD_RR C,(IX+d)
		tmp1=Z80free_doRR(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.C=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 26: // LD_RR D,(IX+d)
		tmp1=Z80free_doRR(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.D=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 27: // LD_RR E,(IX+d)
		tmp1=Z80free_doRR(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.E=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 28: // LD_RR H,(IX+d)
		tmp1=Z80free_doRR(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.H=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 29: // LD_RR L,(IX+d)
		tmp1=Z80free_doRR(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.L=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 30: // RR b(IX+d)
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),Z80free_doRR(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))));
		return (19);
	break;
	case 31: // LD_RR A,(IX+d)
		tmp1=Z80free_doRR(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.A=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 32: // LD_SLA B,(IX+d)
		tmp1=Z80free_doSL(processor,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.B=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 33: // LD_SLA C,(IX+d)
		tmp1=Z80free_doSL(processor,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.C=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 34: // LD_SLA D,(IX+d)
		tmp1=Z80free_doSL(processor,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.D=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 35: // LD_SLA E,(IX+d)
		tmp1=Z80free_doSL(processor,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.E=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 36: // LD_SLA H,(IX+d)
		tmp1=Z80free_doSL(processor,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.H=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 37: // LD_SLA L,(IX+d)
		tmp1=Z80free_doSL(processor,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.L=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 38: // SLA b(IX+d)
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),Z80free_doSL(processor,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))));
		return (19);
	break;
	case 39: // LD_SLA A,(IX+d)
		tmp1=Z80free_doSL(processor,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.A=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 40: // LD_SRA B,(IX+d)
		tmp1=Z80free_doSR(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.B=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 41: // LD_SRA C,(IX+d)
		tmp1=Z80free_doSR(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.C=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 42: // LD_SRA D,(IX+d)
		tmp1=Z80free_doSR(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.D=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 43: // LD_SRA E,(IX+d)
		tmp1=Z80free_doSR(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.E=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 44: // LD_SRA H,(IX+d)
		tmp1=Z80free_doSR(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.H=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 45: // LD_SRA L,(IX+d)
		tmp1=Z80free_doSR(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.L=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 46: // SRA b(IX+d)
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),Z80free_doSR(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))));
		return (19);
	break;
	case 47: // LD_SRA A,(IX+d)
		tmp1=Z80free_doSR(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.A=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 48: // LD_SLL B,(IX+d)
		tmp1=Z80free_doSL(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.B=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 49: // LD_SLL C,(IX+d)
		tmp1=Z80free_doSL(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.C=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 50: // LD_SLL D,(IX+d)
		tmp1=Z80free_doSL(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.D=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 51: // LD_SLL E,(IX+d)
		tmp1=Z80free_doSL(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.E=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 52: // LD_SLL H,(IX+d)
		tmp1=Z80free_doSL(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.H=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 53: // LD_SLL L,(IX+d)
		tmp1=Z80free_doSL(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.L=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 54: // SLL b(IX+d)
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),Z80free_doSL(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))));
		return (19);
	break;
	case 55: // LD_SLL A,(IX+d)
		tmp1=Z80free_doSL(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.A=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 56: // LD_SRL B,(IX+d)
		tmp1=Z80free_doSR(processor,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.B=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 57: // LD_SRL C,(IX+d)
		tmp1=Z80free_doSR(processor,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.C=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 58: // LD_SRL D,(IX+d)
		tmp1=Z80free_doSR(processor,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.D=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 59: // LD_SRL E,(IX+d)
		tmp1=Z80free_doSR(processor,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.E=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 60: // LD_SRL H,(IX+d)
		tmp1=Z80free_doSR(processor,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.H=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 61: // LD_SRL L,(IX+d)
		tmp1=Z80free_doSR(processor,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.L=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 62: // SRL b(IX+d)
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),Z80free_doSR(processor,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))));
		return (19);
	break;
	case 63: // LD_SRL A,(IX+d)
		tmp1=Z80free_doSR(processor,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		processor->Rm.br.A=tmp1;
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		return (19);
	break;
	case 64: // BIT 0,(IX+d)
		Z80free_doBIT(processor,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 65: // BIT 0,(IX+d)
		Z80free_doBIT(processor,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 66: // BIT 0,(IX+d)
		Z80free_doBIT(processor,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 67: // BIT 0,(IX+d)
		Z80free_doBIT(processor,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 68: // BIT 0,(IX+d)
		Z80free_doBIT(processor,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 69: // BIT 0,(IX+d)
		Z80free_doBIT(processor,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 70: // BIT 0,(IX+d)
		Z80free_doBIT(processor,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 71: // BIT 0,(IX+d)
		Z80free_doBIT(processor,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 72: // BIT 1,(IX+d)
		Z80free_doBIT(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 73: // BIT 1,(IX+d)
		Z80free_doBIT(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 74: // BIT 1,(IX+d)
		Z80free_doBIT(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 75: // BIT 1,(IX+d)
		Z80free_doBIT(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 76: // BIT 1,(IX+d)
		Z80free_doBIT(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 77: // BIT 1,(IX+d)
		Z80free_doBIT(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 78: // BIT 1,(IX+d)
		Z80free_doBIT(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 79: // BIT 1,(IX+d)
		Z80free_doBIT(processor,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 80: // BIT 2,(IX+d)
		Z80free_doBIT(processor,2,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 81: // BIT 2,(IX+d)
		Z80free_doBIT(processor,2,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 82: // BIT 2,(IX+d)
		Z80free_doBIT(processor,2,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 83: // BIT 2,(IX+d)
		Z80free_doBIT(processor,2,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 84: // BIT 2,(IX+d)
		Z80free_doBIT(processor,2,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 85: // BIT 2,(IX+d)
		Z80free_doBIT(processor,2,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 86: // BIT 2,(IX+d)
		Z80free_doBIT(processor,2,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 87: // BIT 2,(IX+d)
		Z80free_doBIT(processor,2,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 88: // BIT 3,(IX+d)
		Z80free_doBIT(processor,3,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 89: // BIT 3,(IX+d)
		Z80free_doBIT(processor,3,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 90: // BIT 3,(IX+d)
		Z80free_doBIT(processor,3,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 91: // BIT 3,(IX+d)
		Z80free_doBIT(processor,3,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 92: // BIT 3,(IX+d)
		Z80free_doBIT(processor,3,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 93: // BIT 3,(IX+d)
		Z80free_doBIT(processor,3,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 94: // BIT 3,(IX+d)
		Z80free_doBIT(processor,3,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 95: // BIT 3,(IX+d)
		Z80free_doBIT(processor,3,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 96: // BIT 4,(IX+d)
		Z80free_doBIT(processor,4,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 97: // BIT 4,(IX+d)
		Z80free_doBIT(processor,4,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 98: // BIT 4,(IX+d)
		Z80free_doBIT(processor,4,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 99: // BIT 4,(IX+d)
		Z80free_doBIT(processor,4,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 100: // BIT 4,(IX+d)
		Z80free_doBIT(processor,4,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 101: // BIT 4,(IX+d)
		Z80free_doBIT(processor,4,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 102: // BIT 4,(IX+d)
		Z80free_doBIT(processor,4,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 103: // BIT 4,(IX+d)
		Z80free_doBIT(processor,4,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 104: // BIT 5,(IX+d)
		Z80free_doBIT(processor,5,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 105: // BIT 5,(IX+d)
		Z80free_doBIT(processor,5,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 106: // BIT 5,(IX+d)
		Z80free_doBIT(processor,5,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 107: // BIT 5,(IX+d)
		Z80free_doBIT(processor,5,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 108: // BIT 5,(IX+d)
		Z80free_doBIT(processor,5,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 109: // BIT 5,(IX+d)
		Z80free_doBIT(processor,5,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 110: // BIT 5,(IX+d)
		Z80free_doBIT(processor,5,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 111: // BIT 5,(IX+d)
		Z80free_doBIT(processor,5,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 112: // BIT 6,(IX+d)
		Z80free_doBIT(processor,6,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 113: // BIT 6,(IX+d)
		Z80free_doBIT(processor,6,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 114: // BIT 6,(IX+d)
		Z80free_doBIT(processor,6,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 115: // BIT 6,(IX+d)
		Z80free_doBIT(processor,6,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 116: // BIT 6,(IX+d)
		Z80free_doBIT(processor,6,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 117: // BIT 6,(IX+d)
		Z80free_doBIT(processor,6,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 118: // BIT 6,(IX+d)
		Z80free_doBIT(processor,6,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 119: // BIT 6,(IX+d)
		Z80free_doBIT(processor,6,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 120: // BIT 7,(IX+d)
		Z80free_doBIT(processor,7,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 121: // BIT 7,(IX+d)
		Z80free_doBIT(processor,7,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 122: // BIT 7,(IX+d)
		Z80free_doBIT(processor,7,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 123: // BIT 7,(IX+d)
		Z80free_doBIT(processor,7,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 124: // BIT 7,(IX+d)
		Z80free_doBIT(processor,7,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 125: // BIT 7,(IX+d)
		Z80free_doBIT(processor,7,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 126: // BIT 7,(IX+d)
		Z80free_doBIT(processor,7,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 127: // BIT 7,(IX+d)
		Z80free_doBIT(processor,7,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		return (16);
	break;
	case 128: // LD_RES B,0,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.B=tmp1;
		return (19);
	break;
	case 129: // LD_RES C,0,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.C=tmp1;
		return (19);
	break;
	case 130: // LD_RES D,0,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.D=tmp1;
		return (19);
	break;
	case 131: // LD_RES E,0,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.E=tmp1;
		return (19);
	break;
	case 132: // LD_RES H,0,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.H=tmp1;
		return (19);
	break;
	case 133: // LD_RES L,0,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.L=tmp1;
		return (19);
	break;
	case 134: // RES 0,(IX+d)
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),Z80free_doSetRes(processor,0,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))));
		return (19);
	break;
	case 135: // LD_RES A,0,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.A=tmp1;
		return (19);
	break;
	case 136: // LD_RES B,1,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.B=tmp1;
		return (19);
	break;
	case 137: // LD_RES C,1,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.C=tmp1;
		return (19);
	break;
	case 138: // LD_RES D,1,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.D=tmp1;
		return (19);
	break;
	case 139: // LD_RES E,1,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.E=tmp1;
		return (19);
	break;
	case 140: // LD_RES H,1,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.H=tmp1;
		return (19);
	break;
	case 141: // LD_RES L,1,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.L=tmp1;
		return (19);
	break;
	case 142: // RES 1,(IX+d)
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),Z80free_doSetRes(processor,0,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))));
		return (19);
	break;
	case 143: // LD_RES A,1,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.A=tmp1;
		return (19);
	break;
	case 144: // LD_RES B,2,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,2,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.B=tmp1;
		return (19);
	break;
	case 145: // LD_RES C,2,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,2,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.C=tmp1;
		return (19);
	break;
	case 146: // LD_RES D,2,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,2,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.D=tmp1;
		return (19);
	break;
	case 147: // LD_RES E,2,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,2,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.E=tmp1;
		return (19);
	break;
	case 148: // LD_RES H,2,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,2,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.H=tmp1;
		return (19);
	break;
	case 149: // LD_RES L,2,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,2,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.L=tmp1;
		return (19);
	break;
	case 150: // RES 2,(IX+d)
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),Z80free_doSetRes(processor,0,2,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))));
		return (19);
	break;
	case 151: // LD_RES A,2,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,2,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.A=tmp1;
		return (19);
	break;
	case 152: // LD_RES B,3,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,3,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.B=tmp1;
		return (19);
	break;
	case 153: // LD_RES C,3,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,3,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.C=tmp1;
		return (19);
	break;
	case 154: // LD_RES D,3,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,3,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.D=tmp1;
		return (19);
	break;
	case 155: // LD_RES E,3,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,3,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.E=tmp1;
		return (19);
	break;
	case 156: // LD_RES H,3,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,3,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.H=tmp1;
		return (19);
	break;
	case 157: // LD_RES L,3,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,3,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.L=tmp1;
		return (19);
	break;
	case 158: // RES 3,(IX+d)
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),Z80free_doSetRes(processor,0,3,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))));
		return (19);
	break;
	case 159: // LD_RES A,3,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,3,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.A=tmp1;
		return (19);
	break;
	case 160: // LD_RES B,4,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,4,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.B=tmp1;
		return (19);
	break;
	case 161: // LD_RES C,4,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,4,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.C=tmp1;
		return (19);
	break;
	case 162: // LD_RES D,4,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,4,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.D=tmp1;
		return (19);
	break;
	case 163: // LD_RES E,4,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,4,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.E=tmp1;
		return (19);
	break;
	case 164: // LD_RES H,4,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,4,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.H=tmp1;
		return (19);
	break;
	case 165: // LD_RES L,4,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,4,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.L=tmp1;
		return (19);
	break;
	case 166: // RES 4,(IX+d)
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),Z80free_doSetRes(processor,0,4,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))));
		return (19);
	break;
	case 167: // LD_RES A,4,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,4,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.A=tmp1;
		return (19);
	break;
	case 168: // LD_RES B,5,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,5,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.B=tmp1;
		return (19);
	break;
	case 169: // LD_RES C,5,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,5,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.C=tmp1;
		return (19);
	break;
	case 170: // LD_RES D,5,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,5,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.D=tmp1;
		return (19);
	break;
	case 171: // LD_RES E,5,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,5,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.E=tmp1;
		return (19);
	break;
	case 172: // LD_RES H,5,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,5,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.H=tmp1;
		return (19);
	break;
	case 173: // LD_RES L,5,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,5,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.L=tmp1;
		return (19);
	break;
	case 174: // RES 5,(IX+d)
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),Z80free_doSetRes(processor,0,5,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))));
		return (19);
	break;
	case 175: // LD_RES A,5,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,5,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.A=tmp1;
		return (19);
	break;
	case 176: // LD_RES B,6,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,6,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.B=tmp1;
		return (19);
	break;
	case 177: // LD_RES C,6,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,6,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.C=tmp1;
		return (19);
	break;
	case 178: // LD_RES D,6,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,6,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.D=tmp1;
		return (19);
	break;
	case 179: // LD_RES E,6,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,6,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.E=tmp1;
		return (19);
	break;
	case 180: // LD_RES H,6,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,6,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.H=tmp1;
		return (19);
	break;
	case 181: // LD_RES L,6,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,6,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.L=tmp1;
		return (19);
	break;
	case 182: // RES 6,(IX+d)
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),Z80free_doSetRes(processor,0,6,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))));
		return (19);
	break;
	case 183: // LD_RES A,6,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,6,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.A=tmp1;
		return (19);
	break;
	case 184: // LD_RES B,7,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,7,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.B=tmp1;
		return (19);
	break;
	case 185: // LD_RES C,7,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,7,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.C=tmp1;
		return (19);
	break;
	case 186: // LD_RES D,7,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,7,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.D=tmp1;
		return (19);
	break;
	case 187: // LD_RES E,7,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,7,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.E=tmp1;
		return (19);
	break;
	case 188: // LD_RES H,7,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,7,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.H=tmp1;
		return (19);
	break;
	case 189: // LD_RES L,7,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,7,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.L=tmp1;
		return (19);
	break;
	case 190: // RES 7,(IX+d)
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),Z80free_doSetRes(processor,0,7,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))));
		return (19);
	break;
	case 191: // LD_RES A,7,(IX+d)
		tmp1=Z80free_doSetRes(processor,0,7,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.A=tmp1;
		return (19);
	break;
	case 192: // LD_SET B,0,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.B=tmp1;
		return (19);
	break;
	case 193: // LD_SET C,0,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.C=tmp1;
		return (19);
	break;
	case 194: // LD_SET D,0,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.D=tmp1;
		return (19);
	break;
	case 195: // LD_SET E,0,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.E=tmp1;
		return (19);
	break;
	case 196: // LD_SET H,0,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.H=tmp1;
		return (19);
	break;
	case 197: // LD_SET L,0,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.L=tmp1;
		return (19);
	break;
	case 198: // SET 0,(IX+d)
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),Z80free_doSetRes(processor,1,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))));
		return (19);
	break;
	case 199: // LD_SET A,0,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,0,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.A=tmp1;
		return (19);
	break;
	case 200: // LD_SET B,1,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.B=tmp1;
		return (19);
	break;
	case 201: // LD_SET C,1,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.C=tmp1;
		return (19);
	break;
	case 202: // LD_SET D,1,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.D=tmp1;
		return (19);
	break;
	case 203: // LD_SET E,1,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.E=tmp1;
		return (19);
	break;
	case 204: // LD_SET H,1,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.H=tmp1;
		return (19);
	break;
	case 205: // LD_SET L,1,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.L=tmp1;
		return (19);
	break;
	case 206: // SET 1,(IX+d)
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),Z80free_doSetRes(processor,1,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))));
		return (19);
	break;
	case 207: // LD_SET A,1,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,1,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.A=tmp1;
		return (19);
	break;
	case 208: // LD_SET B,2,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,2,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.B=tmp1;
		return (19);
	break;
	case 209: // LD_SET C,2,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,2,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.C=tmp1;
		return (19);
	break;
	case 210: // LD_SET D,2,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,2,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.D=tmp1;
		return (19);
	break;
	case 211: // LD_SET E,2,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,2,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.E=tmp1;
		return (19);
	break;
	case 212: // LD_SET H,2,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,2,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.H=tmp1;
		return (19);
	break;
	case 213: // LD_SET L,2,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,2,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.L=tmp1;
		return (19);
	break;
	case 214: // SET 2,(IX+d)
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),Z80free_doSetRes(processor,1,2,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))));
		return (19);
	break;
	case 215: // LD_SET A,2,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,2,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.A=tmp1;
		return (19);
	break;
	case 216: // LD_SET B,3,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,3,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.B=tmp1;
		return (19);
	break;
	case 217: // LD_SET C,3,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,3,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.C=tmp1;
		return (19);
	break;
	case 218: // LD_SET D,3,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,3,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.D=tmp1;
		return (19);
	break;
	case 219: // LD_SET E,3,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,3,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.E=tmp1;
		return (19);
	break;
	case 220: // LD_SET H,3,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,3,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.H=tmp1;
		return (19);
	break;
	case 221: // LD_SET L,3,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,3,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.L=tmp1;
		return (19);
	break;
	case 222: // SET 3,(IX+d)
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),Z80free_doSetRes(processor,1,3,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))));
		return (19);
	break;
	case 223: // LD_SET A,3,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,3,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.A=tmp1;
		return (19);
	break;
	case 224: // LD_SET B,4,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,4,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.B=tmp1;
		return (19);
	break;
	case 225: // LD_SET C,4,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,4,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.C=tmp1;
		return (19);
	break;
	case 226: // LD_SET D,4,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,4,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.D=tmp1;
		return (19);
	break;
	case 227: // LD_SET E,4,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,4,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.E=tmp1;
		return (19);
	break;
	case 228: // LD_SET H,4,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,4,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.H=tmp1;
		return (19);
	break;
	case 229: // LD_SET L,4,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,4,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.L=tmp1;
		return (19);
	break;
	case 230: // SET 4,(IX+d)
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),Z80free_doSetRes(processor,1,4,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))));
		return (19);
	break;
	case 231: // LD_SET A,4,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,4,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.A=tmp1;
		return (19);
	break;
	case 232: // LD_SET B,5,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,5,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.B=tmp1;
		return (19);
	break;
	case 233: // LD_SET C,5,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,5,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.C=tmp1;
		return (19);
	break;
	case 234: // LD_SET D,5,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,5,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.D=tmp1;
		return (19);
	break;
	case 235: // LD_SET E,5,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,5,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.E=tmp1;
		return (19);
	break;
	case 236: // LD_SET H,5,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,5,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.H=tmp1;
		return (19);
	break;
	case 237: // LD_SET L,5,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,5,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.L=tmp1;
		return (19);
	break;
	case 238: // SET 5,(IX+d)
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),Z80free_doSetRes(processor,1,5,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))));
		return (19);
	break;
	case 239: // LD_SET A,5,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,5,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.A=tmp1;
		return (19);
	break;
	case 240: // LD_SET B,6,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,6,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.B=tmp1;
		return (19);
	break;
	case 241: // LD_SET C,6,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,6,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.C=tmp1;
		return (19);
	break;
	case 242: // LD_SET D,6,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,6,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.D=tmp1;
		return (19);
	break;
	case 243: // LD_SET E,6,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,6,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.E=tmp1;
		return (19);
	break;
	case 244: // LD_SET H,6,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,6,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.H=tmp1;
		return (19);
	break;
	case 245: // LD_SET L,6,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,6,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.L=tmp1;
		return (19);
	break;
	case 246: // SET 6,(IX+d)
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),Z80free_doSetRes(processor,1,6,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))));
		return (19);
	break;
	case 247: // LD_SET A,6,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,6,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.A=tmp1;
		return (19);
	break;
	case 248: // LD_SET B,7,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,7,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.B=tmp1;
		return (19);
	break;
	case 249: // LD_SET C,7,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,7,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.C=tmp1;
		return (19);
	break;
	case 250: // LD_SET D,7,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,7,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.D=tmp1;
		return (19);
	break;
	case 251: // LD_SET E,7,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,7,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.E=tmp1;
		return (19);
	break;
	case 252: // LD_SET H,7,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,7,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.H=tmp1;
		return (19);
	break;
	case 253: // LD_SET L,7,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,7,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.L=tmp1;
		return (19);
	break;
	case 254: // SET 7,(IX+d)
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),Z80free_doSetRes(processor,1,7,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1))));
		return (19);
	break;
	case 255: // LD_SET A,7,(IX+d)
		tmp1=Z80free_doSetRes(processor,1,7,Z80free_Rd(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1)));
		Z80free_Wr(Z80free_addr_relativeXDCB(processor,processor->Rm.wr.IX,d1),tmp1);
		processor->Rm.br.A=tmp1;
		return (19);
	break;
	}

	return -1;
}
