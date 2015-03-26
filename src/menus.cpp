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
#include <dirent.h>
#include <string.h>
#include <iostream>
#include <SDL/SDL.h>

#include "z80free/Z80free.h"
#include "cargador.hh"
#include "computer.hh"
#include "emulator.hh"
#include "tape.hh"
#include "menus.hh"
#include "llscreen.hh"

// shows the settings menu

void print_copy() {

	llscreen->print_string("(C) 2012 Fabio Olimpieri",-1,-1,13,0);
	llscreen->print_string("(C) 2003-2015 Raster Software Vigo",-1,-2,13,0);
}

// shows the help menu

void help_menu() {

	unsigned char fin;

	llscreen->clear_screen();

	llscreen->print_string("FBZX (2.12.0)",-1,1,15,0);
	llscreen->print_string("Available keys",-1,2,14,0);
	llscreen->print_string("Shift:Caps Shift        Ctrl:Symbol Shift",-1,4,11,0);

	llscreen->print_string("F1: \001\017this help",14,6,12,0);

	llscreen->print_string("F2: \001\017manage snapshots",336,6,12,0);

	llscreen->print_string("F3: \001\017manage TAP/TZX",14,8,12,0);

	llscreen->print_string("F4: \001\017change settings",336,8,12,0);

	llscreen->print_string("F5: \001\017stop TAPE",14,10,12,0);

	llscreen->print_string("F6: \001\017play TAPE",336,10,12,0);

	llscreen->print_string("F7: \001\017manage MICRODRIVE",14,12,12,0);

	llscreen->print_string("F8: \001\017tools",336,12,12,0);

	llscreen->print_string("F9: \001\017toggle fullscreen",14,14,12,0);

	llscreen->print_string("F10: \001\017reset spectrum",336,14,12,0);

	llscreen->print_string("F11/O: \001\017volume low",14,16,12,0);

	llscreen->print_string("F12/P: \001\017volume up",336,16,12,0);

	llscreen->print_string("ESC: \001\017exit emulator",184,18,12,0);

	print_copy();

	fin=1;
	do {
		unsigned int key = wait_key();
		switch(key) {
		case SDLK_ESCAPE: // to exit the help
			fin=0;
		break;
		default:
			if (launch_menu(key)) {
				fin=0;
			}
		break;
		}
	} while(fin);
	llscreen->clear_screen();
}



int launch_menu(unsigned int key_pressed) {

	int retval=0;
	switch(key_pressed) {

		case SDLK_F2:
			snapshots_menu ();	// manage snapshot files
			retval=1;
			break;

		case SDLK_F3:
			taps_menu ();	// manage TAP files
			retval=1;
			break;

		case SDLK_F4:	// settings
			settings_menu ();
			retval=1;
			break;

		case SDLK_F7:
			microdrive_menu ();	// shows the microdrive menu
			retval=1;
			break;

		case SDLK_F8:
			tools_menu();
			retval=1;
			break;
	}
	return (retval);
}

