/*
 * Copyright 2003-2015 (C) Raster Software Vigo (Sergio Costas)
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

#include "computer.hh"

#include "z80free/Z80free.h"
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#include "emulator.hh"
#include "llscreen.hh"
#include "menus.hh"
#include "microdrive.hh"
#include "sound.hh"
#include "spk_ay.hh"
#include "tape.hh"

/* Returns the bus value when reading a port without a periferial */

byte bus_empty () {

	if (ordenador.mode128k != 3)
		return (ordenador.bus_value);
	else
		return (255);	// +2A and +3 returns always 255
}

/* calls all the routines that emulates the computer, runing them for 'tstados'
   tstates */

void emulate (int tstados) {

	if((procesador.I>=0x40)&&(procesador.I<=0x7F)) {
		ordenador.screen_snow=1;
	} else {
		ordenador.screen_snow=0;
	}
	show_screen (tstados);
	play_ay (tstados);
	play_sound (tstados);
	ordenador.OOTape.play(tstados);
	microdrive_emulate(tstados);

	if (!ordenador.OOTape.get_pause()) {
		if (ordenador.OOTape.read_signal() != 0) {
			ordenador.sound_bit = 1;
		} else {
			ordenador.sound_bit = 0;	// if not paused, asign SOUND_BIT the value of tape
		}
	}
}

void computer_init () {

	int bucle;

	ordenador.page48k = 0;
	ordenador.bus_counter = 0;
	ordenador.port254 = 0;
	ordenador.issue = 3;
	ordenador.mode128k = 0;
	ordenador.joystick = 0;

	ordenador.tape_fast_load = 1;	// fast load by default
	ordenador.tap_file = NULL;

	ordenador.osd_text[0] = 0;
	ordenador.osd_time = 0;

	ordenador.other_ret = 0;

	ordenador.s8 = ordenador.s9 = ordenador.s10 = ordenador.s11 =
	ordenador.s12 = ordenador.s13 = ordenador.s14 =
	ordenador.s15 = 0xFF;
	ordenador.tab_extended=0;
	ordenador.esc_again=0;

	ordenador.js = 0x00;

	for (bucle = 0; bucle < 16; bucle++)
		ordenador.ay_registers[bucle] = 0;
	ordenador.ay_emul = 0;
	ordenador.aych_a = 0;
	ordenador.aych_b = 0;
	ordenador.aych_c = 0;
	ordenador.aych_n = 0;
	ordenador.aych_envel = 0;
	ordenador.vol_a = 0;
	ordenador.vol_b = 0;
	ordenador.vol_c = 0;
	ordenador.tst_ay = 0;
	ordenador.tst_ay2 = 0;

	ordenador.ayval_a = 0;
	ordenador.ayval_b = 0;
	ordenador.ayval_c = 0;
	ordenador.ayval_n = 0;
	ordenador.ay_envel_value = 0;
	ordenador.ay_envel_way = 0;
}

/* Registers the screen surface where the Spectrum will put the picture,
prepares the palette and creates two arrays (translate and translate2)
that gives the memory address for each scan */

void register_screen () {

	//int resx,resy;
	int bucle, bucle2, bucle3, bucle4, bucle5;

	// we prepare the scanline transform arrays

	bucle5 = 0;
	for (bucle = 0; bucle < 3; bucle++)
		for (bucle2 = 0; bucle2 < 8; bucle2++)
			for (bucle3 = 0; bucle3 < 8; bucle3++)
				for (bucle4 = 0; bucle4 < 32; bucle4++) {
					ordenador.translate[bucle5] =
						147456 + bucle * 2048 +
						bucle2 * 32 + bucle3 * 256 +
						bucle4;
					ordenador.translate2[bucle5] =
						153600 + bucle * 256 +
						bucle2 * 32 + bucle4;
					bucle5++;
				}
	ordenador.tstados_counter = 0;

	ordenador.border = 0;
	ordenador.currline = 0;
	ordenador.currpix = 0;
	ordenador.flash = 0;

	//resx = ordenador.screen->w;
	//resy = ordenador.screen->h;

	switch (ordenador.zaurus_mini) {
	case 0:
		ordenador.init_line = 0;
		ordenador.next_line = 640;
		ordenador.next_scanline = 640;
		ordenador.first_line = 40;
		ordenador.last_line = 280;
		ordenador.first_pixel = 16;
		ordenador.last_pixel = 336;
		ordenador.next_pixel = 1;
		ordenador.jump_pixel = 16;
		break;
	case 1:
		ordenador.init_line = 65;
		ordenador.next_line = 160;
		ordenador.next_scanline = 160;
		ordenador.first_line = 40;
		ordenador.last_line = 280;
		ordenador.first_pixel = 0;
		ordenador.last_pixel = 351;
		ordenador.next_pixel = 1;
		ordenador.jump_pixel = 8;
		break;
	case 2:
		ordenador.init_line = 479;
		ordenador.next_line = -(307202);
		ordenador.next_scanline = -1;
		ordenador.first_line = 40;
		ordenador.last_line = 280;
		ordenador.first_pixel = 16;
		ordenador.last_pixel = 336;
		ordenador.next_pixel = 480;
		ordenador.jump_pixel = 7680;
		break;
	case 3:
		ordenador.init_line = 0;
		ordenador.next_line = 0;
		ordenador.next_scanline = 0;
		ordenador.first_line = 40;
		ordenador.last_line = 280;
		ordenador.first_pixel = 0;
		ordenador.last_pixel = 319;
		ordenador.next_pixel = 1;
		ordenador.jump_pixel = 4;
		break;
	}
	
	ordenador.next_line *= llscreen->bpp;
	ordenador.next_scanline *= llscreen->bpp;
	ordenador.init_line *= llscreen->bpp;
	ordenador.next_pixel *= llscreen->bpp;
	ordenador.jump_pixel *= llscreen->bpp;
	
	llscreen->set_paletes(ordenador.bw);

	ordenador.base_pixel = ((unsigned char *) (llscreen->llscreen->pixels));
	ordenador.pixel = ordenador.base_pixel + ordenador.init_line;
	ordenador.interr = 0;

	ordenador.p_translt = ordenador.translate;
	ordenador.p_translt2 = ordenador.translate2;

	ordenador.contador_flash = 0;
	ordenador.readed = 0;

	ordenador.contended_zone=0;
	ordenador.cicles_counter=0;
	
	ordenador.tstados_counter_sound = 0;
	ordenador.current_buffer = sound[0];
	ordenador.num_buff = 0;	// first buffer
	ordenador.sound_cuantity = 0;
	ordenador.sound_current_value = 0;
	ordenador.pixancho = 447;
	ordenador.pixalto = 311;	// values for 48K mode
}

