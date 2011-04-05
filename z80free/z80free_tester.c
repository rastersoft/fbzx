/*
 * Copyright 2009 (C) Raster Software Vigo (Sergio Costas)
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
#include <stdio.h>


Z80FREE emulator,emulator2;
unsigned char memory[65536];
FILE *infile;
char test_name[20];
byte ports[256];
word portdirs[256];
int portpos=0,elements=0;

#define NO35

int compare_processors() {

	if (emulator.Rm.br.A!=emulator2.Rm.br.A) {
		printf("A should be %X but is %X\n",emulator2.Rm.br.A,emulator.Rm.br.A);
	}

#ifdef NO35
	if ((emulator.Rm.br.F|0x28)!=(emulator2.Rm.br.F|0x28)) {
		printf("F should be %X but is %X\n",emulator2.Rm.br.F,emulator.Rm.br.F);
	}
#else
	if (emulator.Rm.br.F!=emulator2.Rm.br.F) {
		printf("F should be %X but is %X\n",emulator2.Rm.br.F,emulator.Rm.br.F);
	}
#endif

	if (emulator.Rm.wr.BC!=emulator2.Rm.wr.BC) {
		printf("BC should be %X but is %X\n",emulator2.Rm.wr.BC,emulator.Rm.wr.BC);
	}
	if (emulator.Rm.wr.DE!=emulator2.Rm.wr.DE) {
		printf("DE should be %X but is %X\n",emulator2.Rm.wr.DE,emulator.Rm.wr.DE);
	}
	if (emulator.Rm.wr.HL!=emulator2.Rm.wr.HL) {
		printf("HL should be %X but is %X\n",emulator2.Rm.wr.HL,emulator.Rm.wr.HL);
	}

	if (emulator.Ra.br.A!=emulator2.Ra.br.A) {
		printf("A' should be %X but is %X\n",emulator2.Ra.br.A,emulator.Ra.br.A);
	}

#ifdef NO35
	if ((emulator.Ra.br.F|0x28)!=(emulator2.Ra.br.F|0x28)) {
		printf("F' should be %X but is %X\n",emulator2.Ra.br.F,emulator.Ra.br.F);
	}
#else
	if (emulator.Ra.br.F!=emulator2.Ra.br.F) {
		printf("F' should be %X but is %X\n",emulator2.Ra.br.F,emulator.Ra.br.F);
	}
#endif

	if (emulator.Ra.wr.BC!=emulator2.Ra.wr.BC) {
		printf("BC' should be %X but is %X\n",emulator2.Ra.wr.BC,emulator.Ra.wr.BC);
	}
	if (emulator.Ra.wr.DE!=emulator2.Ra.wr.DE) {
		printf("DE' should be %X but is %X\n",emulator2.Ra.wr.DE,emulator.Ra.wr.DE);
	}
	if (emulator.Ra.wr.HL!=emulator2.Ra.wr.HL) {
		printf("HL' should be %X but is %X\n",emulator2.Ra.wr.HL,emulator.Ra.wr.HL);
	}

	if (emulator.Rm.wr.IX!=emulator2.Rm.wr.IX) {
		printf("IX should be %X but is %X\n",emulator2.Rm.wr.IX,emulator.Rm.wr.IX);
	}
	if (emulator.Rm.wr.IY!=emulator2.Rm.wr.IY) {
		printf("IY should be %X but is %X\n",emulator2.Rm.wr.IY,emulator.Rm.wr.IY);
	}

	if (emulator.PC!=emulator2.PC) {
		printf("PC should be %X but is %X\n",emulator2.PC,emulator.PC);
	}
	if (emulator.Rm.wr.SP!=emulator2.Rm.wr.SP) {
		printf("SP should be %X but is %X\n",emulator2.Rm.wr.SP,emulator.Rm.wr.SP);
	}

	if (emulator.HALT!=emulator2.HALT) {
		printf("HALT should be %X but is %X\n",emulator2.HALT,emulator.HALT);
	}
	if (emulator.I!=emulator2.I) {
		printf("I should be %X but is %X\n",emulator2.I,emulator.I);
	}
	if ((emulator.R&0x7F)!=(emulator2.R&0x7F)) {
		printf("R should be %X but is %X\n",(emulator2.R&0x7F),(emulator.R&0x7F));
	}
	if (emulator.IM!=emulator2.IM) {
		printf("IM should be %X but is %X\n",emulator2.IM,emulator.IM);
	}
	if (emulator.IFF1!=emulator2.IFF1) {
		printf("IFF1 should be %X but is %X\n",emulator2.IFF1,emulator.IFF1);
	}
	if (emulator.IFF2!=emulator2.IFF2) {
		printf("IFF2 should be %X but is %X\n",emulator2.IFF2,emulator.IFF2);
	}
	return 0;
}

int read_elements(Z80FREE *emulador) {

	int tstates,retval,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10;

	Z80free_reset(emulador);

	retval=fscanf(infile,"%x %x %x %x",(int *)&emulador->Rm.wr.AF,(int *)&emulador->Rm.wr.BC,(int *)&emulador->Rm.wr.DE,(int *)&emulador->Rm.wr.HL);
	retval=fscanf(infile,"%x %x %x %x",&v7,&v8,&v9,&v10);
	emulador->Ra.wr.AF=v7;
	emulador->Ra.wr.BC=v8;
	emulador->Ra.wr.DE=v9;
	emulador->Ra.wr.HL=v10;
	retval=fscanf(infile,"%x %x %x %x",(int *)&emulador->Rm.wr.IX,(int *)&emulador->Rm.wr.IY,(int *)&emulador->Rm.wr.SP,(int *)&emulador->PC);
	retval=fscanf(infile,"%x %x %x %x %x %x %d",&v1,&v2,&v3,&v4,&v5,&v6,&tstates);

	emulador->I=v1;
	emulador->R=v2;
	emulador->R2=v2;
	emulador->IFF1=v3;
	emulador->IFF2=v4;
	emulador->IM=v5;
	emulador->HALT=v6;

	return (tstates);
}

int read_memory(char mode) {
	// mode=0 : store value in memory
	// mode=1 : compare value with memory

	int position,value,retval;

	retval=fscanf(infile,"%x",&position);
	if (position==-1) {
		return -1;
	}

	do {
		retval=fscanf(infile,"%x",&value);
		if (value!=-1) {
			if (mode) {
				if (memory[position]!=value) {
					printf("Error in memory, address %X. Expected value: %X, current value: %X\n",position,value,memory[position]);
				}
			} else {
				memory[position]=value;
			}
			position++;
		}
	} while(value!=-1);
	return 0;
}

int read_inports() {

	int addr,value,retval;

	portpos=0;
	elements=0;

	while (1) {
		retval=fscanf(infile,"%x",&addr);
		if (addr==-1) {
			break;
		}
		retval=fscanf(infile,"%x",&value);
		portdirs[elements]=addr;
		ports[elements]=value;
		elements++;
	}
	return 0;
}

int read_test() {

	int retval,tstates,tst2,totaltst;

	retval=fscanf(infile,"%s",test_name);
	if (retval<=0) {
		return -1;
	}
	printf("Test %s\n",test_name);
	tstates=read_elements(&emulator);
	while(0==read_memory(0)) {
	}
	read_inports();
	totaltst=0;
	while(totaltst<tstates) {
		totaltst+=Z80free_step(&emulator);
	}
	tst2=read_elements(&emulator2);
	compare_processors();
	if (totaltst!=tst2) {
		printf("Test needed %d tstates, but should needed %d\n",totaltst,tst2);
	}
	while(0==read_memory(1)) {
	}
	return 0;
}

byte Z80free_Rd (register word Addr) {

	return ((byte)memory[Addr]);

}
void Z80free_Wr (register word Addr, register byte Value) {

	memory[Addr]=(unsigned char) Value;

}
byte Z80free_In (register word Port) {

	byte value;

	if (portpos>=elements) {
		printf("IN beyond values\n");
		return 0;
	}

	if (Port!=portdirs[portpos])
		printf("IN to port %X (should be %X)\n",Port,portdirs[portpos]);
	value=ports[portpos];
	portpos++;
	return (value);

}
void Z80free_Out (register word Port, register byte Value) {

}

int main() {

	infile=fopen("tests.z80free","r");
	while(0==read_test()) {
	}

	return 0;
}


