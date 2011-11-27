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
	
	print_string(fbuffer,"(C) 2003-2011 Raster Software Vigo",-1,455,13,0,ancho);
	
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

		print_string(fbuffer,"Current settings",-1,5,15,0,ancho);
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
  
		print_string(fbuffer,texto,-1,45,14,0,ancho);

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
		print_string(fbuffer,texto,-1,65,13,0,ancho);

		if(ordenador.ay_emul)
			sprintf(texto,"AY-3-8912 Emulation: enabled");
		else
			sprintf(texto,"AY-3-8912 Emulation: disabled");

		print_string(fbuffer,texto,-1,85,11,0,ancho);

		if(ordenador.mdr_active)
			sprintf(texto,"Interface I Emulation: enabled");
		else
			sprintf(texto,"Interface I Emulation: disabled");

		print_string(fbuffer,texto,-1,105,15,0,ancho);
	
		if(ordenador.dblscan)
			sprintf(texto,"Double scan: enabled");
		else
			sprintf(texto,"Double scan: disabled");

		print_string(fbuffer,texto,-1,125,12,0,ancho);

		if(ordenador.turbo)
			sprintf(texto,"TURBO mode: enabled");
		else
			sprintf(texto,"TURBO mode: disabled");
		print_string(fbuffer,texto,-1,145,14,0,ancho);

		if (ordenador.bw) {
			print_string(fbuffer,"TV Set: \001\011B\001\012&\001\014W",-1,165,15,0,ancho);
		} else {
			print_string(fbuffer,"TV Set: \001\012C\001\014o\001\015l\001\016o\001\013r",-1,165,15,0,ancho);
		}
	
		print_string(fbuffer,"1:",30,190,12,0,ancho);
		print_string(fbuffer,"48K issue2",78,190,15,0,ancho);

		print_string(fbuffer,"2:",350,190,12,0,ancho);
		print_string(fbuffer,"48K issue3",398,190,15,0,ancho);

		print_string(fbuffer,"3:",30,220,12,0,ancho);
		print_string(fbuffer,"Sinclair 128K",78,220,15,0,ancho);

		print_string(fbuffer,"4:",350,220,12,0,ancho);
		print_string(fbuffer,"Amstrad +2",398,220,15,0,ancho);

		print_string(fbuffer,"5:",30,250,12,0,ancho);
		print_string(fbuffer,"Amstrad +2A/+3",78,250,15,0,ancho);

		print_string(fbuffer,"6:",350,250,12,0,ancho);
		print_string(fbuffer,"Spanish 128K",398,250,15,0,ancho);

		print_string(fbuffer,"7:",30,280,12,0,ancho);
		print_string(fbuffer,"Cursor",78,280,15,0,ancho);

		print_string(fbuffer,"8:",350,280,12,0,ancho);
		print_string(fbuffer,"Kempston",398,280,15,0,ancho);

		print_string(fbuffer,"9:",30,310,12,0,ancho);
		print_string(fbuffer,"Sinclair (1)",78,310,15,0,ancho);

		print_string(fbuffer,"0:",350,310,12,0,ancho);
		print_string(fbuffer,"Sinclair (2)",398,310,15,0,ancho);

		print_string(fbuffer,"I:",30,340,12,0,ancho);
		print_string(fbuffer,"Interface I",78,340,15,0,ancho);

		print_string(fbuffer,"D:",350,340,12,0,ancho);
		print_string(fbuffer,"Double Scan",398,340,15,0,ancho);

		print_string(fbuffer,"A:",350,370,12,0,ancho);
		print_string(fbuffer,"AY emulation",398,370,15,0,ancho);
		print_string(fbuffer,"T:",30,370,12,0,ancho);
		print_string(fbuffer,"TURBO mode",78,370,15,0,ancho);

		print_string(fbuffer,"V:",30,400,12,0,ancho);
		print_string(fbuffer,"TV Set mode",78,400,15,0,ancho);

		print_string(fbuffer,"ESC:",168,460,12,0,ancho);
		print_string(fbuffer,"return emulator",232,460,15,0,ancho);

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
		case SDLK_v:
			ordenador.bw=1-ordenador.bw;
			computer_set_palete();
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

	print_string(fbuffer,"FBZX (2.5.0)",-1,10,15,0,ancho);
	print_string(fbuffer,"Available keys",-1,40,14,0,ancho);
	print_string(fbuffer,"Shift:Caps Shift    Ctrl:Symbol Shift",-1,95,11,0,ancho);

	print_string(fbuffer,"F1:",14,160,12,0,ancho);
	print_string(fbuffer,"this help",62,160,15,0,ancho);

	print_string(fbuffer,"F2:",336,160,12,0,ancho);
	print_string(fbuffer,"manage snapshots",382,160,15,0,ancho);

	print_string(fbuffer,"F3:",14,200,12,0,ancho);
	print_string(fbuffer,"manage TAP/TZX",62,200,15,0,ancho);

	print_string(fbuffer,"F4:",336,200,12,0,ancho);
	print_string(fbuffer,"change settings",382,200,15,0,ancho);

	print_string(fbuffer,"F5:",14,240,12,0,ancho);
	print_string(fbuffer,"stop TAPE",62,240,15,0,ancho);

	print_string(fbuffer,"F6:",336,240,12,0,ancho);
	print_string(fbuffer,"play TAPE",382,240,15,0,ancho);

	print_string(fbuffer,"F7:",14,280,12,0,ancho);
	print_string(fbuffer,"manage MICRODRIVE",62,280,15,0,ancho);

	print_string(fbuffer,"F8:",336,280,12,0,ancho);
	print_string(fbuffer,"tools",382,280,15,0,ancho);

	print_string(fbuffer,"F9:",14,320,12,0,ancho);
	print_string(fbuffer,"Toggle fullscreen",62,320,15,0,ancho);

	print_string(fbuffer,"F10:",336,320,12,0,ancho);
	print_string(fbuffer,"reset spectrum",398,320,15,0,ancho);

	print_string(fbuffer,"F11/O:",14,360,12,0,ancho);
	print_string(fbuffer,"volume low",110,360,15,0,ancho);

	print_string(fbuffer,"F12/P:",336,360,12,0,ancho);
	print_string(fbuffer,"volume up",430,360,15,0,ancho);

	print_string(fbuffer,"ESC:",184,400,12,0,ancho);
	print_string(fbuffer,"exit emulator",248,400,15,0,ancho);

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