void set_memory_pointers () {

	static unsigned int rom, ram;

	// assign the offset for video page

	if (ordenador.mport1 & 0x08)
		ordenador.video_offset = 32768;	// page 7
	else
		ordenador.video_offset = 0;	// page 5

	// assign ROMs and, if in special mode, RAM for the whole blocks

	if ((ordenador.mode128k == 3)) {
		if (ordenador.mport2 & 0x01) {		// +2A/+3 special mode
			ordenador.page48k = 0; // no 48K ROM paged in
			ram = (unsigned int) (ordenador.mport1 & 0x06);	// bits 1&2
			switch (ram) {
			case 0:
				ordenador.block0 = ordenador.memoria + 65536;
				ordenador.block1 = ordenador.memoria + 65536;
				ordenador.block2 = ordenador.memoria + 65536;
				ordenador.block3 = ordenador.memoria + 65536;
				break;
			case 2:
				ordenador.block0 = ordenador.memoria + 131072;
				ordenador.block1 = ordenador.memoria + 131072;
				ordenador.block2 = ordenador.memoria + 131072;
				ordenador.block3 = ordenador.memoria + 131072;
				break;
			case 4:
				ordenador.block0 = ordenador.memoria + 131072;
				ordenador.block1 = ordenador.memoria + 131072;
				ordenador.block2 = ordenador.memoria + 131072;
				ordenador.block3 = ordenador.memoria + 65536;
				break;
			case 6:
				ordenador.block0 = ordenador.memoria + 131072;
				ordenador.block1 = ordenador.memoria + 163840;
				ordenador.block2 = ordenador.memoria + 131072;
				ordenador.block3 = ordenador.memoria + 65536;
				break;
			default:
				printf("FullRAM desconocido\n");
			}
			return;
		} else {		// ROMs for +2A/+3 normal mode
			rom = 0;
			if (ordenador.mport1 & 0x10)
				rom++;
			if (ordenador.mport2 & 0x04)
				rom += 2;
			// assign the first block pointer to the right block bank
			ordenador.block0 = ordenador.memoria + (16384 * rom);
			ordenador.page48k = (rom==3) ? 1 : 0; // 48K ROM is in ROM page 3
		}
	} else {			// ROMs for 128K/+2 mode
		if (ordenador.mport1 & 0x10) {
			ordenador.block0 = ordenador.memoria + 16384;
			ordenador.page48k = 1;
		} else {
			ordenador.block0 = ordenador.memoria;
			ordenador.page48k = 0;
		}
	}

	// RAMs for 128K/+2 mode, and +2A/+3 in normal mode

	ordenador.block1 = ordenador.memoria + 131072;	// page 5 minus 16384
	ordenador.block2 = ordenador.memoria + 65536;	// page 2 minus 32768

	ram = 1 + ((unsigned int) (ordenador.mport1 & 0x07));	// RAM page for block3 plus 1
	ordenador.block3 = ordenador.memoria + (16384 * ram);	// page n minus 49152
}

/* Paints the spectrum screen during the TSTADOS tstates that the Z80 used
to execute last instruction */