void settings_menu() {

	unsigned char fin;
	char texto[41];

	fin=1;

	texto[0]=0;

	do {
		llscreen->clear_screen();

		llscreen->print_string("Current settings",-1,0,15,0);
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
  
		llscreen->print_string(texto,-1,2,14,0);

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
		llscreen->print_string(texto,-1,3,13,0);

		if(ordenador.ay_emul)
			sprintf(texto,"AY-3-8912 Emulation: enabled");
		else
			sprintf(texto,"AY-3-8912 Emulation: disabled");

		llscreen->print_string(texto,-1,4,11,0);

		if(ordenador.mdr_active)
			sprintf(texto,"Interface I Emulation: enabled");
		else
			sprintf(texto,"Interface I Emulation: disabled");

		llscreen->print_string(texto,-1,5,15,0);

		if(ordenador.dblscan)
			sprintf(texto,"Double scan: enabled");
		else
			sprintf(texto,"Double scan: disabled");

		llscreen->print_string(texto,-1,6,12,0);

		if(ordenador.turbo)
			sprintf(texto,"TURBO mode: enabled");
		else
			sprintf(texto,"TURBO mode: disabled");
		llscreen->print_string(texto,-1,7,14,0);

		if (ordenador.bw) {
			llscreen->print_string("TV Set: \001\011B\001\012&\001\014W",-1,8,15,0);
		} else {
			llscreen->print_string("TV Set: \001\012C\001\014o\001\015l\001\016o\001\013r",-1,8,15,0);
		}

		llscreen->print_string("1: \001\01748K issue2",30,10,12,0);

		llscreen->print_string("2: \001\01748K issue3",213,10,12,0);

		llscreen->print_string("3: \001\017Sinclair 128K",426,10,12,0);

		llscreen->print_string("4: \001\017Amstrad +2",30,12,12,0);

		llscreen->print_string("5: \001\017Amstrad +2A/+3",213,12,12,0);

		llscreen->print_string("6: \001\017Spanish 128K",426,12,12,0);

		llscreen->print_string("7: \001\017Cursor",30,14,12,0);

		llscreen->print_string("8: \001\017Kempston",213,14,12,0);

		llscreen->print_string("9: \001\017Sinclair (1)",426,14,12,0);

		llscreen->print_string("0: \001\017Sinclair (2)",30,16,12,0);

		llscreen->print_string("I: \001\017Interface I",213,16,12,0);

		llscreen->print_string("A: \001\017AY emulation",426,16,12,0);

		llscreen->print_string("T: \001\017TURBO mode",30,18,12,0);

		llscreen->print_string("D: \001\017Double Scan",213,18,12,0);

		llscreen->print_string("V: \001\017TV Set mode",426,18,12,0);

		llscreen->print_string("ESC: \001\017return to emulator",-1,22,12,0);

		print_copy();

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
			llscreen->set_paletes(ordenador.bw);
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

	llscreen->clear_screen();
}


// shows the POKE menu

void do_poke() {

	char string[80];
	int retorno,address,old_value,new_value;

	llscreen->clear_screen();

	while(1) {
		llscreen->print_string("Type address to POKE",-1,2,15,0);
		llscreen->print_string("(ESC to exit)",-1,3,12,0);

		retorno=ask_value(&address,5,65535);

		llscreen->clear_screen();

		if (retorno==2) {
			return;
		}

		if ((address<16384) && ((ordenador.mode128k != 3) || (1 != (ordenador.mport2 & 0x01)))) {
			llscreen->print_string("That address is ROM memory.",-1,1,15,0);
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

		llscreen->print_string("Type new value to POKE",-1,2,15,0);
		llscreen->print_string("(ESC to cancel)",-1,3,12,0);
		sprintf(string,"Address: %d; old value: %d\n",address,old_value);
		llscreen->print_string(string,-1,8,14,0);

		retorno=ask_value(&new_value,5,255);

		llscreen->clear_screen();

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
		llscreen->print_string(string,-1,8,14,0);

	}
}

// shows the tools menu

void tools_menu() {

	unsigned char fin;

	fin=1;
	do {
		llscreen->clear_screen();

		llscreen->print_string("Tools",-1,1,15,0);

		llscreen->print_string("1: \001\017show keyboard template",14,3,12,0);
		llscreen->print_string("",62,50,15,0);

		llscreen->print_string("2: \001\017insert POKEs",14,5,12,0);

		llscreen->print_string("ESC: \001\017return to emulator",14,12,12,0);

		print_copy();

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

	llscreen->clear_screen();
}




// shows the SNAPSHOTS menu

void snapshots_menu() {

	unsigned char fin;

	llscreen->clear_screen();

	llscreen->print_string("SNAPSHOTS",-1,1,15,0);

	llscreen->print_string("1: \001\017load a Z80/SNA snapshot",14,4,12,0);

	if(ordenador.mode128k!=3) { // not in +3 mode
		llscreen->print_string("2: \001\017make a Z80 snapshot",14,6,12,0);
	} else {
		llscreen->print_string("Can't make snapshots in +3 mode",14,6,15,0);
	}

	llscreen->print_string("3: \001\017load a SCR snapshot",14,8,12,0);

	llscreen->print_string("4: \001\017save a SCR snapshot",14,10,12,0);

	llscreen->print_string("ESC: \001\017return to emulator",-1,14,12,0);

	print_copy();

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
	llscreen->clear_screen();
}


// shows the TAPs menu

void taps_menu() {

	unsigned char fin;

	fin=1;
	do {
		llscreen->clear_screen();

		llscreen->print_string("TAP/TZX files",-1,1,15,0);

		llscreen->print_string("1: \001\017select a TAP/TZX file",14,4,12,0);

		llscreen->print_string("2: \001\017rewind TAP/TZX file",14,6,12,0);

		llscreen->print_string("3: \001\017fast/normal speed",14,8,12,0);

		llscreen->print_string("4: \001\017write protection",14,10,12,0);

		llscreen->print_string("5: \001\017create empty TAP file",14,12,12,0);

		llscreen->print_string("ESC: \001\017return to emulator",14,16,12,0);

		llscreen->print_string("Current TAP/TZX file is:",-1,19,12,0);
		llscreen->print_string(ordenador.current_tap,-1,20,15,0);

		print_copy();

		if(ordenador.tape_fast_load)
			llscreen->print_string("Fast load enabled",10,-4,14,0);
		else
			llscreen->print_string("Fast load disabled",10,-4,14,0);

		if(ordenador.tape_write)
			llscreen->print_string("Write enabled",390,-4,14,0);
		else
			llscreen->print_string("Write disabled",390,-4,14,0);

		switch(wait_key()) {
		case SDLK_ESCAPE: // to exit the help
			fin=0;
		break;
		case SDLK_1:
			ordenador.OOTape.set_pause(true);
			select_tapfile();
			ordenador.OOTape.rewind();
		break;
		case SDLK_2:
			fin=0;
			ordenador.OOTape.set_pause(true);
			ordenador.OOTape.rewind();
			ordenador.osd_text = "Tape rewinded";
			ordenador.osd_time=50;
		break;
		case SDLK_3:
			ordenador.OOTape.set_pause(true);
			ordenador.tape_fast_load = ordenador.tape_fast_load ? false : true;
		break;
		case SDLK_4:
			ordenador.tape_write = ordenador.tape_write ? false : true;
		break;
		case SDLK_5:
			create_tapfile();
		break;
		default:
		break;
		}

	} while(fin);

	llscreen->clear_screen();
}

// shows a menu to allow user to choose a tape file

void select_tapfile() {


	char *filename;
	int retorno;

	llscreen->clear_screen();

	filename=select_file("Choose the TAPE file to load",path_taps,FILETYPE_TAP_TZX);

	if(filename==NULL) { // Aborted
		llscreen->clear_screen();
		return;
	}

	ordenador.tape_write = false; // by default, can't record
	ordenador.current_tap = filename;
	free(filename);

	if (ordenador.OOTape.load_file(ordenador.current_tap)) {
		llscreen->print_string("Error: Can't load that file",-1,-4,10,0);
		llscreen->print_string("Press any key",-1,-3,10,0);
		ordenador.current_tap = "";
		wait_key();
	}

	llscreen->clear_screen();
}

void create_tapfile() {

	int retorno;
	char nombre2[1024];
	struct stat tmpstat;

	llscreen->clear_screen();

	llscreen->print_string("Choose a name for the TAP file",-1,2,14,0);
	llscreen->print_string("(up to 30 characters)",-1,3,14,0);

	llscreen->print_string("TAP file will be saved in:",-1,19,12,0);
	llscreen->print_string(path_taps,0,20,12,0);


	retorno=ask_filename(nombre2,5,"tap");

	llscreen->clear_screen();

	if(retorno==2) // abort
		return;

	if (-1 == stat(nombre2,&tmpstat)) {
		// File not exists. Good.
		FILE *tmp = fopen(nombre2,"a+"); // create for read and write
		if(tmp == NULL) {
			retorno = -2;
		} else {
			fclose(tmp);
			retorno = 0;
		}
	} else {
		retorno = -1;
	}

	switch(retorno) {
	case 0:
		ordenador.tape_write = true; // allow to write
		ordenador.current_tap = nombre2;
	break;
	case -1:
		llscreen->print_string("File already exists",-1,5,10,0);
		ordenador.current_tap = "";
		ordenador.tape_write = false;
		wait_key();
	break;
	case -2:
		llscreen->print_string("Can't create file",-1,5,10,0);
		ordenador.current_tap = "";
		ordenador.tape_write = false;
		wait_key();
	break;
	}
	llscreen->clear_screen();
}

// shows the microdrive menu

void microdrive_menu() {

	unsigned char fin;

	fin=1;
	do {
			llscreen->clear_screen();

		llscreen->print_string("MICRODRIVE files",-1,1,15,0);

		llscreen->print_string("1: \001\017select a MDR file",14,3,12,0);

		llscreen->print_string("2: \001\017create a MDR file",14,5,12,0);

		llscreen->print_string("3: \001\017write protection",14,7,12,0);

		llscreen->print_string("ESC: \001\017return to emulator",14,10,12,0);

		llscreen->print_string("Current MDR file is:",-1,13,12,0);
		llscreen->print_string(ordenador.mdr_current_mdr,-1,14,12,0);

		print_copy();

		if(!ordenador.mdr_cartridge[137922])
			llscreen->print_string("Write enabled",-1,-4,14,0);
		else
			llscreen->print_string("Write disabled",-1,-4,14,0);

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
				fwrite(ordenador.mdr_cartridge,137923,1,ordenador.mdr_file); // save cartridge
				fclose(ordenador.mdr_file);
				ordenador.mdr_file=NULL;
				ordenador.mdr_modified=0;
			}
		break;
		default:
		break;
		}

	} while(fin);

	llscreen->clear_screen();
}

// shows a menu to allow user to choose a microdrive file

void select_mdrfile() {


	char *filename;
	int retorno;

	llscreen->clear_screen();

	filename=select_file("Choose the MICRODRIVE file to load",path_mdrs,FILETYPE_MDR); // MDR files

	if(filename==NULL) { // Aborted
		llscreen->clear_screen();
		return;
	}

	ordenador.mdr_file=fopen(filename,"rb"); // read
	if(ordenador.mdr_file==NULL)
		retorno=-1;
	else {
		retorno=0;
		fread(ordenador.mdr_cartridge,137923,1,ordenador.mdr_file); // read the cartridge in memory
		ordenador.mdr_modified=0; // not modified
		fclose(ordenador.mdr_file);
		ordenador.mdr_tapehead=0;
	}

	llscreen->clear_screen();

	strcpy(ordenador.mdr_current_mdr,filename);

	free(filename);

	switch(retorno) {
	case 0: // all right
		break;
	case -1:
		llscreen->print_string("Error: Can't load that file",-1,-3,10,0);
		llscreen->print_string("Press any key",-1,-2,10,0);
		ordenador.mdr_current_mdr[0]=0;
		wait_key();
		break;
	}

	llscreen->clear_screen();
}

void create_mdrfile() {


	int retorno,bucle;
	char nombre2[1024];

	llscreen->clear_screen();

	llscreen->print_string("Choose a name for the MDR file",-1,4,14,0);
	llscreen->print_string("(up to 30 characters)",-1,5,14,0);

	llscreen->print_string("MDR file will be saved in:",-1,10,12,0);
	llscreen->print_string(path_mdrs,0,11,12,0);

	retorno=ask_filename(nombre2,7,"mdr");

	llscreen->clear_screen();

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
			fwrite(ordenador.mdr_cartridge,137923,1,ordenador.mdr_file); // save cartridge
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
		llscreen->print_string("File already exists",-1,9,10,0);
		ordenador.mdr_current_mdr[0]=0;
		wait_key();
	break;
	case -2:
		llscreen->print_string("Can't create file",-1,9,10,0);
		ordenador.mdr_current_mdr[0]=0;
		wait_key();
	break;
	}
	llscreen->clear_screen();
}