// shows the POKE menu

void do_poke() {

	unsigned char *videomem,string[80];
	int ancho,retorno,address,old_value,new_value;

	videomem=screen->pixels;
	ancho=screen->w;

	clean_screen();

	while(1) {
		print_string(videomem,"Type address to POKE",-1,32,15,0,ancho);
		print_string(videomem,"(ESC to exit)",-1,52,12,0,ancho);

		retorno=ask_value(&address,84,65535);

		clean_screen();

		if (retorno==2) {
			return;
		}

		if ((address<16384) && ((ordenador.mode128k != 3) || (1 != (ordenador.mport2 & 0x01)))) {
			print_string(videomem,"That address is ROM memory.",-1,13,15,0,ancho);
			continue;
		}

		switch (address & 0x0C000) {
		case 0x0000:
			old_value= (*(ordenador.block0 + address));
		break;

		case 0x4000:
			old_value= (*(ordenador.block1 + address));
		break;

		case 0x8000:
			old_value= (*(ordenador.block2 + address));
		break;

		case 0xC000:
			old_value= (*(ordenador.block3 + address));
		break;
		default:
			old_value=0;
		break;
		}

		print_string(videomem,"Type new value to POKE",-1,32,15,0,ancho);
		print_string(videomem,"(ESC to cancel)",-1,52,12,0,ancho);
		sprintf(string,"Address: %d; old value: %d\n",address,old_value);
		print_string(videomem,string,-1,130,14,0,ancho);

		retorno=ask_value(&new_value,84,255);

		clean_screen();

		if (retorno==2) {
			continue;
		}

		switch (address & 0x0C000) {
		case 0x0000:
			(*(ordenador.block0 + address))=new_value;
		break;

		case 0x4000:
			(*(ordenador.block1 + address))=new_value;
		break;

		case 0x8000:
			(*(ordenador.block2 + address))=new_value;
		break;

		case 0xC000:
			(*(ordenador.block3 + address))=new_value;
		break;
		default:
		break;
		}

		sprintf(string,"Set address %d from %d to %d\n",address,old_value,new_value);
		print_string(videomem,string,-1,130,14,0,ancho);

	}
}

