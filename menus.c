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

#include "z80free/Z80free.h"
#include "computer.h"
#include "emulator.h"
#include "cargador.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include "characters.h"
#include "menus.h"
#include "tape.h"
#include <SDL/SDL.h>


// shows the settings menu

void print_copy(unsigned char *fbuffer,int ancho) {
	
	printstring(fbuffer,"(C) 2003-2011 Raster Software Vigo",-1,455,29,16,ancho);
	
}

void settings_menu() {

	unsigned char *fbuffer,fin;
	unsigned char texto[41];
	int ancho=screen->w;

	fbuffer=screen->pixels;

	fin=1;

	texto[0]=0;

	do {
		clean_screen();

		printstring(fbuffer,"Current settings",-1,5,31,16,ancho);
		switch(ordenador.mode128k) {
		case 0:
			if(ordenador.issue==2)
				sprintf(texto,"Mode: 48K issue2");
			else
				sprintf(texto,"Mode: 48K issue3");
		break;
		case 1:
			sprintf(texto,"Mode: Sinclair 128K");
		break;
		case 2:
			sprintf(texto,"Mode: Amstrad +2");
		break;
		case 3:
			sprintf(texto,"Mode: Amstrad +2A/+3");
		break;
		case 4:
			sprintf(texto,"Mode: Spanish 128K");
		break;
		}
  
		printstring(fbuffer,texto,-1,45,30,16,ancho);

		switch(ordenador.joystick) {
		case 0:
			sprintf(texto,"Joystick emulation: Cursor");
			break;
		case 1:
			sprintf(texto,"Joystick emulation: Kempston");
			break;
		case 2:
			sprintf(texto,"Joystick emulation: Sinclair (1)");
			break;
		case 3:
			sprintf(texto,"Joystick emulation: Sinclair (2)");
			break;
		}
		printstring(fbuffer,texto,-1,65,29,16,ancho);

		if(ordenador.ay_emul)
			sprintf(texto,"AY-3-8912 Emulation: enabled");
		else
			sprintf(texto,"AY-3-8912 Emulation: disabled");

		printstring(fbuffer,texto,-1,85,27,16,ancho);

		if(ordenador.mdr_active)
			sprintf(texto,"Interface I Emulation: enabled");
		else
			sprintf(texto,"Interface I Emulation: disabled");

		printstring(fbuffer,texto,-1,105,31,16,ancho);
	
		if(ordenador.dblscan)
			sprintf(texto,"Double scan: enabled");
		else
			sprintf(texto,"Double scan: disabled");

		printstring(fbuffer,texto,-1,125,28,16,ancho);

		if(ordenador.turbo)
			sprintf(texto,"TURBO mode: enabled");
		else
			sprintf(texto,"TURBO mode: disabled");

		printstring(fbuffer,texto,-1,145,30,16,ancho);
	
		printstring(fbuffer,"1:",30,180,28,16,ancho);
		printstring(fbuffer,"48K issue2",78,180,31,16,ancho);

		printstring(fbuffer,"2:",350,180,28,16,ancho);
		printstring(fbuffer,"48K issue3",398,180,31,16,ancho);

		printstring(fbuffer,"3:",30,220,28,16,ancho);
		printstring(fbuffer,"Sinclair 128K",78,220,31,16,ancho);

		printstring(fbuffer,"4:",350,220,28,16,ancho);
		printstring(fbuffer,"Amstrad +2",398,220,31,16,ancho);

		printstring(fbuffer,"5:",30,260,28,16,ancho);
		printstring(fbuffer,"Amstrad +2A/+3",78,260,31,16,ancho);

		printstring(fbuffer,"6:",350,260,28,16,ancho);
		printstring(fbuffer,"Spanish 128K",398,260,31,16,ancho);

		printstring(fbuffer,"7:",30,300,28,16,ancho);
		printstring(fbuffer,"Cursor",78,300,31,16,ancho);

		printstring(fbuffer,"8:",350,300,28,16,ancho);
		printstring(fbuffer,"Kempston",398,300,31,16,ancho);

		printstring(fbuffer,"9:",30,340,28,16,ancho);
		printstring(fbuffer,"Sinclair (1)",78,340,31,16,ancho);

		printstring(fbuffer,"0:",350,340,28,16,ancho);
		printstring(fbuffer,"Sinclair (2)",398,340,31,16,ancho);

		printstring(fbuffer,"I:",30,380,28,16,ancho);
		printstring(fbuffer,"Interface I",78,380,31,16,ancho);

		printstring(fbuffer,"D:",350,380,28,16,ancho);
		printstring(fbuffer,"Double Scan",398,380,31,16,ancho);

		printstring(fbuffer,"A:",350,420,28,16,ancho);
		printstring(fbuffer,"AY emulation",398,420,31,16,ancho);
		printstring(fbuffer,"T:",30,420,28,16,ancho);
		printstring(fbuffer,"TURBO mode",78,420,31,16,ancho);

		printstring(fbuffer,"ESC:",168,460,28,16,ancho);
		printstring(fbuffer,"return emulator",232,460,31,16,ancho);

		switch(wait_key()) {
		case SDLK_ESCAPE:
		case SDLK_RETURN:
			fin=0;
		break;
		case SDLK_1:
			ordenador.issue=2;
			ordenador.mode128k=0;
			ordenador.ay_emul=0;
			ResetComputer();
		break;
		case SDLK_2:
			ordenador.issue=3;
			ordenador.mode128k=0;
			ordenador.ay_emul=0;
			ResetComputer();
		break;
		case SDLK_3:
			ordenador.issue=3;
			ordenador.mode128k=1;
			ordenador.ay_emul=1;
			ResetComputer();
		break;
		case SDLK_4:
			ordenador.issue=3;
			ordenador.mode128k=2;
			ordenador.ay_emul=1;
			ResetComputer();
		break;
		case SDLK_5:
			ordenador.issue=3;
			ordenador.mode128k=3;
			ordenador.ay_emul=1;
			ordenador.mdr_active=0;
			ResetComputer();
		break;
		case SDLK_6:
			ordenador.issue=3;
			ordenador.mode128k=4;
			ordenador.ay_emul=1;
			ResetComputer();
		break;
		case SDLK_7:
			ordenador.joystick=0;
		break;
		case SDLK_8:
			ordenador.joystick=1;
		break;
		case SDLK_9:
			ordenador.joystick=2;
		break;
		case SDLK_0:
			ordenador.joystick=3;
		break;
		case SDLK_i:
			if(ordenador.mode128k!=3) {
				ordenador.mdr_active=1-ordenador.mdr_active;
				ResetComputer();
			}
		break;
		case SDLK_d:
			ordenador.dblscan=1-ordenador.dblscan;
		break;
		case SDLK_a:
			ordenador.ay_emul=1-ordenador.ay_emul;
		break;
		case SDLK_t:
			if(ordenador.turbo){
				ordenador.tst_sample=3500000/ordenador.freq;
				ordenador.turbo = 0;
			} else {
				ordenador.tst_sample=100000000/ordenador.freq;
				ordenador.turbo = 1;
			}
		}
	} while(fin);

	clean_screen();
}