void show_screen (int tstados) {

	static unsigned char temporal, ink, paper, fflash, tmp2;

	ordenador.tstados_counter += tstados;
	ordenador.cicles_counter += tstados;

	if (curr_frames!=jump_frames) {
		if (ordenador.tstados_counter>=69888) {
			ordenador.tstados_counter-=69888;
			ordenador.interr = 1;
			curr_frames++;
		}
		return;
	}
	
	fflash = 0; // flash flag
	while (ordenador.tstados_counter > 3) {
		ordenador.tstados_counter -= 4;

		// test if current pixel is for border or for user area

		if ((ordenador.currline < 64) || (ordenador.currline > 255)
			|| (ordenador.currpix < 48) || (ordenador.currpix > 303)) {
			
			// is border
				
			ordenador.contended_zone=0; // no contention here
			if (ordenador.ulaplus) {
				paint_pixels (255, ordenador.border+24, 0);	// paint 8 pixels with BORDER color
			} else {
				paint_pixels (255, ordenador.border, 0);	// paint 8 pixels with BORDER color
			}

			ordenador.bus_value = 255;

		} else {

			// is user area. We search for ink and paper colours

			ordenador.contended_zone=1; // can have contention
			
			temporal = ordenador.memoria[(*ordenador.p_translt2) + ordenador.video_offset];	// attributes
			ordenador.bus_value = temporal;
			ink = temporal & 0x07;	// ink colour
			paper = (temporal >> 3) & 0x07;	// paper colour
			if (ordenador.ulaplus) {
				tmp2=0x10+((temporal>>2)&0x30);
				ink+=tmp2;
				paper+=8+tmp2;
			} else {
				if (temporal & 0x40) {	// bright flag?
					ink += 8;
					paper += 8;
				}
				fflash = temporal & 0x80;	// flash flag
			}

			// Snow Effect

			if(ordenador.screen_snow) {
				temporal = ordenador.memoria[(((*ordenador.p_translt) + (ordenador.video_offset))&0xFFFFFF00)+(procesador.R)];	// data with snow
				ordenador.screen_snow=0; // no more snow for now
			} else
				temporal = ordenador.memoria[(*ordenador.p_translt) + ordenador.video_offset];	// data
			
			ordenador.p_translt++;
			ordenador.p_translt2++;
			if ((fflash) && (ordenador.flash))
				paint_pixels (temporal, paper, ink);	// if FLASH, invert PAPER and INK
			else
				paint_pixels (temporal, ink, paper);
		}
		ordenador.currpix += 8;
		if (ordenador.currpix > ordenador.pixancho) {
			ordenador.currpix = 0;
			ordenador.currline++;
			if (ordenador.currline > ordenador.first_line) {	// ordenador.first_line)
				ordenador.pixel += ordenador.next_line;	// ordenador.next_line;
			}
		}
		
		if ((ordenador.currline > ordenador.pixalto)&&(ordenador.currpix>=64)) {
			ordenador.currpix=64;
			if (ordenador.osd_time) {
				ordenador.osd_time--;
				if (ordenador.osd_time==0) {
					ordenador.tab_extended=0;
					ordenador.esc_again=0;
				}
					
				if (ordenador.osd_time) {
					llscreen->print_string(ordenador.osd_text, -1,-1, 12, 0);
				}
			}
				
			llscreen->do_flip();
			
			curr_frames=0;
			ordenador.currline = 0;
			ordenador.interr = 1;
			ordenador.cicles_counter=0;
			ordenador.pixel = ordenador.base_pixel+ordenador.init_line;
			ordenador.p_translt = ordenador.translate;
			ordenador.p_translt2 = ordenador.translate2;
			ordenador.contador_flash++;
			if (ordenador.contador_flash == 16) {
				ordenador.flash = 1 - ordenador.flash;
				ordenador.contador_flash = 0;
			}
		}
	}
}


/* PAINT_PIXELS paints one byte with INK color for 1 bits and PAPER color
for 0 bits, and increment acordingly the pointer PIXEL */

void paint_pixels (unsigned char octet,unsigned char ink, unsigned char paper) {

	static int bucle,valor;
	static unsigned int *p;
	static unsigned char mask;

	if ((ordenador.currpix < 16) || (ordenador.currpix >= 336)
	    || (ordenador.currline < 40) || (ordenador.currline >= 280))
		return;

	mask = 0x80;
	for (bucle = 0; bucle < 8; bucle++) {
		valor = (octet & mask) ? (int) ink : (int) paper;
		p=(colors+valor);
		llscreen->paint_one_pixel((unsigned char *)p,ordenador.pixel);
		if ((ordenador.zaurus_mini!=1)&&(ordenador.zaurus_mini!=3)) {
			if (ordenador.dblscan) {
				llscreen->paint_one_pixel((unsigned char *)p,ordenador.pixel+ordenador.next_scanline);
			} else {
				llscreen->paint_one_pixel((unsigned char *)colors,ordenador.pixel+ordenador.next_scanline);
			}
		}
		ordenador.pixel+=ordenador.next_pixel;
		if ((ordenador.zaurus_mini!=1)&&(ordenador.zaurus_mini!=3)) {
			llscreen->paint_one_pixel((unsigned char *)p,ordenador.pixel);
			if (ordenador.dblscan) {
				llscreen->paint_one_pixel((unsigned char *)p,ordenador.pixel+ordenador.next_scanline);
			} else {
				llscreen->paint_one_pixel((unsigned char *)colors,ordenador.pixel+ordenador.next_scanline);
			}
			ordenador.pixel+=ordenador.next_pixel;
		}
		mask = ((mask >> 1) & 0x7F);
	}
}