// shows the tools menu

void tools_menu() {

	unsigned char *fbuffer,fin;
	int ancho=screen->w;

	fbuffer=screen->pixels;

	fin=1;
	do {
		clean_screen();

		print_string(fbuffer,"Tools",-1,10,15,0,ancho);

		print_string(fbuffer,"1:",14,50,12,0,ancho);
		print_string(fbuffer,"show keyboard template",62,50,15,0,ancho);

		print_string(fbuffer,"2:",14,90,12,0,ancho);
		print_string(fbuffer,"insert POKEs",62,90,15,0,ancho);

		print_string(fbuffer,"ESC:",14,250,12,0,ancho);
		print_string(fbuffer,"return emulator",78,250,15,0,ancho);

		print_copy(fbuffer,ancho);

		switch(wait_key()) {
		case SDLK_ESCAPE: // to exit the help
			fin=0;
		break;
		case SDLK_1:
			fin=0;
			keyboard_menu();
		break;
		case SDLK_2:
			fin=0;
			do_poke();
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

	print_string(fbuffer,"SNAPSHOTS",-1,10,15,0,ancho);

	print_string(fbuffer,"1:",14,100,12,0,ancho);
	print_string(fbuffer,"load a Z80/SNA snapshot",62,100,15,0,ancho);

	if(ordenador.mode128k!=3) { // not in +3 mode
		print_string(fbuffer,"2:",14,160,12,0,ancho);
		print_string(fbuffer,"make a Z80 snapshot",62,160,15,0,ancho);
	} else {
		print_string(fbuffer,"Can't make snapshots in +3 mode",14,160,15,0,ancho);
	}

	print_string(fbuffer,"3: \001\017load a SCR snapshot",14,220,12,0,ancho);

	print_string(fbuffer,"4: \001\017save a SCR snapshot",14,280,12,0,ancho);

	print_string(fbuffer,"ESC: \001\017return to emulator",-1,400,12,0,ancho);

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
		case SDLK_3:
			fin=0;
			load_scrfile();
		break;
		case SDLK_4:
			fin=0;
			create_scrfile();
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

		print_string(fbuffer,"TAP/TZX files",-1,10,15,0,ancho);

		print_string(fbuffer,"1:",14,50,12,0,ancho);
		print_string(fbuffer,"select a TAP/TZX file",62,50,15,0,ancho);


		print_string(fbuffer,"2:",14,90,12,0,ancho);
		print_string(fbuffer,"rewind TAP/TZX file",62,90,15,0,ancho);

		print_string(fbuffer,"3:",14,130,12,0,ancho);
		print_string(fbuffer,"fast/normal speed",62,130,15,0,ancho);

		print_string(fbuffer,"4:",14,170,12,0,ancho);
		print_string(fbuffer,"write protection",62,170,15,0,ancho);
		
		print_string(fbuffer,"5:",14,210,12,0,ancho);
		print_string(fbuffer,"create TAP file",62,210,15,0,ancho);
				
		print_string(fbuffer,"ESC:",14,250,12,0,ancho);
		print_string(fbuffer,"return emulator",78,250,15,0,ancho);

		print_string(fbuffer,"Current TAP/TZX file is:",-1,300,12,0,ancho);
		print_string(fbuffer,ordenador.current_tap,-1,320,12,0,ancho);

		print_copy(fbuffer,ancho);

		if(ordenador.tape_fast_load)
			print_string(fbuffer,"Fast load enabled	",10,420,14,0,ancho);
		else
			print_string(fbuffer,"Fast load disabled ",10,420,14,0,ancho);
		
		if(ordenador.tape_write)
			print_string(fbuffer,"Write enabled",390,420,14,0,ancho);
		else
			print_string(fbuffer,"Write disabled",390,420,14,0,ancho);

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

	print_string(videomem,"Choose the TAPE file to load",-1,32,13,0,ancho);

	filename=select_file(path_taps,FILETYPE_TAP_TZX);

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
		print_string(videomem,"Error: Can't load that file",-1,232,10,0,ancho);
		print_string(videomem,"Press any key",-1,248,10,0,ancho);
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
	int ancho,retorno;
	unsigned char nombre2[1024];

	videomem=screen->pixels;
	ancho=screen->w;

	clean_screen();

	print_string(videomem,"Choose a name for the TAP file",-1,32,14,0,ancho);
	print_string(videomem,"(up to 30 characters)",-1,52,14,0,ancho);

	print_string(videomem,"TAP file will be saved in:",-1,132,12,0,ancho);
	print_string(videomem,path_taps,0,152,12,0,ancho);


	retorno=ask_filename(nombre2,84,"tap");

	clean_screen();

	if(retorno==2) // abort
		return;

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
		print_string(videomem,"File already exists",-1,80,10,0,ancho);
		ordenador.current_tap[0]=0;
		wait_key();
	break;
	case -2:
		print_string(videomem,"Can't create file",-1,80,10,0,ancho);
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

		print_string(fbuffer,"MICRODRIVE files",-1,10,15,0,ancho);

		print_string(fbuffer,"1:",14,50,12,0,ancho);
		print_string(fbuffer,"select a MDR file",62,50,15,0,ancho);

		print_string(fbuffer,"2:",14,90,12,0,ancho);
		print_string(fbuffer,"create a MDR file",62,90,15,0,ancho);

		print_string(fbuffer,"3:",14,130,12,0,ancho);
		print_string(fbuffer,"write protection",62,130,15,0,ancho);
				
		print_string(fbuffer,"ESC:",14,170,12,0,ancho);
		print_string(fbuffer,"return emulator",78,170,15,0,ancho);

		print_string(fbuffer,"Current MDR file is:",-1,300,12,0,ancho);
		print_string(fbuffer,ordenador.mdr_current_mdr,-1,320,12,0,ancho);

		print_copy(fbuffer,ancho);
		
		if(!ordenador.mdr_cartridge[137922])
			print_string(fbuffer,"Write enabled",-1,420,14,0,ancho);
		else
			print_string(fbuffer,"Write disabled",-1,420,14,0,ancho);

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

	print_string(videomem,"Choose the MICRODRIVE file to load",-1,32,13,0,ancho);

	filename=select_file(path_mdrs,FILETYPE_MDR); // MDR files

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
		print_string(videomem,"Error: Can't load that file",-1,232,10,0,ancho);
		print_string(videomem,"Press any key",-1,248,10,0,ancho);
		ordenador.mdr_current_mdr[0]=0;
		wait_key();
		break;
	}

	clean_screen();
}

void create_mdrfile() {

	unsigned char *videomem;
	int ancho,retorno,bucle,retval;
	unsigned char nombre2[1024];

	videomem=screen->pixels;
	ancho=screen->w;

	clean_screen();

	print_string(videomem,"Choose a name for the MDR file",-1,32,14,0,ancho);
	print_string(videomem,"(up to 30 characters)",-1,52,14,0,ancho);

	print_string(videomem,"MDR file will be saved in:",-1,132,12,0,ancho);
	print_string(videomem,path_mdrs,0,152,12,0,ancho);

	retorno=ask_filename(nombre2,84,"mdr");

	clean_screen();

	if(retorno==2) // abort
		return;

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
		print_string(videomem,"File already exists",-1,80,10,0,ancho);
		ordenador.mdr_current_mdr[0]=0;
		wait_key();
	break;
	case -2:
		print_string(videomem,"Can't create file",-1,80,10,0,ancho);
		ordenador.mdr_current_mdr[0]=0;
		wait_key();
	break;
	}
	clean_screen();
}


