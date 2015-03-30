/*
 * Copyright 2003-2009 (C) Raster Software Vigo (Sergio Costas)
 * This file is part of FBZX
 *
 * FBZX is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * FBZX is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "cargador.hh"

#include "z80free/Z80free.h"
#include <stdio.h>
#include <string.h>
#include "computer.hh"
#include "emulator.hh"
#include "keyboard.hh"

void uncompress_z80(FILE *fichero,int length,unsigned char *memo) {

	unsigned char byte_loaded,EDfound,counter;
	int position;

	counter=0;
	byte_loaded=0;
	EDfound=0;
	position=0;
  
	printf("Descomprimo de longitud %d\n",length);
	
	do {
		if(counter) {
			memo[position++]=byte_loaded;
			counter--;
			continue;
		} else
			fread(&byte_loaded,1,1,fichero);
    
		if(EDfound==2) { // we have two EDs
			counter=byte_loaded;
			fread(&byte_loaded,1,1,fichero);
			EDfound=0;
			continue;
		}
    
		if(byte_loaded==0xED) {
			EDfound++;	
		} else {
			if(EDfound==1) { // we found ED xx. We write ED and xx
				memo[position++]=0xED;
				EDfound=0;
			}
			if (position>=length) {
				break;
			}
			memo[position++]=byte_loaded;
		}
	} while(position<length);

}

int save_z80(char *filename) {

  FILE *fichero;
  unsigned char value,bucle;

  fichero=fopen(filename,"r");
  if(fichero!=NULL) {
    fclose(fichero);
    return -1; // file already exists
  }

  fichero=fopen(filename,"w");

  if(fichero==NULL)
    return -2; // can't create file

  fprintf(fichero,"%c%c%c%c%c%c",procesador.Rm.br.A,procesador.Rm.br.F,procesador.Rm.br.C,procesador.Rm.br.B,procesador.Rm.br.L,procesador.Rm.br.H); // AF, BC and HL

  if(ordenador->mode128k==0) // 48K
    fprintf(fichero,"%c%c",(byte)(procesador.PC&0x0FF),(byte)((procesador.PC>>8)&0xFF)); // PC
  else
    fprintf(fichero,"%c%c",0,0); // 128K

  fprintf(fichero,"%c%c",procesador.Rm.br.P,procesador.Rm.br.S); // SP
  fprintf(fichero,"%c%c%c",procesador.I,procesador.R,(((procesador.R2>>7)&0x01)|((screen->border<<1)&0x0E))); // I, R and border color

  fprintf(fichero,"%c%c%c%c%c%c%c%c%c%c%c%c%c%c",procesador.Rm.br.E,procesador.Rm.br.D,procesador.Ra.br.C,procesador.Ra.br.B,procesador.Ra.br.E,procesador.Ra.br.D,procesador.Ra.br.L,procesador.Ra.br.H,procesador.Ra.br.A,procesador.Ra.br.F,procesador.Rm.br.IYl,procesador.Rm.br.IYh,procesador.Rm.br.IXl,procesador.Rm.br.IXh);

  if (procesador.IFF1)
    value=1;
  else
    value=0;
  fprintf(fichero,"%c",value);
  if (procesador.IFF2)
    value=1;
  else
    value=0;
  fprintf(fichero,"%c",value);
  value=procesador.IM;
  if(ordenador->issue==2)
    value|=4;
  switch (keyboard->joystick) {
  case 1:
  	value|=64;
  	break;
  case 2:
  	value|=128;
  	break;
  case 3:
  	value|=192;
  	break;
  }
  fprintf(fichero,"%c",value);

  if(ordenador->mode128k==0) { // 48K
    fwrite((ordenador->memoria+147456),16384,1,fichero); // video memory
    fwrite((ordenador->memoria+98304),32768,1,fichero); // memory pages 2 & 3
    fclose(fichero);
    return 0;
  }

  // 128K

  fprintf(fichero,"%c%c",23,0); // Z80 file v2.01
  fprintf(fichero,"%c%c",(byte)(procesador.PC&0x0FF),(byte)((procesador.PC>>8)&0x0FF)); // PC
  fprintf(fichero,"%c",3); // hardware mode=3
  fprintf(fichero,"%c",ordenador->mport1); // content of 0x7FFD latch
  fprintf(fichero,"%c%c",0,0); // no If1, no emulation of any kind
  fprintf(fichero,"%c",ordenador->ay_latch); // last selected AY register
  for(bucle=0;bucle<16;bucle++)
    fprintf(fichero,"%c",ordenador->ay_registers[bucle]); // AY registers
  for(bucle=0;bucle<8;bucle++) {
    fprintf(fichero,"%c%c",0xFF,0xFF); // length=0xFFFF (uncompressed)
    fprintf(fichero,"%c",bucle+3); // page number
    fwrite(ordenador->memoria+(16384*bucle)+65536,16384,1,fichero); // store page
  }
  fclose(fichero);
  return 0;
}

int load_z80(const char *filename) {

	struct z80snapshot *snap;
	unsigned char tempo[30],tempo2[56],type,compressed,page,byte_read[3];
	unsigned char *memo;
	FILE *fichero;
	int longitud=0,longitud2,bucle;

	memo=(unsigned char *)malloc(49152);
	snap=(struct z80snapshot *)malloc(sizeof(struct z80snapshot));

	longitud=strlen(filename);
	if ((longitud>4)&&(0==strcasecmp(".sna",filename+(longitud-4)))) {
		printf("Read SNA file\n");
		free(memo);
		free(snap);
		return load_sna(filename);
	}

	printf("Read Z80 file\n");

	for(bucle=0;bucle<16;bucle++)
		snap->ay_regs[bucle]=0;

	fichero=fopen(filename,"r");
	if(fichero==NULL) {
		free(memo);
		free(snap);
		return -1; // error
	}

	printf("Read header (first 30 bytes)\n");
	fread(tempo,1,30,fichero);

	if((tempo[6]==0)&&(tempo[7]==0)) { // extended Z80
		printf("It's an extended Z80 file\n");
		type=1; // new type

		fread(tempo2,1,2,fichero); // read the length of the extension
 
		longitud=((int)tempo2[0])+256*((int)tempo2[1]);
		if(longitud>54) {
			fclose(fichero);
			printf("Not suported Z80 file\n");
			free(memo);
			free(snap);
			return -3; // not a supported Z80 file
		}
		printf("Length: %d\n",longitud);
		fread(tempo2+2,1,longitud,fichero);

		if(longitud==23) // z80 ver 2.01
			switch(tempo2[4]) {
			case 0:
			case 1:
				snap->type=0; // 48K
				break;
			case 3:
			case 4:
				snap->type=1; // 128K
			break;
			default:
				fclose(fichero);
				printf("Again not suported Z80 file\n");
				free(memo);
				free(snap);
				return -3; // not a supported Z80 file
			break;
			}
		else // z80 ver 3.0x
			switch(tempo2[4]) {
			case 0:
			case 1:
			case 3:
				snap->type=0; // 48K
				break;
			case 4:
			case 5:
			case 6:
				snap->type=1; // 128K
				break;
			default:
				fclose(fichero);
				free(memo);
				free(snap);
				return -3; // not a supported Z80 file
				break;
			}      
	} else {
		printf("Old type z80\n");
		type=0; // old type
		snap->type=0; // 48k
	}

	if(tempo[29]&0x04) {
		printf("Issue 2\n");
		snap->issue=2; // issue2
	} else {
		printf("Issue 3\n");
		snap->issue=3; // issue3
	}

	snap->A=tempo[0];
	snap->F=tempo[1];
	snap->C=tempo[2];
	snap->B=tempo[3];
	snap->L=tempo[4];
	snap->H=tempo[5];
	if(type) {
		snap->PC=((word)tempo2[2])+256*((word)tempo2[3]);
		for(bucle=0;bucle<16;bucle++)
			snap->ay_regs[bucle]=tempo2[9+bucle];
		snap->ay_latch=tempo2[8];
	} else {
		snap->PC=((word)tempo[6])+256*((word)tempo[7]);
	}

	snap->SP=((word)tempo[8])+256*((word)tempo[9]);
	snap->I=tempo[10];
	snap->R=(tempo[11]&0x7F);

	if(tempo[12]==255) {
		printf("Byte 12 is 255! doing 1\n");
		tempo[12]=1;
	}

	if(tempo[12]&0x01)
		snap->R|=0x80;

	snap->border=(tempo[12]>>1)&0x07;

	if(tempo[12]&32)
		compressed=1;
	else
		compressed=0;

	snap->E=tempo[13];
	snap->D=tempo[14];
	snap->CC=tempo[15];
	snap->BB=tempo[16];
	snap->EE=tempo[17];
	snap->DD=tempo[18];
	snap->LL=tempo[19];
	snap->HH=tempo[20];
	snap->AA=tempo[21];
	snap->FF=tempo[22];
	snap->IY=((word)tempo[23])+256*((word)tempo[24]);
	snap->IX=((word)tempo[25])+256*((word)tempo[26]);

	if(tempo[27]!=0)
		snap->IFF1=1;
	else
		snap->IFF1=0;

	if(tempo[28]!=0)
		snap->IFF2=1;
	else
		snap->IFF2=0;
	
	switch(tempo[29]&0x03) {
	case 0:
		snap->Imode=0;
		break;
	case 1:
		snap->Imode=1;
		break;
	case 2:
		snap->Imode=2;
		break;
	}

	snap->joystick=((tempo[29]>>6)&0x03);

	if(type)
		snap->pager=tempo2[5];

	if(type) { // extended z80
		if(snap->type==1) { // 128K snapshot

		/*       fclose(fichero);
		return -3;*/  // z80 file not yet supported

			while(!feof(fichero)) {
				fread(byte_read,3,1,fichero);
				if(feof(fichero))
					break;
				longitud2=((int)byte_read[0])+256*((int)byte_read[1]);
				switch(byte_read[2]) {
				case 3:
					page=0;
					break;
				case 4:
					page=1;
					break;
				case 5:
					page=2;
					break;
				case 6:
					page=3;
					break;
				case 7:
					page=4;
					break;
				case 8:
					page=5;
					break;
				case 9:
					page=6;
					break;
				case 10:
					page=7;
					break;
				default:
					page=11;
					break;
				}
				printf("Loading page %d of length %d\n",page,longitud);
				if(longitud2==0xFFFF) // uncompressed raw data
					fread(snap->page[page],16384,1,fichero);
				else
					uncompress_z80(fichero,16384,snap->page[page]);
			}

		} else {
			while(!feof(fichero)) {
				fread(byte_read,3,1,fichero);
				if(feof(fichero))
					break;
				longitud2=((int)byte_read[0])+256*((int)byte_read[1]);
				switch(byte_read[2]) {
				case 8:
					page=0;
					break;
				case 4:
					page=1;
					break;
				case 5:
					page=2;
					break;
				default:
					page=11;
					break;
				}
				if(longitud2==0xFFFF) // uncompressed raw data
					fread(snap->page[page],16384,1,fichero);
				else
					uncompress_z80(fichero,16384,snap->page[page]);
			}
		}
	} else {

		if(compressed) {
			// 48k compressed z80 loader
     
			// we uncompress first the data
      
			uncompress_z80(fichero,49152,memo);
      
			memcpy(snap->page[0],memo,16384);
			memcpy(snap->page[1],memo+16384,16384);
			memcpy(snap->page[2],memo+32768,16384);
     
		} else {
			// 48k uncompressed z80 loader
      
			fread(snap->page[0],16384,1,fichero);
			fread(snap->page[1],16384,1,fichero);
			fread(snap->page[2],16384,1,fichero);
		}
		
	}

	load_snap(snap);
	fclose(fichero);
	free(memo);
	free(snap);
	return 0; // all right
}