// Read the keyboard and stores the flags

void read_keyboard (SDL_Event *pevento2) {

	unsigned int temporal_io;
	SDL_Event evento,evento2,*pevento;
	Sint16 valor;
	Uint8 eje;

	if (pevento2==NULL) {
		pevento=&evento;
		if (!SDL_PollEvent (&evento))
			return;
	} else {
		pevento=pevento2;
	}

	if (pevento->type==SDL_QUIT) {
		salir = 0;
		return;
	}

	if (pevento->type==SDL_JOYBUTTONDOWN) {
		pevento->type=SDL_KEYDOWN;
		pevento->key.keysym.sym=SDLK_MENU; // emulate pressing the MENU key
	}
	
	if (pevento->type==SDL_JOYBUTTONUP) {
		pevento->type=SDL_KEYUP;
		pevento->key.keysym.sym=SDLK_MENU; // emulate depressing the MENU key
	}
	
	if (pevento->type==SDL_JOYAXISMOTION) {
		eje=pevento->jaxis.axis;
		valor=pevento->jaxis.value;

		evento2.type=SDL_KEYUP;
		if ((valor<16384)&&(valor>-16384)) { // JoyStick centered
			pevento->type=SDL_KEYUP;
			if (eje==1) {
				evento2.key.keysym.sym=SDLK_DOWN;
				pevento->key.keysym.sym=SDLK_UP; // pull up both keys
				read_keyboard(&evento2);
			}
			if (eje==0) {
				evento2.key.keysym.sym=SDLK_LEFT;
				pevento->key.keysym.sym=SDLK_RIGHT;
				read_keyboard(&evento2);
			}
		} else { // JoyStick moved
			if (eje==0) {
				if (valor>=0) {
					evento2.key.keysym.sym=SDLK_LEFT; // pull up LEFT
					read_keyboard(&evento2);
					pevento->key.keysym.sym=SDLK_RIGHT; // and press RIGHT
				} else {
					evento2.key.keysym.sym=SDLK_RIGHT; // pull up RIGHT
					read_keyboard(&evento2);
					pevento->key.keysym.sym=SDLK_LEFT; // and press LEFT
				}
			}
			if (eje==1) {
				if (valor<0) {
					evento2.key.keysym.sym=SDLK_DOWN; // pull up DOWN
					pevento->key.keysym.sym=SDLK_UP; // and press UP
					read_keyboard(&evento2);
				} else {
					evento2.key.keysym.sym=SDLK_UP; // pull up UP
					pevento->key.keysym.sym=SDLK_DOWN; // and press DOWN
					read_keyboard(&evento2);
				}
			}
			pevento->type=SDL_KEYDOWN;
		}
	}

	if ((pevento->type != SDL_KEYDOWN) && (pevento->type != SDL_KEYUP))
		return;

	ordenador.k8 = ordenador.k9 = ordenador.k10 = ordenador.k11 =
		ordenador.k12 = ordenador.k13 = ordenador.k14 =
		ordenador.k15 = 0;
		ordenador.jk = 0;

	temporal_io = (unsigned int) pevento->key.keysym.sym;

	if ((pevento->type==SDL_KEYUP)&&(temporal_io==SDLK_TAB)) {
		if (ordenador.tab_extended==0) {
			ordenador.tab_extended=1;
			strcpy(ordenador.osd_text,"Function Key mode on");
			ordenador.osd_time=100;
			return;
		} else {
			ordenador.tab_extended=0;
			ordenador.osd_time=0;
			return;
		}
	}
	
	if ((pevento->type==SDL_KEYDOWN)&&(ordenador.tab_extended==1))
		return;
	
	if ((pevento->type==SDL_KEYUP)&&(ordenador.tab_extended==1)) {
		ordenador.tab_extended=0;
		ordenador.osd_time=0;
		switch(temporal_io) {
		case SDLK_1:
			temporal_io=SDLK_F1;
		break;
		case SDLK_2:
			temporal_io=SDLK_F2;
		break;
		case SDLK_3:
			temporal_io=SDLK_F3;
		break;
		case SDLK_4:
			temporal_io=SDLK_F4;
		break;
		case SDLK_5:
			temporal_io=SDLK_F5;
		break;
		case SDLK_6:
			temporal_io=SDLK_F6;
		break;
		case SDLK_7:
			temporal_io=SDLK_F7;
		break;
		case SDLK_8:
			temporal_io=SDLK_F8;
		break;
		case SDLK_9:
			temporal_io=SDLK_F9;
		break;
		case SDLK_0:
			temporal_io=SDLK_F10;
		break;
		case SDLK_o:
			temporal_io=SDLK_F11;
		break;
		case SDLK_p:
			temporal_io=SDLK_F12;
		break;
		}
	}
	

	if (pevento->type == SDL_KEYUP)
		switch (temporal_io) {
		case SDLK_ESCAPE:	// to exit from the emulator
			if (ordenador.esc_again==0) {
				ordenador.esc_again=1;
				strcpy(ordenador.osd_text,"ESC again to exit");
				ordenador.osd_time=100;
			} else
				salir = 0;
			return;
			break;
		case SDLK_F1:
			help_menu ();	// shows the help menu
			break;

		case SDLK_F2:
		case SDLK_F3:
		case SDLK_F4:
		case SDLK_F7:
		case SDLK_F8:
			launch_menu(temporal_io);
			break;

		case SDLK_F5:   // STOP tape
			ordenador.OOTape.set_pause(true);
			break;

		case SDLK_F6:	// PLAY tape
			ordenador.OOTape.set_pause(false);
			break;		

		case SDLK_F9:
			llscreen->fullscreen_switch();
			break;

		case SDLK_F10:	// Reset emulator
			ResetComputer ();
			ordenador.OOTape.set_pause(true);
			ordenador.OOTape.rewind();
		break;

		case SDLK_F11:	// lower volume
			if (ordenador.volume > 3)
				set_volume (ordenador.volume - 4);
			sprintf (ordenador.osd_text, " Volume: %d ",ordenador.volume / 4);
			ordenador.osd_time = 50;
		break;
			
		case SDLK_F12:	// upper volume
			set_volume (ordenador.volume + 4);
			sprintf (ordenador.osd_text, " Volume: %d ",ordenador.volume / 4);
			ordenador.osd_time = 50;
			break;
		}

	// reorder joystick if screen is rotated
		
	if(ordenador.zaurus_mini==2) {
		switch(temporal_io) {
		case SDLK_UP:
			temporal_io=SDLK_LEFT;
		break;
		case SDLK_LEFT:
			temporal_io=SDLK_DOWN;
		break;
		case SDLK_DOWN:
			temporal_io=SDLK_RIGHT;
		break;
		case SDLK_RIGHT:
			temporal_io=SDLK_UP;
		break;
		}
	}
		
		
	// test for joystick
		
	switch (temporal_io) {
	case SDLK_UP:
		switch (ordenador.joystick) {
		case 0:	// cursor
			temporal_io = SDLK_7;
		break;
		
		case 1:
			ordenador.jk = 8;
		break;
		
		case 2:	// sinclair 1
			temporal_io = SDLK_4;
		break;
		
		case 3:	// sinclair 2
			temporal_io = SDLK_9;
		break;
		}
	break;
	
	case SDLK_DOWN:
		switch (ordenador.joystick) {
		case 0:	// cursor
			temporal_io = SDLK_6;
		break;
		
		case 1:
			ordenador.jk = 4;
		break;
		
		case 2:	// sinclair 1
			temporal_io = SDLK_3;
		break;
		
		case 3:	// sinclair 2
			temporal_io = SDLK_8;
		break;		
		}
	break;
		
	case SDLK_RIGHT:
		switch (ordenador.joystick) {
		case 0:	// cursor
			temporal_io = SDLK_8;
		break;
		
		case 1:
			ordenador.jk = 1;
		break;
		
		case 2:	// sinclair 1
			temporal_io = SDLK_1;
		break;
				
		case 3:	// sinclair 2
			temporal_io = SDLK_6;
		break;
		
		}
	break;
	
	case SDLK_LEFT:
		switch (ordenador.joystick) {
		case 0:	// cursor
			temporal_io = SDLK_5;
		break;
		
		case 1:
			ordenador.jk = 2;
		break;
		
		case 2:	// sinclair 1
			temporal_io = SDLK_2;
		break;
		
		case 3:	// sinclair 2
			temporal_io = SDLK_7;
		break;		
		}
	break;
	
	case SDLK_RALT:
	case SDLK_RMETA:
	case SDLK_LMETA:
	case SDLK_RSUPER:
	case SDLK_LSUPER:
	case SDLK_MENU:
		switch (ordenador.joystick) {
		case 0:	// cursor
			temporal_io = SDLK_0;
		break;
		
		case 1:
			ordenador.jk = 16;
		break;
		
		case 2:	// sinclair 1
			temporal_io = SDLK_5;
		break;
		
		case 3:	// sinclair 2
			temporal_io = SDLK_0;
		break;		
		}
	break;
	}

	switch (temporal_io) {

	case SDLK_SPACE:
		ordenador.k15 = 1;
	break;
	
	case SDLK_RCTRL:
	case SDLK_LCTRL:
		ordenador.k15 = 2;
	break;
	
	case SDLK_m:
		ordenador.k15 = 4;
	break;
	
	case SDLK_n:
		ordenador.k15 = 8;
	break;
	
	case SDLK_b:
		ordenador.k15 = 16;
	break;
	
	case SDLK_RETURN:
		ordenador.k14 = 1;
	break;
	
	case SDLK_l:
		ordenador.k14 = 2;
	break;
	
	case SDLK_k:
		ordenador.k14 = 4;
	break;
	
	case SDLK_j:
		ordenador.k14 = 8;
	break;
	
	case SDLK_h:
		ordenador.k14 = 16;
	break;
	
	case SDLK_p:
		ordenador.k13 = 1;
	break;
	
	case SDLK_o:
		ordenador.k13 = 2;
	break;
	
	case SDLK_i:
		ordenador.k13 = 4;
	break;
	
	case SDLK_u:
		ordenador.k13 = 8;
	break;
	
	case SDLK_y:
		ordenador.k13 = 16;
	break;
	
	case SDLK_0:
		ordenador.k12 = 1;
	break;
	
	case SDLK_9:
		ordenador.k12 = 2;
	break;
	
	case SDLK_8:
		ordenador.k12 = 4;
	break;
	
	case SDLK_7:
		ordenador.k12 = 8;
	break;
	
	case SDLK_6:
		ordenador.k12 = 16;
	break;
	
	case SDLK_1:
		ordenador.k11 = 1;
	break;
	
	case SDLK_2:
		ordenador.k11 = 2;
	break;
	
	case SDLK_3:
		ordenador.k11 = 4;
	break;
	
	case SDLK_4:
		ordenador.k11 = 8;
	break;
	
	case SDLK_5:
		ordenador.k11 = 16;
	break;
	
	case SDLK_q:
		ordenador.k10 = 1;
	break;
	
	case SDLK_w:
		ordenador.k10 = 2;
	break;
	
	case SDLK_e:
		ordenador.k10 = 4;
	break;
	
	case SDLK_r:
		ordenador.k10 = 8;
	break;
	
	case SDLK_t:
		ordenador.k10 = 16;
	break;
	
	case SDLK_a:
		ordenador.k9 = 1;
	break;
	
	case SDLK_s:
		ordenador.k9 = 2;
	break;
	
	case SDLK_d:
		ordenador.k9 = 4;
	break;
	
	case SDLK_f:
		ordenador.k9 = 8;
	break;
	
	case SDLK_g:
		ordenador.k9 = 16;
	break;
	
	case SDLK_RSHIFT:
	case SDLK_LSHIFT:
		ordenador.k8 = 1;
	break;
	
	case SDLK_z:
		ordenador.k8 = 2;
	break;
	
	case SDLK_x:
		ordenador.k8 = 4;
	break;
	
	case SDLK_c:
		ordenador.k8 = 8;
	break;
	
	case SDLK_v:
		ordenador.k8 = 16;
	break;
	
	case SDLK_BACKSPACE:
		ordenador.k12 = 1;
		ordenador.k8 = 1;
	break;
	case SDLK_PERIOD:
		ordenador.k15 = 6;
	break;
	case SDLK_COMMA:
		ordenador.k15 = 10;
	break;
	
	}

	if (pevento->type == SDL_KEYUP) {
		ordenador.s8 |= ordenador.k8;
		ordenador.s9 |= ordenador.k9;
		ordenador.s10 |= ordenador.k10;
		ordenador.s11 |= ordenador.k11;
		ordenador.s12 |= ordenador.k12;
		ordenador.s13 |= ordenador.k13;
		ordenador.s14 |= ordenador.k14;
		ordenador.s15 |= ordenador.k15;
		ordenador.js &= (ordenador.jk ^ 255);
	} else {
		ordenador.s8 &= (ordenador.k8 ^ 255);
		ordenador.s9 &= (ordenador.k9 ^ 255);
		ordenador.s10 &= (ordenador.k10 ^ 255);
		ordenador.s11 &= (ordenador.k11 ^ 255);
		ordenador.s12 &= (ordenador.k12 ^ 255);
		ordenador.s13 &= (ordenador.k13 ^ 255);
		ordenador.s14 &= (ordenador.k14 ^ 255);
		ordenador.s15 &= (ordenador.k15 ^ 255);
		ordenador.js |= ordenador.jk;
	}

	return;
}