void create_scrfile() {

	unsigned char *videomem;
	int ancho,retorno,retval;
	unsigned char nombre2[1024];
	FILE *fichero;

	videomem=screen->pixels;
	ancho=screen->w;

	clean_screen();

	print_string(videomem,"Choose a name for the SCR file",-1,32,14,0,ancho);
	print_string(videomem,"(up to 30 characters)",-1,52,14,0,ancho);

	print_string(videomem,"SCR file will be saved in:",-1,132,12,0,ancho);
	print_string(videomem,path_mdrs,0,152,12,0,ancho);

	retorno=ask_filename(nombre2,84,"scr");

	clean_screen();

	if(retorno==2) // abort
		return;

	fichero=fopen(nombre2,"r"); // test if it exists
	if(fichero==NULL)
		retorno=0;
	else {
		fclose(fichero);
		retorno=-1;
	}

	if(!retorno) {
		fichero=fopen(nombre2,"wb"); // create for write
		if(fichero==NULL)
			retorno=-2;
		else {
			retval=fwrite(ordenador.block1+0x04000,6912,1,fichero); // save screen
			if (ordenador.ulaplus!=0) {
				retval=fwrite(ordenador.ulaplus_palete,64,1,fichero); // save ULAPlus palete
			}
			fclose(fichero);
			retorno=0;
		}
	}

	switch(retorno) {
	case -1:
		print_string(videomem,"File already exists",-1,80,10,0,ancho);
		wait_key();
	break;
	case -2:
		print_string(videomem,"Can't create file",-1,80,10,0,ancho);
		wait_key();
	break;
	default:
	break;
	}
	clean_screen();
}


