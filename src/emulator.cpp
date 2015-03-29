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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "z80free/Z80free.h"
#include "emulator.hh"
#include "cargador.hh"
#include "computer.hh"
#include "llscreen.hh"
#include "menus.hh"
#include "microdrive.hh"
#include "computer.hh"
#include "cmdline.hh"
#include "llsound.hh"
#include "keyboard.hh"

char debug_var=1;

Z80FREE procesador;
char salir;
char path_snaps[2049];
char path_taps[2049];
char path_mdrs[2049];
unsigned int jump_frames, curr_frames;
string filenames[5];

string load_a_rom(string *filenames) {
	
	string *pointer;
	int offset=0;
	ifstream *fichero;

	for(pointer=filenames; *pointer != ""; pointer++) {
		fichero=llscreen->myfopen(*pointer,ios::in|ios::binary);
		if(fichero == NULL) {
			return (*pointer);
		}
		fichero->read((char*)ordenador->memoria+offset,16384);
		offset+=16384;
		fichero->close();
		delete fichero;
	}
	return "";
}

void load_rom(char type) {

	string retval;
	ifstream *fichero;

	switch(type) {
	case 0:
		filenames[0]="spectrum-roms/48.rom";
		filenames[1]="";
		retval=load_a_rom(filenames);
		if (retval != "") {
			printf("Can't load file %s\n",retval.c_str());
			exit(1);
		}
	break;
	case 1:
		filenames[0]="spectrum-roms/128-0.rom";
		filenames[1]="spectrum-roms/128-1.rom";
		filenames[2]="";
		retval=load_a_rom(filenames);
		if (retval != "") {
			printf("Can't load file %s\n",retval.c_str());
			exit(1);
		}
	break;
	case 2:
		filenames[0]="spectrum-roms/plus2-0.rom";
		filenames[1]="spectrum-roms/plus2-1.rom";
		filenames[2]="";
		retval=load_a_rom(filenames);
		if (retval != "") {
			printf("Can't load file %s\n",retval.c_str());
			exit(1);
		}
	break;
	case 3:
		// first, try last version of PLUS3 roms
		
		filenames[0]="spectrum-roms/plus3-41-0.rom";
		filenames[1]="spectrum-roms/plus3-41-1.rom";
		filenames[2]="spectrum-roms/plus3-41-2.rom";
		filenames[3]="spectrum-roms/plus3-41-3.rom";
		filenames[4]="";
		retval=load_a_rom(filenames);
		if (retval != "") {
			printf("Can't load the Spectrum +3 ROM version 4.1. Trying with version 4.0\n");
			filenames[0]="spectrum-roms/plus3-40-0.rom";
			filenames[1]="spectrum-roms/plus3-40-1.rom";
			filenames[2]="spectrum-roms/plus3-40-2.rom";
			filenames[3]="spectrum-roms/plus3-40-3.rom";
			filenames[4]="";
			retval=load_a_rom(filenames);
			if (retval != "") {
				printf("Can't load the Spectrum +3 ROM version 4.0. Trying with legacy filenames\n");
				filenames[0]="spectrum-roms/plus3-0.rom";
				filenames[1]="spectrum-roms/plus3-1.rom";
				filenames[2]="spectrum-roms/plus3-2.rom";
				filenames[3]="spectrum-roms/plus3-3.rom";
				filenames[4]="";
				retval=load_a_rom(filenames);
				if (retval != "") {
					printf("Can't load file %s\n",retval.c_str());
					exit(1);
				}
			}
		}
	break;
	case 4:
		filenames[0]="spectrum-roms/128-spanish-0.rom";
		filenames[1]="spectrum-roms/128-spanish-1.rom";
		filenames[2]="";
		retval=load_a_rom(filenames);
		if (retval != "") {
			printf("Can't load file %s\n",retval.c_str());
			exit(1);
		}
	break;
	}
  
	fichero=llscreen->myfopen("spectrum-roms/if1-2.rom",ios::in|ios::binary); // load Interface1 ROM
	if(fichero == NULL) {
		delete fichero;
		// try legacy name
		fichero=llscreen->myfopen("spectrum-roms/if1-v2.rom",ios::in|ios::binary);
		if(fichero == NULL) {
			delete fichero;
			printf("Can't open Interface1 ROM file\n");
			exit(1);
		}
	}
	fichero->read((char*)ordenador->shadowrom,8192);
  	fichero->close();
  	delete fichero;
}

void end_system() {
	
	delete(llsound);
	delete(llscreen);

}