void create_scrfile() {


	int retorno;
	char nombre2[1024];
	FILE *fichero;

	llscreen->clear_screen();

	llscreen->print_string("Choose a name for the SCR file",-1,4,14,0);
	llscreen->print_string("(up to 30 characters)",-1,5,14,0);

	llscreen->print_string("SCR file will be saved in:",-1,10,12,0);
	llscreen->print_string(path_mdrs,0,11,12,0);

	retorno=ask_filename(nombre2,7,"scr");

	llscreen->clear_screen();

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
			fwrite(ordenador.block1+0x04000,6912,1,fichero); // save screen
			if (ordenador.ulaplus!=0) {
				fwrite(ordenador.ulaplus_palete,64,1,fichero); // save ULAPlus palete
			}
			fclose(fichero);
			retorno=0;
		}
	}

	switch(retorno) {
	case -1:
		llscreen->print_string("File already exists",-1,9,10,0);
		wait_key();
	break;
	case -2:
		llscreen->print_string("Can't create file",-1,9,10,0);
		wait_key();
	break;
	default:
	break;
	}
	llscreen->clear_screen();
}


int ask_filename(char *nombre_final,int y_coord,string extension) {

	int longitud,retorno;
	char nombre[37],nombre2[38];

	nombre[0] = CURSOR_ELEMENT;
	nombre[1]=0;
	longitud=0;
	retorno=0;

	do {
		sprintf (nombre2, " %s.%s ", nombre,extension.c_str());
		llscreen->print_string(nombre2, -1, y_coord, 15, 0);
		switch (wait_key ()) {
		case SDLK_BACKSPACE:
			if (longitud > 0) {
				nombre[longitud] = 0;
				longitud--;
				nombre[longitud] = CURSOR_ELEMENT;
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
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_b:
			if (longitud < 30) {
				nombre[longitud++]='b';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_c:
			if (longitud < 30) {
				nombre[longitud++]='c';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_d:
			if (longitud < 30) {
				nombre[longitud++]='d';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_e:
			if (longitud < 30) {
				nombre[longitud++]='e';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_f:
			if (longitud < 30) {
				nombre[longitud++]='f';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_g:
			if (longitud < 30) {
				nombre[longitud++]='g';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_h:
			if (longitud < 30) {
				nombre[longitud++]='h';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_i:
			if (longitud < 30) {
				nombre[longitud++]='i';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_j:
			if (longitud < 30) {
				nombre[longitud++]='j';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_k:
			if (longitud < 30) {
				nombre[longitud++]='k';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_l:
			if (longitud < 30) {
				nombre[longitud++]='l';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_m:
			if (longitud < 30) {
				nombre[longitud++]='m';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_n:
			if (longitud < 30) {
				nombre[longitud++]='n';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_o:
			if (longitud < 30) {
				nombre[longitud++]='o';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_p:
			if (longitud < 30) {
				nombre[longitud++]='p';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_q:
			if (longitud < 30) {
				nombre[longitud++]='q';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_r:
			if (longitud < 30) {
				nombre[longitud++]='r';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_s:
			if (longitud < 30) {
				nombre[longitud++]='s';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_t:
			if (longitud < 30) {
				nombre[longitud++]='t';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_u:
			if (longitud < 30) {
				nombre[longitud++]='u';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_v:
			if (longitud < 30) {
				nombre[longitud++]='v';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_w:
			if (longitud < 30) {
				nombre[longitud++]='w';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_x:
			if (longitud < 30) {
				nombre[longitud++]='x';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_y:
			if (longitud < 30) {
				nombre[longitud++]='y';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_z:
			if (longitud < 30) {
				nombre[longitud++]='z';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_0:
			if (longitud < 30) {
				nombre[longitud++]='0';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_1:
			if (longitud < 30) {
				nombre[longitud++]='1';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_2:
			if (longitud < 30) {
				nombre[longitud++]='2';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_3:
			if (longitud < 30) {
				nombre[longitud++]='3';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_4:
			if (longitud < 30) {
				nombre[longitud++]='4';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_5:
			if (longitud < 30) {
				nombre[longitud++]='5';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_6:
			if (longitud < 30) {
				nombre[longitud++]='6';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_7:
			if (longitud < 30) {
				nombre[longitud++]='7';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_8:
			if (longitud < 30) {
				nombre[longitud++]='8';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_9:
			if (longitud < 30) {
				nombre[longitud++]='9';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		case SDLK_MINUS:
			if (longitud < 30) {
				nombre[longitud++]='-';
				nombre[longitud] = CURSOR_ELEMENT;
				nombre[longitud + 1]=0;
			}
		break;
		}
	} while (!retorno);

	nombre[longitud]=0; // erase cursor

	longitud=strlen(path_snaps);
	if((path_snaps[longitud-1]!='/')&&(longitud>1))
		sprintf(nombre_final,"%s/%s.%s",path_snaps,nombre,extension.c_str()); // name
	else
		sprintf(nombre_final,"%s%s.%s",path_snaps,nombre,extension.c_str());

	return (retorno);
}


int ask_value(int *final_value,int y_coord,int max_value) {

	char nombre2[50];

	int value,tmp,retorno;

	retorno=0;
	value=0;
	do {
		sprintf (nombre2, " %d%c ", value,CURSOR_ELEMENT);
		llscreen->print_string(nombre2, -1, y_coord, 15, 0);
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


	int retorno;
	char nombre2[1024];

	llscreen->clear_screen();

	llscreen->print_string("Choose a name for the Z80 snapshot",-1,4,14,0);
	llscreen->print_string("(up to 30 characters)",-1,5,14,0);

	llscreen->print_string("Snapshot will be saved in:",-1,11,12,0);
	llscreen->print_string(path_snaps,0,11,12,0);


	retorno=ask_filename(nombre2,7,"z80");

	llscreen->clear_screen();

	if(retorno==2) // abort
		return;

	retorno=save_z80(nombre2);
	switch(retorno) {
	case 0:
		break;
	case -1:
		llscreen->print_string("File already exists",-1,9,10,0);
		wait_key();
		break;
	case -2:
		llscreen->print_string("Can't create file",-1,9,10,0);
		wait_key();
		break;
	}
	llscreen->clear_screen();
}

// shows a menu to allow user to load a snapshot file

void load_z80file() {



	char *filename;
	int retorno;

	llscreen->clear_screen();

	filename=select_file("Choose the Z80 snapshot file to load",path_snaps,FILETYPE_Z80);

	if(filename==NULL) { // Aborted
		llscreen->clear_screen();
		return;
	}

	retorno=load_z80(filename);
	free(filename);
	llscreen->clear_screen();

	switch(retorno) {
	case 0: // all right
		break;
	case -1:
		llscreen->print_string("Error: Can't load that file",-1,-3,10,0);
		llscreen->print_string("Press any key",-1,-2,10,0);
		wait_key();
		break;
	case -2:
	case -3:
		llscreen->print_string("Error: unsuported snap file",-1,-3,10,0);
		llscreen->print_string("Press any key",-1,-2,10,0);
		wait_key();
		break;
	}
	llscreen->clear_screen();
}


void load_scrfile() {


	unsigned char value;
	char *filename;
	int retorno,loop;
	FILE *fichero;
	unsigned char paleta_tmp[64];

	llscreen->clear_screen();

	filename=select_file("Choose the SCR snapshot file to load",path_snaps,FILETYPE_SCR);

	if(filename==NULL) { // Aborted
		llscreen->clear_screen();
		return;
	}

	ordenador.osd_text = "";
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
	llscreen->clear_screen();

	switch(retorno) {
	case 0: // all right
		break;
	default:
		llscreen->print_string("Error: Can't load that file",-1,-3,10,0);
		llscreen->print_string("Press any key",-1,-2,10,0);
		wait_key();
	break;
	}
	llscreen->clear_screen();
}



/* fills a FICHERO chained list with all the files and directories contained in PATH.
	 If KIND is 0, it returns only Snapshots, if is 1, it returns only TAPE files, and
	if is 2, it returns only MDR files */

class fichero *read_directory(char *cpath,enum LOAD_FILE_TYPES kind) {

	class fichero *listhead,*listend;
	struct dirent *entry;
	DIR *directory;
	struct stat estado;
	char path[2049],fichero[2049],extension[5],found;
	int bucle,length;

	strcpy(path,cpath);
	if('/'!=path[strlen(path)-1])
		strcat(path,"/"); // add the final / to the path

	listhead = new class fichero();
	listhead->nombre = "..";
	listhead->tipo = 2;
	listhead->next = NULL;
	listhead->prev = NULL;
	listend = listhead;

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

			if ( (found || (S_ISDIR(estado.st_mode))) && ('.' != entry->d_name[0])) { // is a directory. We must add it
				class fichero *new_file = new class fichero();
				class fichero *floop;
				new_file->nombrepath = fichero;
				new_file->nombre = entry->d_name;
				if(S_ISDIR(estado.st_mode)) {
					new_file->tipo=1; // a directory
				} else {
					new_file->tipo=0; // a file
				}
				for (floop = listhead; floop != NULL; floop = floop->next) {
					if (floop->tipo == 2) { // parent directory
						continue;
					}
					// if found a file, and the new entry is a folder, add it before the file
					if (((floop->tipo == 0) && (new_file->tipo == 1)) || ((floop->tipo == new_file->tipo) && (floop->nombre>new_file->nombre))){
						new_file->prev = floop->prev;
						new_file->next = floop;
						floop->prev = new_file;
						if (new_file->prev != NULL) {
							new_file->prev->next = new_file;
						}
						new_file = NULL;
						break;
					}
				}
				if (new_file != NULL) { // hasn't been added; append to the end
					listend->next = new_file;
					new_file->next = NULL;
					new_file->prev = listend;
					listend = new_file;
				}
			}
		}
	} while(entry!=NULL);
	closedir(directory);
	return(listhead);
}

// deletes a filelist tree, freeing the memory used by it

void delete_filelist(class fichero *filelist) {

	class fichero *fl1,*fl2;

	fl1=fl2=filelist;

	while(fl1!=NULL) {
		fl2=fl1->next;
		delete(fl1);
		fl1=fl2;
	}
}


/* allows user to choose a file from PATH. If KIND=0, only snapshots. If KIND=1, only
	 TAPE files */

char *select_file(string title, char *path,enum LOAD_FILE_TYPES kind) {

	class fichero *filelist,*fl2;
	unsigned char fin,read;
	char *salida;
	int bucle,numitems,selected,from,longitud;

	salida = (char*)malloc(4096);
	salida[0]=0;
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
			while(fl2 != NULL) { // counts the number of items
				fl2=fl2->next;
				numitems++;
			}
			selected=0;
			from=0;
		}

		llscreen->clear_screen();
		llscreen->print_string(title,-1,2,15,0);
		print_files(filelist,from,selected);

		switch(wait_key()) {
		case SDLK_ESCAPE: // to exit the help
			fin=0;
			delete_filelist(filelist);
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
				strcpy(salida,fl2->nombrepath.c_str());
				delete_filelist(filelist);
				return(salida); // ends returning the filename
			break;
			case 1: // change directory
				strcpy(path,fl2->nombrepath.c_str()); // new path_taps is namepath
				delete_filelist(filelist); // frees the memory
				read=1; // and redisplay all the files
			break;
			case 2: // upper directory
				delete_filelist(filelist); // frees the memory
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

	return(NULL);

}

void keyboard_menu() {

	llscreen->paint_picture("fbzx/keymap.bmp");
	print_copy();
	wait_key();
	llscreen->clear_screen();
}





// waits for a keystroke and returns its value

unsigned int wait_key() {

	char fin;
	unsigned int temporal_io=0;
	SDL_Event evento;

	fin=1;

	do {
		llscreen->do_flip();
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

void print_files(class fichero *filelist,int from,int mark) {

	class fichero *fl2;
	int bucle,numitems,pos;
	char ink1,ink2;
	string namefile;

	fl2=filelist;
	numitems=0;

	while(fl2!=NULL) { // counts the number of items
		fl2=fl2->next;
		numitems++;
	}

	ink1=ink2=0;

	fl2=filelist;
	pos=3;
	for(bucle=0;bucle<numitems;bucle++) {
		if(bucle>=from) {
			namefile = fl2->nombre;
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
			if(bucle==mark) {
				llscreen->print_string(namefile,10,pos,ink2,15);
			} else {
				llscreen->print_string(namefile,10,pos,ink1,0);
			}
			pos++;
		}
		if((pos+1)>llscreen->lines_in_screen)
			break; // reached bottom part of the rectangle
		fl2=fl2->next;
	}
}