int load_sna(const char *filename) {

	unsigned char *tempo;
	unsigned char *tempo2;
	unsigned char type=0;
	FILE *fichero;
	struct z80snapshot *snap;
	unsigned char v1,v2;
	int addr,loop;

	tempo=(unsigned char *)malloc(49179);
	tempo2=(unsigned char *)malloc(98308);
	snap=(struct z80snapshot *)malloc(sizeof(struct z80snapshot));
	
	printf("Loading SNA file\n");
	
	fichero=fopen(filename,"r");
	if(fichero==NULL) {
		free(tempo);
		free(tempo2);
		free(snap);
		return -1; // error
	}

	if (1!=fread(tempo,49179,1,fichero)) {
		free(tempo);
		free(tempo2);
		free(snap);
		fclose(fichero);
		return -1;
	}
	
	if (0==fread(tempo2,1,98308,fichero)) {
		printf("48K SNA\n");
		type=0;
	} else {
		printf("128K SNA\n");
		type=1;
	}
	
	snap->type=type;

	//Some inits
	for(loop=0;loop<16;loop++)
		snap->ay_regs[loop]=0;
	snap->ay_latch=0;
	snap->issue=3;
	snap->joystick=1; //kempston

	snap->I=tempo[0];
	snap->LL=tempo[1];
	snap->HH=tempo[2];
	snap->EE=tempo[3];
	snap->DD=tempo[4];
	snap->CC=tempo[5];
	snap->BB=tempo[6];
	snap->FF=tempo[7];
	snap->AA=tempo[8];
	
	snap->L=tempo[9];
	snap->H=tempo[10];
	snap->E=tempo[11];
	snap->D=tempo[12];
	snap->C=tempo[13];
	snap->B=tempo[14];
	
	snap->IY=((word)tempo[15])+256*((word)tempo[16]);
	snap->IX=((word)tempo[17])+256*((word)tempo[18]);
	
	if (tempo[19]&0x01) {
		snap->IFF1=1;
	} else {
		snap->IFF1=0;
	}
	
	if (tempo[19]&0x02) {
		snap->IFF2=1;
	} else {
		snap->IFF2=0;
	}
	
	snap->R=tempo[20];
	snap->F=tempo[21];
	snap->A=tempo[22];
	snap->SP=((word)tempo[23])+256*((word)tempo[24]);
	snap->Imode=tempo[25];
	snap->border=tempo[26];
	
	if (type==0) {
			
		v1=tempo[23];
		v2=tempo[24];
		addr=((int)v1)+256*((int)v2);
		if ((addr<16384)||(addr>=65534)) {
			free(tempo);
			free(tempo2);
			free(snap);
			fclose(fichero);
			printf("Error loading SNA file. Return address in ROM.\n");
			return -1;
		}
		addr-=16384;
		addr+=27;
		snap->PC=((word)tempo[addr])+256*((word)tempo[addr+1]);
		tempo[addr]=0;
		tempo[addr+1]=0;
		snap->SP+=2;
		snap->IFF1=snap->IFF2;
		memcpy(snap->page[0],tempo+27,16384);
		memcpy(snap->page[1],tempo+16411,16384);
		memcpy(snap->page[2],tempo+32795,16384);
	} else {
		snap->PC=((word)tempo2[0])+256*((word)tempo2[1]);
		memcpy(snap->page[5],tempo+27,16384);
		memcpy(snap->page[2],tempo+16411,16384);
		v1=tempo[2];
		snap->pager=v1;
		v1&=0x07;
		memcpy(snap->page[v1],tempo+32795,16384);
		addr=4;
		for (loop=0;loop<7;loop++) {
			if ((loop==2)||(loop==5)||(loop==((int)v1))) {
				continue;
			}
			memcpy(snap->page[loop],tempo2+addr,16384);
			addr+=16384;
		}
	}
	
	load_snap(snap);
	free(tempo);
	free(tempo2);
	free(snap);
	fclose(fichero);
	return 0;
	
}