// shows the help menu

void help_menu() {

	unsigned char *fbuffer,fin;
	int ancho=screen->w;

	fbuffer=screen->pixels;

	clean_screen();

	printstring(fbuffer,"FBZX (2.4.3)",-1,10,31,16,ancho);
	printstring(fbuffer,"Available keys",-1,40,30,16,ancho);
	printstring(fbuffer,"Shift:Caps Shift		Ctrl:Symbol Shift",-1,95,27,16,ancho);

	printstring(fbuffer,"F1:",14,160,28,16,ancho);
	printstring(fbuffer,"this help",62,160,31,16,ancho);

	printstring(fbuffer,"F2:",336,160,28,16,ancho);
	printstring(fbuffer,"manage snapshots",382,160,31,16,ancho);

	printstring(fbuffer,"F3:",14,200,28,16,ancho);
	printstring(fbuffer,"manage TAP/TZX",62,200,31,16,ancho);

	printstring(fbuffer,"F4:",336,200,28,16,ancho);
	printstring(fbuffer,"change settings",382,200,31,16,ancho);

	printstring(fbuffer,"F5:",14,240,28,16,ancho);
	printstring(fbuffer,"stop TAPE",62,240,31,16,ancho);

	printstring(fbuffer,"F6:",336,240,28,16,ancho);
	printstring(fbuffer,"play TAPE",382,240,31,16,ancho);

	printstring(fbuffer,"F7:",14,280,28,16,ancho);
	printstring(fbuffer,"manage MICRODRIVE",62,280,31,16,ancho);

	printstring(fbuffer,"F8:",336,280,28,16,ancho);
	printstring(fbuffer,"Keyboard layout",382,280,31,16,ancho);

	printstring(fbuffer,"F9:",14,320,28,16,ancho);
	printstring(fbuffer,"Toggle fullscreen",62,320,31,16,ancho);

	printstring(fbuffer,"F10:",336,320,28,16,ancho);
	printstring(fbuffer,"reset spectrum",398,320,31,16,ancho);

	printstring(fbuffer,"F11/O:",14,360,28,16,ancho);
	printstring(fbuffer,"volume low",110,360,31,16,ancho);

	printstring(fbuffer,"F12/P:",336,360,28,16,ancho);
	printstring(fbuffer,"volume up",430,360,31,16,ancho);

	printstring(fbuffer,"ESC:",184,400,28,16,ancho);
	printstring(fbuffer,"exit emulator",248,400,31,16,ancho);

	print_copy(fbuffer,ancho);

	fin=1;
	do {

		switch(wait_key()) {
		case SDLK_ESCAPE: // to exit the help
			fin=0;
		break;
		default:
		break;
		}
	} while(fin);
	clean_screen();
}


// shows the SNAPSHOTS menu

void snapshots_menu() {

	unsigned char *fbuffer,fin;
	int ancho=screen->w;

	fbuffer=screen->pixels;

	clean_screen();

	printstring(fbuffer,"SNAPSHOTS",-1,10,31,16,ancho);

	printstring(fbuffer,"1:",14,100,28,16,ancho);
	printstring(fbuffer,"load a Z80/SNA snapshot",62,100,31,16,ancho);


	if(ordenador.mode128k!=3) { // not in +3 mode
		printstring(fbuffer,"2:",14,160,28,16,ancho);
		printstring(fbuffer,"make a Z80 snapshot",62,160,31,16,ancho);
	} else {
		printstring(fbuffer,"Can't make snapshots in +3 mode",14,160,31,16,ancho);
	}

	printstring(fbuffer,"ESC:",170,400,28,16,ancho);
	printstring(fbuffer,"return emulator",234,400,31,16,ancho);

	print_copy(fbuffer,ancho);

	fin=1;
	do {
		switch(wait_key()) {
		case SDLK_ESCAPE: // to exit the help
			fin=0;
		break;
		case SDLK_1:
			fin=0;
			load_z80file();
		break;
		case SDLK_2:
			fin=0;
			if(ordenador.mode128k!=3) // not in +3 mode
				save_z80file();
		break;
		default:
		break;
		}
	} while(fin);
	clean_screen();
}


// shows the TAPs menu