// resets the computer and loads the right ROMs

void ResetComputer () {

	static int bucle;

	Z80free_reset (&procesador);
	load_rom (ordenador.mode128k);

	// reset the AY-3-8912

	for (bucle = 0; bucle < 16; bucle++)
		ordenador.ay_registers[bucle] = 0;

	ordenador.aych_a = 0;
	ordenador.aych_b = 0;
	ordenador.aych_c = 0;
	ordenador.aych_n = 0;
	ordenador.aych_envel = 0;
	ordenador.vol_a = 0;
	ordenador.vol_b = 0;
	ordenador.vol_c = 0;

	ordenador.s8|=0x1F;
	ordenador.s9|=0x1F;
	ordenador.s10|=0x1F;
	ordenador.s11|=0x1F;
	ordenador.s12|=0x1F;
	ordenador.s13|=0x1F;
	ordenador.s14|=0x1F;
	ordenador.s15|=0x1F;
	ordenador.js=0;

	ordenador.updown=0;
	ordenador.leftright=0;

	ordenador.ulaplus=0;

	ordenador.mport1 = 0;
	ordenador.mport2 = 0;
	ordenador.video_offset = 0;	// video in page 9 (page 5 in 128K)
	switch (ordenador.mode128k) {
	case 0:		// 48K
		ordenador.pixancho = 447;
		ordenador.pixalto = 311;

		ordenador.block0 = ordenador.memoria;
		ordenador.block1 = ordenador.memoria + 131072;	// video mem. in page 9 (page 5 in 128K)
		ordenador.block2 = ordenador.memoria + 65536;	// 2nd block in page 6 (page 2 in 128K)
		ordenador.block3 = ordenador.memoria + 65536;	// 3rd block in page 7 (page 3 in 128K)
		ordenador.mport1 = 32;	// access to port 7FFD disabled
	break;
	
	case 3:		// +2A/+3
		Z80free_Out (0x1FFD, 0);
	case 1:		// 128K
	case 2:		// +2
	case 4:		// spanish 128K
		Z80free_Out (0x7FFD, 0);
		ordenador.pixancho = 455;
		ordenador.pixalto = 310;
	break;
	}
	
	microdrive_reset();
}