int ask_filename(char *nombre_final,int y_coord,char *extension) {

	int longitud,retorno;
	unsigned char nombre[37],nombre2[38];

	unsigned char *videomem;
	int ancho;

	videomem=screen->pixels;
	ancho=screen->w;

	nombre[0]=127;
	nombre[1]=0;
	longitud=0;
	retorno=0;

	do {
		sprintf (nombre2, " %s.%s ", nombre,extension);
		print_string (videomem, nombre2, -1, y_coord, 15, 0, ancho);
		switch (wait_key ()) {
		case SDLK_BACKSPACE:
			if (longitud > 0) {
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
			if (longitud < 30) {
				nombre[longitud++]='a';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_b:
			if (longitud < 30) {
				nombre[longitud++]='b';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_c:
			if (longitud < 30) {
				nombre[longitud++]='c';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_d:
			if (longitud < 30) {
				nombre[longitud++]='d';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_e:
			if (longitud < 30) {
				nombre[longitud++]='e';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_f:
			if (longitud < 30) {
				nombre[longitud++]='f';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_g:
			if (longitud < 30) {
				nombre[longitud++]='g';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_h:
			if (longitud < 30) {
				nombre[longitud++]='h';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_i:
			if (longitud < 30) {
				nombre[longitud++]='i';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_j:
			if (longitud < 30) {
				nombre[longitud++]='j';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_k:
			if (longitud < 30) {
				nombre[longitud++]='k';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_l:
			if (longitud < 30) {
				nombre[longitud++]='l';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_m:
			if (longitud < 30) {
				nombre[longitud++]='m';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_n:
			if (longitud < 30) {
				nombre[longitud++]='n';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_o:
			if (longitud < 30) {
				nombre[longitud++]='o';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_p:
			if (longitud < 30) {
				nombre[longitud++]='p';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_q:
			if (longitud < 30) {
				nombre[longitud++]='q';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_r:
			if (longitud < 30) {
				nombre[longitud++]='r';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_s:
			if (longitud < 30) {
				nombre[longitud++]='s';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_t:
			if (longitud < 30) {
				nombre[longitud++]='t';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_u:
			if (longitud < 30) {
				nombre[longitud++]='u';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_v:
			if (longitud < 30) {
				nombre[longitud++]='v';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_w:
			if (longitud < 30) {
				nombre[longitud++]='w';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_x:
			if (longitud < 30) {
				nombre[longitud++]='x';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_y:
			if (longitud < 30) {
				nombre[longitud++]='y';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_z:
			if (longitud < 30) {
				nombre[longitud++]='z';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_0:
			if (longitud < 30) {
				nombre[longitud++]='0';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_1:
			if (longitud < 30) {
				nombre[longitud++]='1';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_2:
			if (longitud < 30) {
				nombre[longitud++]='2';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_3:
			if (longitud < 30) {
				nombre[longitud++]='3';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_4:
			if (longitud < 30) {
				nombre[longitud++]='4';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_5:
			if (longitud < 30) {
				nombre[longitud++]='5';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_6:
			if (longitud < 30) {
				nombre[longitud++]='6';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_7:
			if (longitud < 30) {
				nombre[longitud++]='7';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_8:
			if (longitud < 30) {
				nombre[longitud++]='8';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_9:
			if (longitud < 30) {
				nombre[longitud++]='9';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_MINUS:
			if (longitud < 30) {
				nombre[longitud++]='-';
				nombre[longitud]=127;
				nombre[longitud + 1]=0;
			}
		break;
		}
	} while (!retorno);

	nombre[longitud]=0; // erase cursor

	longitud=strlen(path_snaps);
	if((path_snaps[longitud-1]!='/')&&(longitud>1))
		sprintf(nombre_final,"%s/%s.%s",path_snaps,nombre,extension); // name
	else
		sprintf(nombre_final,"%s%s.%s",path_snaps,nombre,extension);

	return (retorno);
}




int ask_value(int *final_value,int y_coord,int max_value) {

	unsigned char nombre2[50];
	unsigned char *videomem;
	int ancho,value,tmp,retorno;

	videomem=screen->pixels;
	ancho=screen->w;

	retorno=0;
	value=0;
	do {
		sprintf (nombre2, " %d\177 ", value);
		print_string (videomem, nombre2, -1, y_coord, 15, 0, ancho);
		switch (wait_key ()) {
		case SDLK_BACKSPACE:
			value/=10;
		break;
		case SDLK_ESCAPE:
			retorno=2;
		break;
		case SDLK_RETURN:
			retorno=1;
		break;
		case SDLK_0:
			tmp=value * 10;
			if (tmp <= max_value) {
				value=tmp;
			}
		break;
		case SDLK_1:
			tmp=1+value * 10;
			if (tmp <= max_value) {
				value=tmp;
			}
		break;
		case SDLK_2:
			tmp=2+value * 10;
			if (tmp <= max_value) {
				value=tmp;
			}
		break;
		case SDLK_3:
			tmp=3+value * 10;
			if (tmp <= max_value) {
				value=tmp;
			}
		break;
		case SDLK_4:
			tmp=4+value * 10;
			if (tmp <= max_value) {
				value=tmp;
			}
		break;
		case SDLK_5:
			tmp=5+value * 10;
			if (tmp <= max_value) {
				value=tmp;
			}
		break;
		case SDLK_6:
			tmp=6+value * 10;
			if (tmp <= max_value) {
				value=tmp;
			}
		break;
		case SDLK_7:
			tmp=7+value * 10;
			if (tmp <= max_value) {
				value=tmp;
			}
		break;
		case SDLK_8:
			tmp=8+value * 10;
			if (tmp <= max_value) {
				value=tmp;
			}
		break;
		case SDLK_9:
			tmp=9+value * 10;
			if (tmp <= max_value) {
				value=tmp;
			}
		break;
		}
	} while (!retorno);

	*final_value=value;

	return (retorno);
}




// shows a menu to allow user to save a snapshot file

void save_z80file() {

	unsigned char *videomem;
	int ancho,retorno;
	unsigned char nombre2[1024];

	videomem=screen->pixels;
	ancho=screen->w;

	clean_screen();

	print_string(videomem,"Choose a name for the Z80 snapshot",-1,32,14,0,ancho);
	print_string(videomem,"(up to 30 characters)",-1,52,14,0,ancho);

	print_string(videomem,"Snapshot will be saved in:",-1,132,12,0,ancho);
	print_string(videomem,path_snaps,0,152,12,0,ancho);


	retorno=ask_filename(nombre2,84,"z80");

	clean_screen();

	if(retorno==2) // abort
		return;

	retorno=save_z80(nombre2);
	switch(retorno) {
	case 0:
		break;
	case -1:
		print_string(videomem,"File already exists",-1,80,10,0,ancho);
		wait_key();
		break;
	case -2:
		print_string(videomem,"Can't create file",-1,80,10,0,ancho);
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

	print_string(videomem,"Choose the Z80 snapshot file to load",-1,32,13,0,ancho);

	filename=select_file(path_snaps,FILETYPE_Z80);

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
		print_string(videomem,"Error: Can't load that file",-1,232,10,0,ancho);
		print_string(videomem,"Press any key",-1,248,10,0,ancho);
		wait_key();
		break;
	case -2:
	case -3:
		print_string(videomem,"Error: unsuported snap file",-1,232,10,0,ancho);
		print_string(videomem,"Press any key",-1,248,10,0,ancho);
		wait_key();
		break;
	}
	clean_screen();
}


void load_scrfile() {


	unsigned char *videomem,*filename,value;
	int ancho,retorno,loop;
	FILE *fichero;
	unsigned char paleta_tmp[64];

	videomem=screen->pixels;
	ancho=screen->w;

	clean_screen();

	print_string(videomem,"Choose the SCR snapshot file to load",-1,32,13,0,ancho);

	filename=select_file(path_snaps,FILETYPE_SCR);

	if(filename==NULL) { // Aborted
		clean_screen();
		return;
	}

	ordenador.osd_text[0]=0;
	fichero=fopen(filename,"rb");
	retorno=0;
	if (!fichero) {
		retorno=-1;
	} else {
		for(loop=0;loop<6912;loop++) {
			if (1==fread(&value,1,1,fichero)) {
				*(ordenador.block1 + 0x04000 + loop) = value;
			} else {
				retorno=-1;
				break;
			}
		}
		if (1==fread(paleta_tmp,64,1,fichero)) {
			memcpy(ordenador.ulaplus_palete,paleta_tmp,64);
			ordenador.ulaplus=1;
		} else {
			ordenador.ulaplus=0;
		}
		fclose(fichero);
	}

	free(filename);
	clean_screen();

	switch(retorno) {
	case 0: // all right
		break;
	default:
		print_string(videomem,"Error: Can't load that file",-1,232,10,0,ancho);
		print_string(videomem,"Press any key",-1,248,10,0,ancho);
		wait_key();
	break;
	}
	clean_screen();
}



/* fills a FICHERO chained list with all the files and directories contained in PATH.
	 If KIND is 0, it returns only Snapshots, if is 1, it returns only TAPE files, and
	if is 2, it returns only MDR files */

struct fichero *read_directory(char *cpath,enum LOAD_FILE_TYPES kind) {

	struct fichero *listhead,*listend;
	struct dirent *entry;
	DIR *directory;
	struct stat estado;
	unsigned char path[2049],fichero[2049],extension[5],found;
	int bucle,length;
	
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
				case FILETYPE_Z80:
					if((!strcasecmp(extension,".z80"))||(!strcasecmp(extension,".sna")))
						found=1; // is a .z80 or SNA file
				break;
				case FILETYPE_TAP_TZX:
					if((!strcasecmp(extension,".tap"))||(!strcasecmp(extension,".tzx")))
						found=1; // is a .tap file
				break;
				case FILETYPE_MDR:
					if(!strcasecmp(extension,".mdr"))
						found=1; // is a .mdr file
				break;
				case FILETYPE_SCR:
					if(!strcasecmp(extension,".scr"))
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

char *select_file(char *path,enum LOAD_FILE_TYPES kind) {

	struct fichero *filelist,*fl2;
	unsigned char fin,read,*salida;
	int bucle,ancho,numitems,selected,from,longitud;

	salida=(unsigned char *)malloc(2049);
	salida[0]=0;

	ancho=screen->w;

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
				ink1=15;
				ink2=0;
			break;
			case 1: // directory
				ink1=12;
				ink2=4;
			break;
			case 2: // parent directory
				ink1=10;
				ink2=2;
			break;
			}
			if(bucle==mark)
				print_string(videomem,namefile,-1,pos,ink2,15,ancho);
			else
				print_string(videomem,namefile,-1,pos,ink1,0,ancho);
			pos+=16;
		}
		if((pos+16)>460)
			break; // reached bottom part of the rectangle
		fl2=fl2->next;
	}
	while((pos+16<460)) {
		print_string(videomem,spaces,-1,pos,0,0,ancho);
		pos+=16;
	}
}