void taps_menu() {

	unsigned char *fbuffer,fin;
	int ancho=screen->w;

	fbuffer=screen->pixels;
	
	fin=1;
	do {
		clean_screen();

		printstring(fbuffer,"TAP/TZX files",-1,10,31,16,ancho);

		printstring(fbuffer,"1:",14,50,28,16,ancho);
		printstring(fbuffer,"select a TAP/TZX file",62,50,31,16,ancho);


		printstring(fbuffer,"2:",14,90,28,16,ancho);
		printstring(fbuffer,"rewind TAP/TZX file",62,90,31,16,ancho);

		printstring(fbuffer,"3:",14,130,28,16,ancho);
		printstring(fbuffer,"fast/normal speed",62,130,31,16,ancho);

		printstring(fbuffer,"4:",14,170,28,16,ancho);
		printstring(fbuffer,"write protection",62,170,31,16,ancho);
		
		printstring(fbuffer,"5:",14,210,28,16,ancho);
		printstring(fbuffer,"create TAP file",62,210,31,16,ancho);
				
		printstring(fbuffer,"ESC:",14,250,28,16,ancho);
		printstring(fbuffer,"return emulator",78,250,31,16,ancho);

		printstring(fbuffer,"Current TAP/TZX file is:",-1,300,28,16,ancho);
		printstring(fbuffer,ordenador.current_tap,-1,320,28,16,ancho);

		print_copy(fbuffer,ancho);

		if(ordenador.tape_fast_load)
			printstring(fbuffer,"Fast load enabled	",10,420,30,16,ancho);
		else
			printstring(fbuffer,"Fast load disabled ",10,420,30,16,ancho);
		
		if(ordenador.tape_write)
			printstring(fbuffer,"Write enabled	",390,420,30,16,ancho);
		else
			printstring(fbuffer,"Write disabled ",390,420,30,16,ancho);

		switch(wait_key()) {
		case SDLK_ESCAPE: // to exit the help
			fin=0;
		break;
		case SDLK_1:
			ordenador.pause=1;
			select_tapfile();
		break;
		case SDLK_2:
			fin=0;
			ordenador.pause=1;
			if(ordenador.tap_file!=NULL) {
				ordenador.tape_current_mode=TAP_TRASH;
				rewind_tape(ordenador.tap_file,1);		
			}
			sprintf(ordenador.osd_text,"Tape rewinded");
			ordenador.osd_time=50;			
		break;
		case SDLK_3:
			ordenador.pause=1;
			ordenador.tape_fast_load=1-ordenador.tape_fast_load;
			if(ordenador.tap_file!=NULL) {
				ordenador.tape_current_mode=TAP_TRASH;
				rewind_tape(ordenador.tap_file,1);
			}
		break;
		case SDLK_4:
			ordenador.tape_write=1-ordenador.tape_write;
		break;
		case SDLK_5:
			create_tapfile();
		break;
		default:
		break;
		}

	} while(fin);

	clean_screen();
}

// shows a menu to allow user to choose a tape file

void select_tapfile() {

	unsigned char *videomem,*filename;
	int ancho,retorno,retval;
	unsigned char char_id[11];

	videomem=screen->pixels;
	ancho=screen->w;

	clean_screen();

	if(ordenador.tap_file!=NULL)
		rewind_tape(ordenador.tap_file,1);

	ordenador.tape_current_bit=0;
	ordenador.tape_current_mode=TAP_TRASH;

	printstring(videomem,"Choose the TAPE file to load",-1,32,29,16,ancho);

	filename=select_file(path_taps,1);

	if(filename==NULL) { // Aborted
		clean_screen();
		return;
	}

	if(ordenador.tap_file!=NULL)
		fclose(ordenador.tap_file);

	ordenador.tap_file=fopen(filename,"r+"); // read and write
	ordenador.tape_write = 0; // by default, can't record
	if(ordenador.tap_file==NULL)
		retorno=-1;
	else
		retorno=0;

	clean_screen();

	strcpy(ordenador.current_tap,filename);

	free(filename);

	switch(retorno) {
	case 0: // all right
	break;
	case -1:
		printstring(videomem,"Error: Can't load that file",-1,232,26,16,ancho);
		printstring(videomem,"Press any key",-1,248,26,16,ancho);
		ordenador.current_tap[0]=0;
		wait_key();
	break;
	}

	retval=fread(char_id,10,1,ordenador.tap_file); // read the (maybe) TZX header
	if((!strncmp(char_id,"ZXTape!",7)) && (char_id[7]==0x1A)&&(char_id[8]==1)) {
		ordenador.tape_file_type = TAP_TZX;
		rewind_tape(ordenador.tap_file,1);
	} else {
		ordenador.tape_file_type = TAP_TAP;
		rewind_tape(ordenador.tap_file,1);
	}

	clean_screen();
}