// check if there's contention and waits the right number of tstates

void do_contention() {
	
	if (!ordenador.contended_zone)
		return;
	
	if (ordenador.cicles_counter<14335) {
		return;
	}

	int ccicles=(ordenador.cicles_counter-14335)%8;

	if (ccicles>5) {
		return;
	}
	
	emulate(6-ccicles);

}

void Z80free_Wr (register word Addr, register byte Value) {

	switch (Addr & 0xC000) {
	case 0x0000:
	// only writes in the first 16K if we are in +3 mode and bit0 of mport2 is 1

		if ((ordenador.mode128k == 3) && (1 == (ordenador.mport2 & 0x01)))
			*(ordenador.block0 + Addr) = (unsigned char) Value;
	break;

	case 0x4000:
		do_contention();
		*(ordenador.block1 + Addr) = (unsigned char) Value;
	break;
	
	case 0x8000:
		*(ordenador.block2 + Addr) = (unsigned char) Value;
	break;
	
	case 0xC000:
		*(ordenador.block3 + Addr) = (unsigned char) Value;
	break;
	}
}


byte Z80free_Rd (register word Addr) {

	if((ordenador.mdr_active)&&(ordenador.mdr_paged)&&(Addr<8192)) // Interface I
		return((byte)ordenador.shadowrom[Addr]);
	
	switch (ordenador.other_ret) {
	case 1:
		ordenador.other_ret = 0;
		return (201);	// RET instruction
	break;

	default:
		switch (Addr & 0xC000) {
		case 0x0000:
			return ((byte) (*(ordenador.block0 + Addr)));
		break;

		case 0x4000:
			do_contention();
			return ((byte) (*(ordenador.block1 + Addr)));
		break;

		case 0x8000:
			return ((byte) (*(ordenador.block2 + Addr)));
		break;

		case 0xC000:
			return ((byte) (*(ordenador.block3 + Addr)));
		break;

		default:
			printf ("Memory error\n");
			exit (1);
			return 0;
		}
		break;
	}
}