void load_main_game(const char *nombre) {

	int longitud;
	char *puntero;
	
	longitud=strlen(nombre);
	if (longitud<5) {
		return;
	}
	puntero=(char *)(nombre+(longitud-4));
	if ((0==strcasecmp(".z80",puntero))||(0==strcasecmp(".sna",puntero))) {
		load_z80(nombre);
		return;
	}
	
	if ((0==strcasecmp(".tap",puntero))||(0==strcasecmp(".tzx",puntero))) {
		ordenador->current_tap = nombre;
		OOTape->load_file(nombre);
		return;
	}
}

void save_config(struct computer *object) {
	
	char config_path[1024];
	int length;
	FILE *fconfig;
	
	strcpy(config_path,getenv("HOME"));
	length=strlen(config_path);
	if ((length>0)&&(config_path[length-1]!='/'))
		strcat(config_path,"/");
	strcat(config_path,".fbzx");
	fconfig = fopen(config_path,"wb");
	if (fconfig==NULL) {
		return;
	}
	fprintf(fconfig,"mode=%c%c",48+object->mode128k,10);
	fprintf(fconfig,"issue=%c%c",48+object->issue,10);
	fprintf(fconfig,"joystick=%c%c",48+keyboard->joystick,10);
	fprintf(fconfig,"ay_sound=%c%c",48+object->ay_emul,10);
	fprintf(fconfig,"interface1=%c%c",48+object->mdr_active,10);
	fprintf(fconfig,"doublescan=%c%c",object->dblscan ? '1' : '0',10);
	fprintf(fconfig,"volume=%c%c",65+(llsound->volume/4),10);
	fprintf(fconfig,"bw=%c%c",object->bw ? '1' : '0',10);
	fclose(fconfig);
}

void load_config(struct computer *object) {
	
	char config_path[1024];
	char line[1024],carac,done;
	int length,pos;
	FILE *fconfig;
	unsigned char volume=255,mode128k=255,issue=255,joystick=255,ay_emul=255,mdr_active=255,dblscan=255,bw=255;
	
	strcpy(config_path,getenv("HOME"));
	length=strlen(config_path);
	if ((length>0)&&(config_path[length-1]!='/'))
		strcat(config_path,"/");
	strcat(config_path,".fbzx");
	fconfig = fopen(config_path,"rb");
	if (fconfig==NULL) {
		return;
	}
	
	done=1;
	pos=0;
	line[0]=0;
	while(!feof(fconfig)) {
		if (done) {
			line[0]=0;
			pos=0;
			done=0;
		}
		if (0!=fread(&carac,1,1,fconfig)) {
			if ((carac!=13)&&(carac!=10)) {
				line[pos]=carac;
				if (pos<1023) {
					pos++;
				}
				continue;
			}
		}
		done=1;
		line[pos]=0;
		if (line[0]=='#') { // coment
			continue;
		}
		if (!strncmp(line,"mode=",5)) {
			mode128k=line[5]-'0';
			continue;
		}
		if (!strncmp(line,"issue=",6)) {
			issue=line[6]-'0';
			continue;
		}
		if (!strncmp(line,"joystick=",9)) {
			joystick=line[9]-'0';
			continue;
		}
		if (!strncmp(line,"ay_sound=",9)) {
			ay_emul=line[9]-'0';
			continue;
		}
		if (!strncmp(line,"interface1=",11)) {
			mdr_active=line[11]-'0';
			continue;
		}
		if (!strncmp(line,"doublescan=",11)) {
			dblscan=line[11]-'0';
			continue;
		}
		if (!strncmp(line,"volume=",7)) {
			volume=4*(line[7]-'A');
			continue;
		}
		if (!strncmp(line,"bw=",3)) {
			bw=(line[3]-'0');
			continue;
		}
	}
	
	if (mode128k<5) {
		object->mode128k=mode128k;
	}
	if (issue<4) {
		object->issue=issue;
	}
	if (joystick<4) {
		keyboard->joystick=joystick;
	}
	if (ay_emul<2) {
		object->ay_emul=ay_emul;
	}
	if (mdr_active<2) {
		object->mdr_active=mdr_active;
	}
	if (dblscan<2) {
		object->dblscan = dblscan==0 ? false : true;
	}
	if (bw<2) {
		object->bw = bw==0 ? false : true;
	}
	if (volume<255) {
		llsound->set_volume(volume);
	}
	
	fclose(fconfig);
}