void load_snap(struct z80snapshot *snap) {

  int bucle;

	printf("Loading SnapShot\n");

  switch(snap->type) {
  case 0: // 48k
  	printf("Mode 48K\n");
    ordenador->mode128k=0; // 48K mode
    ordenador->issue=snap->issue;
    ResetComputer();
    break;
  case 1: // 128k
  	printf("Mode 128K\n");
    ordenador->mode128k=2; // +2 mode
    ordenador->issue=3;
    ResetComputer();
    printf("Pager: %X\n",snap->pager);
    Z80free_Out(0x7FFD,snap->pager);
    break;
  default:
    break;
  }
  
  keyboard->joystick=snap->joystick;

  procesador.Rm.br.A=snap->A;
  procesador.Rm.br.F=snap->F;
  procesador.Rm.br.B=snap->B;
  procesador.Rm.br.C=snap->C;
  procesador.Rm.br.D=snap->D;
  procesador.Rm.br.E=snap->E;
  procesador.Rm.br.H=snap->H;
  procesador.Rm.br.L=snap->L;
  printf("A:%d F:%d B:%d C:%d D:%d E:%d H:%d L:%d\n",snap->A,snap->F,snap->B,snap->C,snap->D,snap->E,snap->H,snap->L);
  procesador.Ra.br.A=snap->AA;
  procesador.Ra.br.F=snap->FF;
  procesador.Ra.br.B=snap->BB;
  procesador.Ra.br.C=snap->CC;
  procesador.Ra.br.D=snap->DD;
  procesador.Ra.br.E=snap->EE;
  procesador.Ra.br.H=snap->HH;
  procesador.Ra.br.L=snap->LL;
  printf("A:%d F:%d B:%d C:%d D:%d E:%d H:%d L:%d\n",snap->AA,snap->FF,snap->BB,snap->CC,snap->DD,snap->EE,snap->HH,snap->LL);
  procesador.Rm.wr.IX=snap->IX;
  procesador.Rm.wr.IY=snap->IY;
  procesador.Rm.wr.SP=snap->SP;
  procesador.PC=snap->PC;
  printf("IX:%d IY:%d SP:%d PC:%d\n",snap->IX,snap->IY,snap->SP,snap->PC);
  procesador.I=snap->I;
  procesador.R=snap->R;
  procesador.R2=snap->R;
  printf("I:%d R:%d\n",snap->I,snap->R);

  if(snap->IFF1) {
    procesador.IFF1=1;
  } else {
    procesador.IFF1=0;
  }
  if(snap->IFF2) {
    procesador.IFF2=1;
  } else {
    procesador.IFF2=0;
  }

  procesador.IM=snap->Imode;
  Z80free_Out(0xFFFE,((snap->border&0x07)|0x10));

  switch(snap->type) {
  case 0: // 48K

    for(bucle=0;bucle<16384;bucle++) {
      ordenador->memoria[bucle+147456]=snap->page[0][bucle];
      ordenador->memoria[bucle+98304]=snap->page[1][bucle];
      ordenador->memoria[bucle+114688]=snap->page[2][bucle];
    }
    
    ordenador->ay_emul=0;
    break;
  case 1: // 128K

    for(bucle=0;bucle<16384;bucle++) {
      ordenador->memoria[bucle+65536]=snap->page[0][bucle];
      ordenador->memoria[bucle+81920]=snap->page[1][bucle];
      ordenador->memoria[bucle+98304]=snap->page[2][bucle];
      ordenador->memoria[bucle+114688]=snap->page[3][bucle];
      ordenador->memoria[bucle+131072]=snap->page[4][bucle];
      ordenador->memoria[bucle+147456]=snap->page[5][bucle];
      ordenador->memoria[bucle+163840]=snap->page[6][bucle];
      ordenador->memoria[bucle+180224]=snap->page[7][bucle];
    }
    ordenador->ay_emul=1;
    for(bucle=0;bucle<16;bucle++)
      ordenador->ay_registers[bucle]=snap->ay_regs[bucle];
    ordenador->ay_latch=snap->ay_latch;
    break;
  default:
    break;
  }
}