void Z80free_Out (register word Port, register byte Value) {

	// Microdrive access
	
	register word maskport;
	
	if (((Port&0x0001)==0)||((Port>=0x4000)&&(Port<0x8000))) {
		do_contention();
	}

	// ULAPlus
	if (Port == 0xBF3B) {
		do_contention();
		ordenador.ulaplus_reg = Value;
		return;
	}
	if (Port == 0xFF3B) {
		do_contention();
		if (ordenador.ulaplus_reg==0x40) { // mode
			ordenador.ulaplus=Value&0x01;
			return;
		}
		if (ordenador.ulaplus_reg<0x40) { // register set mode
			ordenador.ulaplus_palete[ordenador.ulaplus_reg]=Value;
			llscreen->set_palete_entry(ordenador.ulaplus_reg,Value);
		}
	}

	if(((Port &0x0018)!=0x0018)&&(ordenador.mdr_active))
		microdrive_out(Port,Value);
	
	// ULA port (A0 low)

	if (!(Port & 0x0001)) {
		ordenador.port254 = (unsigned char) Value;
		ordenador.border = (((unsigned char) Value) & 0x07);

		if (ordenador.OOTape.get_pause()) {
			if (Value & 0x10)
				ordenador.sound_bit = 1;
			else
				ordenador.sound_bit = 0;	// assign to SOUND_BIT the value
		}
	}

	// Memory page (7FFD & 1FFD)

	if (ordenador.mode128k==3) {
		maskport=0x0FFD;
	} else {
		maskport=0x3FFD;
	}
	
	if (((Port|maskport) == 0x7FFD) && (0 == (ordenador.mport1 & 0x20))) {
		ordenador.mport1 = (unsigned char) Value;
		set_memory_pointers ();	// set the pointers
	}

	if (((Port|maskport) == 0x1FFD) && (0 == (ordenador.mport1 & 0x20))) {
		ordenador.mport2 = (unsigned char) Value;
		set_memory_pointers ();	// set the pointers
	}

	// Sound chip (AY-3-8912)

	if (((Port|maskport) == 0xFFFD)&&(ordenador.ay_emul))
		ordenador.ay_latch = ((unsigned int) (Value & 0x0F));

	if (((Port|maskport) == 0xBFFD)&&(ordenador.ay_emul)) {
		ordenador.ay_registers[ordenador.ay_latch] = (unsigned char) Value;
		if (ordenador.ay_latch == 13)
			ordenador.ay_envel_way = 2;	// start cycle
	}
}