void create_tapfile() {

	unsigned char *videomem;
	int ancho,retorno,longitud;
	unsigned char nombre[35],nombre2[37];

	videomem=screen->pixels;
	ancho=screen->w;

	clean_screen();

	printstring(videomem,"Choose a name for the TAP file",-1,32,30,16,ancho);
	printstring(videomem,"(up to 30 characters)",-1,52,30,16,ancho);

	printstring(videomem,"TAP file will be saved in:",-1,132,28,16,ancho);
	printstring(videomem,path_taps,0,152,28,16,ancho);

	nombre[0]=127;
	nombre[1]=0;
	longitud=0;
	retorno=0;

	do {
		sprintf(nombre2," %s ",nombre);
		printstring(videomem,nombre2,-1,84,31,16,ancho);
		switch(wait_key()) {
		case SDLK_BACKSPACE:
			if(longitud>0) {
				nombre[longitud]=0;
				longitud--;
				nombre[longitud]=127;
			}
		break;
		case SDLK_ESCAPE:
			retorno=2;
		break;
		case SDLK_RETURN:
			retorno=1;
		break;
		case SDLK_a:
			if(longitud<30) {
				nombre[longitud++]='a';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_b:
			if(longitud<30) {
				nombre[longitud++]='b';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_c:
			if(longitud<30) {
				nombre[longitud++]='c';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_d:
			if(longitud<30) {
				nombre[longitud++]='d';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_e:
			if(longitud<30) {
				nombre[longitud++]='e';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_f:
			if(longitud<30) {
				nombre[longitud++]='f';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_g:
			if(longitud<30) {
				nombre[longitud++]='g';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_h:
			if(longitud<30) {
				nombre[longitud++]='h';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_i:
			if(longitud<30) {
				nombre[longitud++]='i';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_j:
			if(longitud<30) {
				nombre[longitud++]='j';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_k:
			if(longitud<30) {
				nombre[longitud++]='k';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_l:
			if(longitud<30) {
				nombre[longitud++]='l';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_m:
			if(longitud<30) {
				nombre[longitud++]='m';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_n:
			if(longitud<30) {
				nombre[longitud++]='n';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_o:
			if(longitud<30) {
				nombre[longitud++]='o';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_p:
			if(longitud<30) {
				nombre[longitud++]='p';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_q:
			if(longitud<30) {
				nombre[longitud++]='q';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_r:
			if(longitud<30) {
				nombre[longitud++]='r';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_s:
			if(longitud<30) {
				nombre[longitud++]='s';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_t:
			if(longitud<30) {
				nombre[longitud++]='t';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_u:
			if(longitud<30) {
				nombre[longitud++]='u';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_v:
			if(longitud<30) {
				nombre[longitud++]='v';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_w:
			if(longitud<30) {
				nombre[longitud++]='w';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_x:
			if(longitud<30) {
				nombre[longitud++]='x';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_y:
			if(longitud<30) {
				nombre[longitud++]='y';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_z:
			if(longitud<30) {
				nombre[longitud++]='z';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_0:
			if(longitud<30) {
				nombre[longitud++]='0';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_1:
			if(longitud<30) {
				nombre[longitud++]='1';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_2:
			if(longitud<30) {
				nombre[longitud++]='2';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_3:
			if(longitud<30) {
				nombre[longitud++]='3';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_4:
			if(longitud<30) {
				nombre[longitud++]='4';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_5:
			if(longitud<30) {
				nombre[longitud++]='5';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_6:
			if(longitud<30) {
				nombre[longitud++]='6';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_7:
			if(longitud<30) {
				nombre[longitud++]='7';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_8:
			if(longitud<30) {
				nombre[longitud++]='8';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_9:
			if(longitud<30) {
				nombre[longitud++]='9';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_MINUS:
			if(longitud<30) {
				nombre[longitud++]='-';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		}
	} while(!retorno);

	clean_screen();

	nombre[longitud]=0; // erase cursor

	if(retorno==2) // abort
		return;

	longitud=strlen(path_taps);
	if((path_snaps[longitud-1]!='/')&&(longitud>1))
		sprintf(nombre2,"%s/%s.tap",path_taps,nombre); // name
	else
		sprintf(nombre2,"%s%s.tap",path_taps,nombre);
	if(ordenador.tap_file!=NULL)
		fclose(ordenador.tap_file);
	
	ordenador.tap_file=fopen(nombre2,"r"); // test if it exists
	if(ordenador.tap_file==NULL)
		retorno=0;
	else
		retorno=-1;
	
	if(!retorno) {
		ordenador.tap_file=fopen(nombre2,"a+"); // create for read and write
		if(ordenador.tap_file==NULL)
			retorno=-2;
		else
			retorno=0;
	}
	ordenador.tape_write=1; // allow to write
	strcpy(ordenador.current_tap,nombre2);
	ordenador.tape_file_type = TAP_TAP;
	switch(retorno) {
	case 0:
	break;
	case -1:
		printstring(videomem,"File already exists",-1,80,26,16,ancho);
		ordenador.current_tap[0]=0;
		wait_key();
	break;
	case -2:
		printstring(videomem,"Can't create file",-1,80,26,16,ancho);
		ordenador.current_tap[0]=0;
		wait_key();
	break;
	}
	clean_screen();
}

// shows the microdrive menu

void microdrive_menu() {

	unsigned char *fbuffer,fin;
	int retval,ancho=screen->w;

	fbuffer=screen->pixels;
	
	fin=1;
	do {
			clean_screen();

		printstring(fbuffer,"MICRODRIVE files",-1,10,31,16,ancho);

		printstring(fbuffer,"1:",14,50,28,16,ancho);
		printstring(fbuffer,"select a MDR file",62,50,31,16,ancho);

		printstring(fbuffer,"2:",14,90,28,16,ancho);
		printstring(fbuffer,"create a MDR file",62,90,31,16,ancho);

		printstring(fbuffer,"3:",14,130,28,16,ancho);
		printstring(fbuffer,"write protection",62,130,31,16,ancho);
				
		printstring(fbuffer,"ESC:",14,170,28,16,ancho);
		printstring(fbuffer,"return emulator",78,170,31,16,ancho);

		printstring(fbuffer,"Current MDR file is:",-1,300,28,16,ancho);
		printstring(fbuffer,ordenador.mdr_current_mdr,-1,320,28,16,ancho);

		print_copy(fbuffer,ancho);
		
		if(!ordenador.mdr_cartridge[137922])
			printstring(fbuffer,"	Write enabled	",-1,420,30,16,ancho);
		else
			printstring(fbuffer," Write disabled ",-1,420,30,16,ancho);

		switch(wait_key()) {
		case SDLK_ESCAPE: // to exit the help
			fin=0;
		break;

		case SDLK_1:			
			select_mdrfile();
		break;
		case SDLK_2:
			create_mdrfile();
		break;
		case SDLK_3:
			if(ordenador.mdr_cartridge[137922])
				ordenador.mdr_cartridge[137922]=0;
			else
				ordenador.mdr_cartridge[137922]=1;
			ordenador.mdr_file=fopen(ordenador.mdr_current_mdr,"wb"); // create for write
			if(ordenador.mdr_file!=NULL) {				
				retval=fwrite(ordenador.mdr_cartridge,137923,1,ordenador.mdr_file); // save cartridge
				fclose(ordenador.mdr_file);
				ordenador.mdr_file=NULL;
				ordenador.mdr_modified=0;
			}			
		break;
		default:
		break;
		}

	} while(fin);

	clean_screen();
}

// shows a menu to allow user to choose a microdrive file

void select_mdrfile() {

	unsigned char *videomem,*filename;
	int ancho,retorno,retval;
	// unsigned char char_id[11];

	videomem=screen->pixels;
	ancho=screen->w;

	clean_screen();

	printstring(videomem,"Choose the MICRODRIVE file to load",-1,32,29,16,ancho);

	filename=select_file(path_mdrs,2); // MDR files

	if(filename==NULL) { // Aborted
		clean_screen();
		return;
	}

	ordenador.mdr_file=fopen(filename,"rb"); // read
	if(ordenador.mdr_file==NULL)
		retorno=-1;
	else {
		retorno=0;
	retval=fread(ordenador.mdr_cartridge,137923,1,ordenador.mdr_file); // read the cartridge in memory
	ordenador.mdr_modified=0; // not modified
	fclose(ordenador.mdr_file);
	ordenador.mdr_tapehead=0;
	}

	clean_screen();

	strcpy(ordenador.mdr_current_mdr,filename);

	free(filename);

	switch(retorno) {
	case 0: // all right
		break;
	case -1:
		printstring(videomem,"Error: Can't load that file",-1,232,26,16,ancho);
		printstring(videomem,"Press any key",-1,248,26,16,ancho);
		ordenador.mdr_current_mdr[0]=0;
		wait_key();
		break;
	}

	clean_screen();
}

void create_mdrfile() {

	unsigned char *videomem;
	int ancho,retorno,longitud,bucle,retval;
	unsigned char nombre[35],nombre2[37];

	videomem=screen->pixels;
	ancho=screen->w;

	clean_screen();

	printstring(videomem,"Choose a name for the MDR file",-1,32,30,16,ancho);
	printstring(videomem,"(up to 30 characters)",-1,52,30,16,ancho);

	printstring(videomem,"MDR file will be saved in:",-1,132,28,16,ancho);
	printstring(videomem,path_mdrs,0,152,28,16,ancho);

	nombre[0]=127;
	nombre[1]=0;
	longitud=0;
	retorno=0;

	do {
		sprintf(nombre2," %s ",nombre);
		printstring(videomem,nombre2,-1,84,31,16,ancho);
		switch(wait_key()) {
		case SDLK_BACKSPACE:
			if(longitud>0) {
				nombre[longitud]=0;
				longitud--;
				nombre[longitud]=127;
			}
		break;
		case SDLK_ESCAPE:
			retorno=2;
		break;
		case SDLK_RETURN:
			retorno=1;
		break;
		case SDLK_a:
			if(longitud<30) {
				nombre[longitud++]='a';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_b:
			if(longitud<30) {
				nombre[longitud++]='b';
				nombre[longitud]=127;
				nombre[longitud+1]=0;
			}
		break;
		case SDLK_c:
			if(longitud<30) {
	nombre[longitud++]='c';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_d:
			if(longitud<30) {
	nombre[longitud++]='d';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_e:
			if(longitud<30) {
	nombre[longitud++]='e';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_f:
			if(longitud<30) {
	nombre[longitud++]='f';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_g:
			if(longitud<30) {
	nombre[longitud++]='g';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_h:
			if(longitud<30) {
	nombre[longitud++]='h';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_i:
			if(longitud<30) {
	nombre[longitud++]='i';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_j:
			if(longitud<30) {
	nombre[longitud++]='j';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_k:
			if(longitud<30) {
	nombre[longitud++]='k';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_l:
			if(longitud<30) {
	nombre[longitud++]='l';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_m:
			if(longitud<30) {
	nombre[longitud++]='m';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_n:
			if(longitud<30) {
	nombre[longitud++]='n';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_o:
			if(longitud<30) {
	nombre[longitud++]='o';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_p:
			if(longitud<30) {
	nombre[longitud++]='p';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_q:
			if(longitud<30) {
	nombre[longitud++]='q';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_r:
			if(longitud<30) {
	nombre[longitud++]='r';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_s:
			if(longitud<30) {
	nombre[longitud++]='s';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_t:
			if(longitud<30) {
	nombre[longitud++]='t';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_u:
			if(longitud<30) {
	nombre[longitud++]='u';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_v:
			if(longitud<30) {
	nombre[longitud++]='v';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_w:
			if(longitud<30) {
	nombre[longitud++]='w';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_x:
			if(longitud<30) {
	nombre[longitud++]='x';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_y:
			if(longitud<30) {
	nombre[longitud++]='y';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_z:
			if(longitud<30) {
	nombre[longitud++]='z';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_0:
			if(longitud<30) {
	nombre[longitud++]='0';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_1:
			if(longitud<30) {
	nombre[longitud++]='1';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_2:
			if(longitud<30) {
	nombre[longitud++]='2';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_3:
			if(longitud<30) {
	nombre[longitud++]='3';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_4:
			if(longitud<30) {
	nombre[longitud++]='4';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_5:
			if(longitud<30) {
	nombre[longitud++]='5';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_6:
			if(longitud<30) {
	nombre[longitud++]='6';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_7:
			if(longitud<30) {
	nombre[longitud++]='7';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_8:
			if(longitud<30) {
				nombre[longitud++]='8';
				nombre[longitud]=127;
					nombre[longitud+1]=0;
			}
		break;
		case SDLK_9:
			if(longitud<30) {
				nombre[longitud++]='9';
				nombre[longitud]=127;
					nombre[longitud+1]=0;
			}
		break;
		case SDLK_MINUS:
			if(longitud<30) {
				nombre[longitud++]='-';
					nombre[longitud]=127;
					nombre[longitud+1]=0;
			}
		break;
		}
	} while(!retorno);

	clean_screen();

	nombre[longitud]=0; // erase cursor

	if(retorno==2) // abort
		return;

	longitud=strlen(path_mdrs);
	if((path_mdrs[longitud-1]!='/')&&(longitud>1))
		sprintf(nombre2,"%s/%s.mdr",path_mdrs,nombre); // name
	else
		sprintf(nombre2,"%s%s.mdr",path_mdrs,nombre);
		
	ordenador.mdr_file=fopen(nombre2,"r"); // test if it exists
	if(ordenador.mdr_file==NULL)
		retorno=0;
	else
		retorno=-1;
	
	if(!retorno) {
		ordenador.mdr_file=fopen(nombre2,"wb"); // create for write
		if(ordenador.mdr_file==NULL)
			retorno=-2;
		else {
			for(bucle=0;bucle<137921;bucle++)
				ordenador.mdr_cartridge[bucle]=0xFF; // erase cartridge
			ordenador.mdr_cartridge[137922]=0;
			retval=fwrite(ordenador.mdr_cartridge,137923,1,ordenador.mdr_file); // save cartridge
			fclose(ordenador.mdr_file);
			ordenador.mdr_file=NULL;
			ordenador.mdr_modified=0;
			retorno=0;
		}
	}	
	strcpy(ordenador.mdr_current_mdr,nombre2);	
	switch(retorno) {
	case 0:
	break;
	case -1:
		printstring(videomem,"File already exists",-1,80,26,16,ancho);
		ordenador.mdr_current_mdr[0]=0;
		wait_key();
	break;
	case -2:
		printstring(videomem,"Can't create file",-1,80,26,16,ancho);
		ordenador.mdr_current_mdr[0]=0;
		wait_key();
	break;
	}
	clean_screen();
}


// shows a menu to allow user to save a snapshot file

void save_z80file() {

	unsigned char *videomem;
	int ancho,retorno,longitud;
	unsigned char nombre[35],nombre2[37];

	videomem=screen->pixels;
	ancho=screen->w;

	clean_screen();

	printstring(videomem,"Choose a name for the snapshot",-1,32,30,16,ancho);
	printstring(videomem,"(up to 30 characters)",-1,52,30,16,ancho);

	printstring(videomem,"Snapshot will be saved in:",-1,132,28,16,ancho);
	printstring(videomem,path_snaps,0,152,28,16,ancho);

	nombre[0]=127;
	nombre[1]=0;
	longitud=0;
	retorno=0;

	do {
		sprintf(nombre2," %s ",nombre);
		printstring(videomem,nombre2,-1,84,31,16,ancho);
		switch(wait_key()) {
		case SDLK_BACKSPACE:
			if(longitud>0) {
	nombre[longitud]=0;
	longitud--;
	nombre[longitud]=127;
			}
		break;
		case SDLK_ESCAPE:
			retorno=2;
		break;
		case SDLK_RETURN:
			retorno=1;
		break;
		case SDLK_a:
			if(longitud<30) {
	nombre[longitud++]='a';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_b:
			if(longitud<30) {
	nombre[longitud++]='b';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_c:
			if(longitud<30) {
	nombre[longitud++]='c';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_d:
			if(longitud<30) {
	nombre[longitud++]='d';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_e:
			if(longitud<30) {
	nombre[longitud++]='e';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_f:
			if(longitud<30) {
	nombre[longitud++]='f';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_g:
			if(longitud<30) {
	nombre[longitud++]='g';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_h:
			if(longitud<30) {
	nombre[longitud++]='h';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_i:
			if(longitud<30) {
	nombre[longitud++]='i';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_j:
			if(longitud<30) {
	nombre[longitud++]='j';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_k:
			if(longitud<30) {
	nombre[longitud++]='k';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_l:
			if(longitud<30) {
	nombre[longitud++]='l';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_m:
			if(longitud<30) {
	nombre[longitud++]='m';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_n:
			if(longitud<30) {
	nombre[longitud++]='n';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_o:
			if(longitud<30) {
	nombre[longitud++]='o';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_p:
			if(longitud<30) {
	nombre[longitud++]='p';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_q:
			if(longitud<30) {
	nombre[longitud++]='q';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_r:
			if(longitud<30) {
	nombre[longitud++]='r';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_s:
			if(longitud<30) {
	nombre[longitud++]='s';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_t:
			if(longitud<30) {
	nombre[longitud++]='t';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_u:
			if(longitud<30) {
	nombre[longitud++]='u';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_v:
			if(longitud<30) {
	nombre[longitud++]='v';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_w:
			if(longitud<30) {
	nombre[longitud++]='w';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_x:
			if(longitud<30) {
	nombre[longitud++]='x';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_y:
			if(longitud<30) {
	nombre[longitud++]='y';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_z:
			if(longitud<30) {
	nombre[longitud++]='z';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_0:
			if(longitud<30) {
	nombre[longitud++]='0';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_1:
			if(longitud<30) {
	nombre[longitud++]='1';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_2:
			if(longitud<30) {
	nombre[longitud++]='2';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_3:
			if(longitud<30) {
	nombre[longitud++]='3';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_4:
			if(longitud<30) {
	nombre[longitud++]='4';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_5:
			if(longitud<30) {
	nombre[longitud++]='5';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_6:
			if(longitud<30) {
	nombre[longitud++]='6';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_7:
			if(longitud<30) {
	nombre[longitud++]='7';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_8:
			if(longitud<30) {
	nombre[longitud++]='8';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_9:
			if(longitud<30) {
	nombre[longitud++]='9';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		case SDLK_MINUS:
			if(longitud<30) {
	nombre[longitud++]='-';
	nombre[longitud]=127;
	nombre[longitud+1]=0;
			}
		break;
		}
	} while(!retorno);

	clean_screen();

	nombre[longitud]=0; // erase cursor

	if(retorno==2) // abort
		return;

	longitud=strlen(path_snaps);
	if((path_snaps[longitud-1]!='/')&&(longitud>1))
		sprintf(nombre2,"%s/%s.z80",path_snaps,nombre); // name
	else
		sprintf(nombre2,"%s%s.z80",path_snaps,nombre);
	retorno=save_z80(nombre2);
	switch(retorno) {
	case 0:
		break;
	case -1:
		printstring(videomem,"File already exists",-1,80,26,16,ancho);
		wait_key();
		break;
	case -2:
		printstring(videomem,"Can't create file",-1,80,26,16,ancho);
		wait_key();
		break;
	}
	clean_screen();
}

// shows a menu to allow user to load a snapshot file

void load_z80file() {


	unsigned char *videomem,*filename;
	int ancho,retorno;

	videomem=screen->pixels;
	ancho=screen->w;

	clean_screen();

	printstring(videomem,"Choose the snapshot file to load",-1,32,29,16,ancho);

	filename=select_file(path_snaps,0);

	if(filename==NULL) { // Aborted
		clean_screen();
		return;
	}

	retorno=load_z80(filename);
	free(filename);
	clean_screen();

	switch(retorno) {
	case 0: // all right
		break;
	case -1:
		printstring(videomem,"Error: Can't load that file",-1,232,26,16,ancho);
		printstring(videomem,"Press any key",-1,248,26,16,ancho);
		wait_key();
		break;
	case -2:
	case -3:
		printstring(videomem,"Error: unsuported snap file",-1,232,26,16,ancho);
		printstring(videomem,"Press any key",-1,248,26,16,ancho);
		wait_key();
		break;
	}
	clean_screen();
}




/* fills a FICHERO chained list with all the files and directories contained in PATH.
	 If KIND is 0, it returns only Snapshots, if is 1, it returns only TAPE files, and
	if is 2, it returns only MDR files */

struct fichero *read_directory(char *cpath,char kind) {

	struct fichero *listhead,*listend;
	struct dirent *entry;
	DIR *directory;
	struct stat estado;
	unsigned char path[2049],fichero[2049],extension[5],found;
	int bucle,length;

	if((kind!=0)&&(kind!=1)&&(kind!=2))
		printf("Error!!!!\n");
	
	strcpy(path,cpath);
	if('/'!=path[strlen(path)-1])
		strcat(path,"/"); // add the final / to the path

	listhead=malloc(sizeof(struct fichero));
	strcpy(listhead->nombre,"..");
	listhead->tipo=2;
	listhead->next=NULL;
	listend=listhead;

	directory=opendir(path);
	if(directory==NULL)
		return(listhead); // can't access the directory

	do {
		entry=readdir(directory);
		if((NULL!=entry)&&(strcmp(entry->d_name,"."))&&(strcmp(entry->d_name,".."))) {
			strcpy(fichero,path);
			strcat(fichero,entry->d_name);
			stat(fichero,&estado);			
			found=0; // by default is not a valid file...
			length=strlen(entry->d_name);			
			if(length>3) {
				extension[4]=0;
				for(bucle=0;bucle<4;bucle++)
					extension[bucle]=entry->d_name[length-4+bucle]; // copy the 4 last chars of the file (the extension)
				switch(kind) {
				case 0:
					if((!strcasecmp(extension,".z80"))||(!strcasecmp(extension,".sna")))
						found=1; // is a .z80 or SNA file
				break;
				case 1:
					if((!strcasecmp(extension,".tap"))||(!strcasecmp(extension,".tzx")))
						found=1; // is a .tap file
				break;
				case 2:
					if(!strcasecmp(extension,".mdr"))
						found=1; // is a .mdr file
				break;
				default:
				break;
				}
			} else
				found=0;			
			if(((found)||(S_ISDIR(estado.st_mode)))&&('.'!=entry->d_name[0])) { // is a directory. We must add it				
				listend->next=malloc(sizeof(struct fichero));
				listend=listend->next;
				listend->next=NULL;
				strcpy(listend->nombrepath,fichero);
				strcpy(listend->nombre,entry->d_name);
				if(S_ISDIR(estado.st_mode))
					listend->tipo=1; // a directory
				else
					listend->tipo=0; // a file
			}
		}
	} while(entry!=NULL);
	closedir(directory);
	return(listhead);
}

// deletes a filelist tree, freeing the memory used by it

void delete_filelist(struct fichero *filelist) {

	struct fichero *fl1,*fl2;

	fl1=fl2=filelist;

	while(fl1!=NULL) {
		fl2=fl1->next;
		free(fl1);
		fl1=fl2;
	}
}


/* allows user to choose a file from PATH. If KIND=0, only snapshots. If KIND=1, only
	 TAPE files */

char *select_file(char *path,char kind) {

	struct fichero *filelist,*fl2;
	unsigned char *pantalla,*pantalla2,fin,read,*salida;
	int bucle,ancho,numitems,selected,from,longitud;

	salida=(unsigned char *)malloc(2049);
	salida[0]=0;

	ancho=screen->w;

	// paints the rectangle

	pantalla=(screen->pixels)+(ancho*70)+10;
	pantalla2=(screen->pixels)+(ancho*460)+10;
	for(bucle=10;bucle<630;bucle++) {
		*(pantalla++)=28;
		*(pantalla2++)=28;
	}
	pantalla=(screen->pixels)+(ancho*70)+10;
	pantalla2=(screen->pixels)+(ancho*70)+630;
	for(bucle=70;bucle<460;bucle++) {
		*pantalla=28;
		*pantalla2=28;
		pantalla+=ancho;
		pantalla2+=ancho;
	}

	fin=1;
	read=1;
	selected=0;
	from=0;
	numitems=0;

	filelist=NULL;

	do {

		if(read) {
			filelist=read_directory(path,kind);
			read=0;

			fl2=filelist;
			numitems=0;
			while(fl2!=NULL) { // counts the number of items
				fl2=fl2->next;
				numitems++;
			}
			selected=0;
			from=0;
		}

		print_files(filelist,from,selected);

		switch(wait_key()) {
		case SDLK_ESCAPE: // to exit the help
			fin=0;
		break;
		case SDLK_UP:
			if(selected>0) {
				selected--;
				if(selected<from)
					from--;
			}
		break;
		case SDLK_DOWN:
			if(selected<(numitems-1)) {
				selected++;
				if(selected>(from+23)) // 23 is the total of items that can be displayed
					from++;
			}
		break;
		case SDLK_PAGEUP:
			for(bucle=0;bucle<15;bucle++)
				if(selected>0) {
					selected--;
					if(selected<from)
						from--;
				}
		break;
		case SDLK_PAGEDOWN:
			for(bucle=0;bucle<15;bucle++)
				if(selected<(numitems-1)) {
					selected++;
					if(selected>(from+23)) // 23 is the total of items that can be displayed
						from++;
				}
		break;
		case SDLK_RETURN:
			fl2=filelist;
			if(selected!=0)
				for(bucle=0;bucle<selected;bucle++)
					fl2=fl2->next;
			switch(fl2->tipo) {
			case 0: // select file
				strcpy(salida,fl2->nombrepath);
				delete_filelist(filelist);
				return(salida); // ends returning the filename
			break;
			case 1: // change directory
				strcpy(path,fl2->nombrepath); // new path_taps is namepath
				delete_filelist(filelist); // frees the memory
				read=1; // and redisplay all the files
			break;
			case 2: // upper directory
				longitud=strlen(path);
				if(longitud<2) // there's no upper directory
					break;
				if('/'==path[longitud-1]) { // is the char ended in '/' ?
					path[longitud-1]=0; // eliminated
					longitud--;
				}
				while('/'!=path[longitud-1]) {
					longitud--;
					path[longitud]=0;
				}
				if(longitud>2) { // it's not the upper directory
					longitud--;
					path[longitud]=0; // delete the final '/'
				}
				read=1;
			break;
			default:
			break;
			}
		break;
		default:
		break;
		}
	} while(fin);

	delete_filelist(filelist);
	return(NULL);

}

void keyboard_menu() {
	
	FILE *fichero;
	int bucle1,bucle2,retval;
	unsigned char *buffer,*buffer2,valor;

	buffer=screen->pixels;
	
	clean_screen();
	fichero=myfopen("fbzx/keymap.bmp","r");
	if (fichero==NULL) {
		strcpy(ordenador.osd_text,"Keymap picture not found");
		ordenador.osd_time=100;
		return;
	}
	if (ordenador.zaurus_mini==0) {
		for (bucle1=0;bucle1<344;bucle1++)
			for(bucle2=0;bucle2<640;bucle2++) {
				retval=fscanf(fichero,"%c",&valor);
				paint_one_pixel(colors+valor,buffer);
				buffer+=ordenador.bpp;
			}
	} else {
		buffer+=(479*ordenador.bpp);
		for(bucle1=0;bucle1<344;bucle1++) {
			buffer2=buffer;
			for(bucle2=0;bucle2<640;bucle2++) {
				retval=fscanf(fichero,"%c",&valor);
				paint_one_pixel(colors+valor,buffer);
				buffer+=(480*ordenador.bpp);
			}
			buffer=buffer2-ordenador.bpp;
		}
	}
	print_copy(screen->pixels,screen->w);
	wait_key();
	clean_screen();
}
	
	

void clean_screen() {

	int bucle;
	unsigned char *buffer;

	buffer=screen->pixels;

	for(bucle=0;bucle<((screen->h)*(screen->w)*(ordenador.bpp));bucle++)
		*(buffer++)=0;

}

// waits for a keystroke and returns its value

unsigned int wait_key() {

	char fin;
	unsigned int temporal_io=0;
	SDL_Event evento;

	fin=1;

	do {
		SDL_Flip(ordenador.screen);
		if(!SDL_WaitEvent(&evento))
			continue;

		if(evento.type!=SDL_KEYUP)
			continue;

		fin=0;

		temporal_io=(unsigned int)evento.key.keysym.sym;
	} while(fin);

	return (temporal_io);
}

// shows the files from the number FROM, and marks the file number MARK

void print_files(struct fichero *filelist,int from,int mark) {

	struct fichero *fl2;
	int bucle,numitems,ancho,pos;
	char ink1,ink2;
	unsigned char spaces[39]="                                      ";
	unsigned char namefile[2089];
	unsigned char *videomem;

	videomem=screen->pixels;
	ancho=screen->w;

	fl2=filelist;
	numitems=0;

	while(fl2!=NULL) { // counts the number of items
		fl2=fl2->next;
		numitems++;
	}

	ink1=ink2=0;

	fl2=filelist;
	pos=72;
	for(bucle=0;bucle<numitems;bucle++) {
		if(bucle>=from) {
			strcpy(namefile,fl2->nombre);
			strcat(namefile,spaces);
			namefile[36]=0; // we print up to 36 chars
			switch(fl2->tipo) {
			case 0: // file
				ink1=31;
				ink2=16;
			break;
			case 1: // directory
				ink1=28;
				ink2=20;
			break;
			case 2: // parent directory
				ink1=26;
				ink2=18;
			break;
			}
			if(bucle==mark)
				printstring(videomem,namefile,-1,pos,ink2,31,ancho);
			else
				printstring(videomem,namefile,-1,pos,ink1,16,ancho);
			pos+=16;
		}
		if((pos+16)>460)
			break; // reached bottom part of the rectangle
		fl2=fl2->next;
	}
	while((pos+16<460)) {
		printstring(videomem,spaces,-1,pos,16,16,ancho);
		pos+=16;
	}
}