int main(int argc,char *argv[]) {

	int bucle,tstados,argumento,length;
	string gamefile;
	word PC=0;

	CMDLine parse(argc,argv);

	osd = new OSD();
	if ((!parse.mini) && (!parse.rotate)) {
		llscreen = new LLScreen(640,480,0,0,parse.db,parse.hw);
		llscreen->rotate = false;
	} else {
		llscreen = new LLScreen(480,640,0,0,parse.db,parse.hw);
		llscreen->rotate = true;
	}

	// by default, try all sound modes
	enum e_soundtype sound_type = SOUND_AUTOMATIC;
	if (parse.nosound) {
		sound_type = SOUND_NO;
	}
	if (parse.oss) {
		sound_type = SOUND_OSS;
	}
	if (parse.pulse) {
		sound_type = SOUND_PULSEAUDIO;
	}
	if (parse.alsa) {
		sound_type = SOUND_ALSA;
	}

	llsound = new LLSound(sound_type);
	OOTape = new Tape();
	keyboard = new Keyboard();
	ordenador = new computer();

	load_config(ordenador);

	ordenador->zaurus_mini = 0;
	if (parse.mini) {
		ordenador->zaurus_mini = 1;
	} else if (parse.rotate) {
		ordenador->zaurus_mini = 2;
	}

	if (parse.ds) {
		ordenador->dblscan = true;
	}
	if (parse.ss) {
		ordenador->dblscan = false;
	}

	if (parse.bw) {
		ordenador->bw = true;
	}
	if (parse.color) {
		ordenador->bw = false;
	}
	screen = new Screen();

	gamefile = "";

	jump_frames = parse.jump;
	curr_frames = 0;

	printf("Computer init\n");
	printf("Modo: %d\n",ordenador->mode128k);

	atexit(end_system);

	if(parse.fs) {
		llscreen->fullscreen_switch();
	}

	SDL_WM_SetCaption("FBZX","");
	ordenador->interr=0;

	// assign initial values for PATH variables

	strcpy(path_snaps,getenv("HOME"));
	length=strlen(path_snaps);
	if ((length>0)&&(path_snaps[length-1]!='/')) {
		strcat(path_snaps,"/");
	}
	strcpy(path_taps,path_snaps);
	strcpy(path_mdrs,path_snaps);
	ordenador->current_tap = "";

	// assign random values to the memory before start execution

	printf("Reset memory\n");
	printf("Modo: %d\n",ordenador->mode128k);
	for(bucle=0;bucle<196608;bucle++)
		ordenador->memoria[bucle]=(unsigned char) rand();

	printf("Memory resetted\n");
	printf("Modo: %d\n",ordenador->mode128k);

	salir=1;
  
	printf("Init microdrive\n");
	microdrive_init();

	printf("Reset computer\n");
	ResetComputer();

	sleep(1);

	printf("Reset screen\n");
	llscreen->clear_screen();

	if (llsound->sound_aborted) {
		osd->set_message("Running without sound (read the FAQ)",2000);
	}

	printf("Modo: %d\n",ordenador->mode128k);
	printf("load main game\n");
	load_main_game(parse.gamefile.c_str());
	printf("Modo: %d\n",ordenador->mode128k);

	osd->set_message("Press F1 for help",4000);

	printf("BPP: %d\n",llscreen->bpp);
	while(salir) {

		do {
			ordenador->no_contention = false;
			tstados=Z80free_ustep(&procesador);
			ordenador->no_contention = true;
			if(tstados<0) {
				printf("Error %X\n",procesador.PC);
				exit(1);
			}
			ordenador->emulate(tstados); // execute the whole hardware emulation for that number of TSTATES
		} while(procesador.Status!=Z80XX);
		PC=procesador.PC;
				
		/* if PC is 0x0556, a call to LD_BYTES has been made, so if
		FAST_LOAD is 1, we must load the block in memory and return */

		if((!ordenador->mdr_paged) && (PC==0x0556) && (ordenador->tape_fast_load)) {
			if(ordenador->current_tap != "") {
				byte iff1 = procesador.IFF1;
				byte iff2 = procesador.IFF2;
				procesador.Rm.br.F &= ~F_Z;
				procesador.IFF1 = 0;
				procesador.IFF2 = 0;
				do_fast_load();
				procesador.IFF1 = iff1;
				procesador.IFF2 = iff2;
			} else {
				osd->set_message("No TAP/TZX file selected",1000);
			}
			continue;
		}
		
		/* if PC is 0x04C2, a call to SA_BYTES has been made, so if
		we want to save to the TAP file, we do it */
		
		if((!ordenador->mdr_paged) && (PC==0x04C2) && (ordenador->tape_write==1)) {

			uint8_t *data;
			uint8_t op_xor;
			uint8_t dato;
			uint32_t length;
			int pointer;

			length = (uint32_t)(procesador.Rm.wr.DE);
			length += 2;

			data = new uint8_t[length];
			pointer = 0;
			data[pointer++] = procesador.Rm.br.A; // flag

			op_xor = procesador.Rm.br.A;

			salir = 0;
			do {
				if (procesador.Rm.wr.DE == 0)
					salir = 2;
				if (!salir) {
					dato = Z80free_Rd(procesador.Rm.wr.IX); // read data
					op_xor^=dato;
					data[pointer++] = dato;
					procesador.Rm.wr.IX++;
					procesador.Rm.wr.DE--;
				}
			} while (!salir);
			data[pointer] = op_xor;
			procesador.Rm.wr.IX++;
			procesador.Rm.wr.IX++;
			OOTape->add_block(data,length);
			ordenador->other_ret = 1;	// next instruction must be RET
			continue;
		}
		
		/* if ordenador->mdr_paged is 2, we have executed the RET at 0x0700, so
		we have to return to the classic ROM */
		
		if(ordenador->mdr_paged==2)
			ordenador->mdr_paged=0;
		
		/* if PC is 0x0008 or 0x1708, and we have a microdrive, we have to page
		the Interface 1 ROM */
		
		if(((PC==0x0008)||(PC==0x1708))&&(ordenador->mdr_active))
			ordenador->mdr_paged = 1;
		
		/* if PC is 0x0700 and we have a microdrive, we have to unpage
		the Interface 1 ROM after the last instruction */
		
		if((PC==0x0700)&&(ordenador->mdr_active))
			ordenador->mdr_paged = 2;

		if(ordenador->interr==1) {
			keyboard->read_keyboard (NULL);	// read the physical keyboard
			Z80free_INT(&procesador,ordenador->bus_empty());
			ordenador->interr=0;
		}
	}

	save_config(ordenador);
	return 0;
}