byte Z80free_In (register word Port) {

	static unsigned int temporal_io;
	byte pines;

	if (((Port&0x0001)==0)||((Port>=0x4000)&&(Port<0x8000))) {
		do_contention();
	}

	temporal_io = (unsigned int) Port;

	if (Port == 0xFF3B) {
		do_contention();
		if (ordenador.ulaplus_reg==0x40) { // mode
			return(ordenador.ulaplus&0x01);
		}
		if (ordenador.ulaplus_reg<0x40) { // register set mode
			return(ordenador.ulaplus_palete[ordenador.ulaplus_reg]);
		}
	}

	if (!(temporal_io & 0x0001)) {
		pines = 0xBF;	// by default, sound bit is 0
		if (!(temporal_io & 0x0100))
			pines &= ordenador.s8;
		if (!(temporal_io & 0x0200))
			pines &= ordenador.s9;
		if (!(temporal_io & 0x0400))
			pines &= ordenador.s10;
		if (!(temporal_io & 0x0800))
			pines &= ordenador.s11;
		if (!(temporal_io & 0x1000))
			pines &= ordenador.s12;
		if (!(temporal_io & 0x2000))
			pines &= ordenador.s13;
		if (!(temporal_io & 0x4000))
			pines &= ordenador.s14;
		if (!(temporal_io & 0x8000))
			pines &= ordenador.s15;

		if (ordenador.OOTape.get_pause()) {
			if (ordenador.issue == 2)	{
				if (ordenador.port254 & 0x18)
					pines |= 0x40;
			} else {
				if (ordenador.port254 & 0x10)
					pines |= 0x40;
			}
			if (random()<(RAND_MAX/200)) { // add tape noise when paused
				pines |= 0x40;
			}
		} else {
			if (ordenador.OOTape.read_signal() != 0)
				pines |= 0x40;	// sound input
			else
				pines &= 0xBF;	// sound input
		}
		return (pines);
	}

	// Joystick
	if (!(temporal_io & 0x0020)) {
		if (ordenador.joystick == 1) {
			return (ordenador.js);
		} else {
			return 0; // if Kempston is not selected, emulate it, but always 0
		}
	}

	if ((temporal_io == 0xFFFD)&&(ordenador.ay_emul))
		return (ordenador.ay_registers[ordenador.ay_latch]);

	// Microdrive access
	
	if(((Port &0x0018)!=0x0018)&&(ordenador.mdr_active))
		return(microdrive_in(Port));
	
	pines=bus_empty();

	return (pines);
}

void set_volume (unsigned char volume) {

	unsigned char vol2;
	int bucle;

	if (volume > 64)
		vol2 = 64;
	else
		vol2 = volume;

	ordenador.volume = vol2;

	for (bucle = 0; bucle < 4; bucle++)	{
		ordenador.sample0[bucle] = 0;
		ordenador.sample1[bucle] = 0;
		ordenador.sample1b[bucle] = 0;
	}

	switch (ordenador.format) {
	case 0: // 8 bits/sample
		ordenador.sample1[0] = 1 * vol2;
		ordenador.sample1[1] = 1 * vol2;
		ordenador.sample1b[0] = 1;
		ordenador.sample1b[1] = 1;
		break;
	case 1: // 16 bits/sample, Little Endian
		ordenador.sample1[0] = 1 * vol2;
		ordenador.sample1[1] = 1 * vol2;
		ordenador.sample1[2] = 1 * vol2;
		ordenador.sample1[3] = 1 * vol2;
		ordenador.sample1b[0] = 1;
		ordenador.sample1b[1] = 1;
		ordenador.sample1b[2] = 1;
		ordenador.sample1b[3] = 1;
		break;
	case 2: // 16 bits/sample, Big Endian
		ordenador.sample1[0] = 1 * vol2;
		ordenador.sample1[1] = 1 * vol2;
		ordenador.sample1[2] = 1 * vol2;
		ordenador.sample1[3] = 1 * vol2;
		ordenador.sample1b[0] = 1;
		ordenador.sample1b[1] = 1;
		ordenador.sample1b[2] = 1;
		ordenador.sample1b[3] = 1;
		break;
	}
}