void do_fast_load() {

	if (!(procesador.Rm.br.F & F_C)) { // if Carry=0, is VERIFY, so return OK
		procesador.Rm.br.F |= F_C;	 // verify OK
		procesador.Rm.wr.IX += procesador.Rm.wr.DE;
		procesador.Rm.wr.DE = 0;
		ordenador->other_ret = 1;	// next instruction must be RET
		return;
	}

	uint16_t size;
	uint8_t flag = procesador.Rm.br.A;
	uint8_t data[65538];
	uint16_t counter;

	while(true) {
		enum FastLoadReturn retval = OOTape->fast_read(data,size,flag);

		switch (retval) {
		case FASTLOAD_NO_TAPE:
			procesador.Rm.br.F &= (~F_C);	// Load error
			procesador.Rm.wr.IX += procesador.Rm.wr.DE;
			procesador.Rm.wr.DE = 0;
			osd->set_message("No tape selected",2000);
			ordenador->other_ret = 1;	// next instruction must be RET
			return;
		break;
		case FASTLOAD_NO_BLOCK:
			if (OOTape->get_pause()) {
				osd->set_message("Can't do fast load. Press F6 to play",2000);
			}
			ordenador->other_ret = 0;	// next instruction must NOT be RET
			return;
		break;
		case FASTLOAD_END_TAPE:
			procesador.Rm.br.F &= (~F_C);	// Load error
			procesador.Rm.wr.IX += procesador.Rm.wr.DE;
			procesador.Rm.wr.DE = 0;
			osd->set_message("End of tape. Rewind it.",2000);
			ordenador->other_ret = 1;	// next instruction must be RET
			return;
		case FASTLOAD_OK:
			counter = 0;
			while (true) {
				if ((size == 0) || (procesador.Rm.wr.DE == 0)) {
					break;
				}
				Z80free_Wr (procesador.Rm.wr.IX, (byte) data[counter]); // store the byte
				procesador.Rm.wr.IX++;
				procesador.Rm.wr.DE--;
				counter++;
				size--;
			}
			printf("\n");
			if (size == 0) {
				if (procesador.Rm.wr.DE == 0) {
					procesador.Rm.br.F |= (F_C);	// Load OK
					printf("Load OK\n");
					ordenador->other_ret = 1;	// next instruction must be RET
					return;
				}
			}
			procesador.Rm.br.F &= (~F_C);	// Load error
			ordenador->other_ret = 1;	// next instruction must be RET
			return;
		break;
		case FASTLOAD_NO_FLAG:
			printf("No flag\n");
			continue;
		break;
		}
	}
}
