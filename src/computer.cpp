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

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <sstream>

#include "emulator.hh"
#include "llscreen.hh"
#include "menus.hh"
#include "microdrive.hh"
#include "sound.hh"
#include "spk_ay.hh"
#include "tape.hh"
#include "computer.hh"
#include "z80free/Z80free.h"

class computer *ordenador;

computer::computer() {

	int bucle;

	this->dblscan = false;
	this->bw = false;

	this->page48k = 0;
	this->bus_counter = 0;
	this->port254 = 0;
	this->issue = 3;
	this->mode128k = 0;
	this->joystick = 0;

	this->tape_fast_load = true; // fast load by default

	this->other_ret = 0;

	this->s8 = this->s9 = this->s10 = this->s11 =
	this->s12 = this->s13 = this->s14 =
	this->s15 = 0xFF;
	this->tab_extended=0;
	this->esc_again=0;

	this->js = 0x00;

	for (bucle = 0; bucle < 16; bucle++)
		this->ay_registers[bucle] = 0;

	this->ay_emul = 0;
	this->aych_a = 0;
	this->aych_b = 0;
	this->aych_c = 0;
	this->aych_n = 0;
	this->aych_envel = 0;
	this->vol_a = 0;
	this->vol_b = 0;
	this->vol_c = 0;
	this->tst_ay = 0;
	this->tst_ay2 = 0;

	this->ayval_a = 0;
	this->ayval_b = 0;
	this->ayval_c = 0;
	this->ayval_n = 0;
	this->ay_envel_value = 0;
	this->ay_envel_way = 0;
	this->readed = 0;

	this->contended_zone = false;
	this->cicles_counter=0;

	this->tstados_counter_sound = 0;
	this->num_buff = 0;	// first buffer
	this->sound_cuantity = 0;
	this->sound_current_value = 0;
	this->interr = 0;
	OOTape->register_signal("pause_tape",this);
	OOTape->register_signal("pause_tape_48k",this);
}

computer::~computer() {
}

bool computer::callback_receiver(string signal_received, class Signals *object) {

	if (signal_received == "pause_tape") {
		OOTape->set_pause(true);
		return true;
	}
	if (signal_received == "pause_tape_48k") {
		if ((this->mode128k == 0) || ((this->mport1 & 0x20) != 0)) {
			OOTape->set_pause(true);
		}
		return true;
	}
	return true;
}

/* Returns the bus value when reading a port without a periferial */

byte bus_empty () {

	if (ordenador->mode128k != 3)
		return (ordenador->bus_value);
	else
		return (255);	// +2A and +3 returns always 255
}

/* calls all the routines that emulates the computer, runing them for 'tstados'
   tstates */

void emulate (int tstados) {

	screen->show_screen (tstados);
	play_ay (tstados);
	play_sound (tstados);
	OOTape->play(tstados);
	microdrive_emulate(tstados);

	if (!OOTape->get_pause()) {
		if (OOTape->read_signal() != 0) {
			ordenador->sound_bit = 1;
		} else {
			ordenador->sound_bit = 0;	// if not paused, asign SOUND_BIT the value of tape
		}
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

	ordenador->k8 = ordenador->k9 = ordenador->k10 = ordenador->k11 =
		ordenador->k12 = ordenador->k13 = ordenador->k14 =
		ordenador->k15 = 0;
		ordenador->jk = 0;

	temporal_io = (unsigned int) pevento->key.keysym.sym;

	if ((pevento->type==SDL_KEYUP)&&(temporal_io==SDLK_TAB)) {
		if (ordenador->tab_extended==0) {
			ordenador->tab_extended=1;
			osd->set_message("Function Key mode on",2000);
			return;
		} else {
			ordenador->tab_extended=0;
			osd->clear_message();
			return;
		}
	}
	
	if ((pevento->type==SDL_KEYDOWN)&&(ordenador->tab_extended==1))
		return;
	
	if ((pevento->type==SDL_KEYUP)&&(ordenador->tab_extended==1)) {
		ordenador->tab_extended=0;

		osd->clear_message();

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
			if (ordenador->esc_again==0) {
				ordenador->esc_again=1;
				osd->set_message("ESC again to exit",2000);
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
			OOTape->set_pause(true);
			break;

		case SDLK_F6:	// PLAY tape
			OOTape->set_pause(false);
			break;		

		case SDLK_F9:
			llscreen->fullscreen_switch();
			break;

		case SDLK_F10:	// Reset emulator
			ResetComputer ();
			OOTape->set_pause(true);
			OOTape->rewind();
		break;

		case SDLK_F11:	// lower volume
			llsound->decrease_volume();
		break;
			
		case SDLK_F12:	// upper volume
			llsound->increase_volume();
		break;
		}

	// reorder joystick if screen is rotated
		
	if(ordenador->zaurus_mini==2) {
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
		switch (ordenador->joystick) {
		case 0:	// cursor
			temporal_io = SDLK_7;
		break;
		
		case 1:
			ordenador->jk = 8;
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
		switch (ordenador->joystick) {
		case 0:	// cursor
			temporal_io = SDLK_6;
		break;
		
		case 1:
			ordenador->jk = 4;
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
		switch (ordenador->joystick) {
		case 0:	// cursor
			temporal_io = SDLK_8;
		break;
		
		case 1:
			ordenador->jk = 1;
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
		switch (ordenador->joystick) {
		case 0:	// cursor
			temporal_io = SDLK_5;
		break;
		
		case 1:
			ordenador->jk = 2;
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
		switch (ordenador->joystick) {
		case 0:	// cursor
			temporal_io = SDLK_0;
		break;
		
		case 1:
			ordenador->jk = 16;
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
		ordenador->k15 = 1;
	break;
	
	case SDLK_RCTRL:
	case SDLK_LCTRL:
		ordenador->k15 = 2;
	break;
	
	case SDLK_m:
		ordenador->k15 = 4;
	break;
	
	case SDLK_n:
		ordenador->k15 = 8;
	break;
	
	case SDLK_b:
		ordenador->k15 = 16;
	break;
	
	case SDLK_RETURN:
		ordenador->k14 = 1;
	break;
	
	case SDLK_l:
		ordenador->k14 = 2;
	break;
	
	case SDLK_k:
		ordenador->k14 = 4;
	break;
	
	case SDLK_j:
		ordenador->k14 = 8;
	break;
	
	case SDLK_h:
		ordenador->k14 = 16;
	break;
	
	case SDLK_p:
		ordenador->k13 = 1;
	break;
	
	case SDLK_o:
		ordenador->k13 = 2;
	break;
	
	case SDLK_i:
		ordenador->k13 = 4;
	break;
	
	case SDLK_u:
		ordenador->k13 = 8;
	break;
	
	case SDLK_y:
		ordenador->k13 = 16;
	break;
	
	case SDLK_0:
		ordenador->k12 = 1;
	break;
	
	case SDLK_9:
		ordenador->k12 = 2;
	break;
	
	case SDLK_8:
		ordenador->k12 = 4;
	break;
	
	case SDLK_7:
		ordenador->k12 = 8;
	break;
	
	case SDLK_6:
		ordenador->k12 = 16;
	break;
	
	case SDLK_1:
		ordenador->k11 = 1;
	break;
	
	case SDLK_2:
		ordenador->k11 = 2;
	break;
	
	case SDLK_3:
		ordenador->k11 = 4;
	break;
	
	case SDLK_4:
		ordenador->k11 = 8;
	break;
	
	case SDLK_5:
		ordenador->k11 = 16;
	break;
	
	case SDLK_q:
		ordenador->k10 = 1;
	break;
	
	case SDLK_w:
		ordenador->k10 = 2;
	break;
	
	case SDLK_e:
		ordenador->k10 = 4;
	break;
	
	case SDLK_r:
		ordenador->k10 = 8;
	break;
	
	case SDLK_t:
		ordenador->k10 = 16;
	break;
	
	case SDLK_a:
		ordenador->k9 = 1;
	break;
	
	case SDLK_s:
		ordenador->k9 = 2;
	break;
	
	case SDLK_d:
		ordenador->k9 = 4;
	break;
	
	case SDLK_f:
		ordenador->k9 = 8;
	break;
	
	case SDLK_g:
		ordenador->k9 = 16;
	break;
	
	case SDLK_RSHIFT:
	case SDLK_LSHIFT:
		ordenador->k8 = 1;
	break;
	
	case SDLK_z:
		ordenador->k8 = 2;
	break;
	
	case SDLK_x:
		ordenador->k8 = 4;
	break;
	
	case SDLK_c:
		ordenador->k8 = 8;
	break;
	
	case SDLK_v:
		ordenador->k8 = 16;
	break;
	
	case SDLK_BACKSPACE:
		ordenador->k12 = 1;
		ordenador->k8 = 1;
	break;
	case SDLK_PERIOD:
		ordenador->k15 = 6;
	break;
	case SDLK_COMMA:
		ordenador->k15 = 10;
	break;
	
	}

	if (pevento->type == SDL_KEYUP) {
		ordenador->s8 |= ordenador->k8;
		ordenador->s9 |= ordenador->k9;
		ordenador->s10 |= ordenador->k10;
		ordenador->s11 |= ordenador->k11;
		ordenador->s12 |= ordenador->k12;
		ordenador->s13 |= ordenador->k13;
		ordenador->s14 |= ordenador->k14;
		ordenador->s15 |= ordenador->k15;
		ordenador->js &= (ordenador->jk ^ 255);
	} else {
		ordenador->s8 &= (ordenador->k8 ^ 255);
		ordenador->s9 &= (ordenador->k9 ^ 255);
		ordenador->s10 &= (ordenador->k10 ^ 255);
		ordenador->s11 &= (ordenador->k11 ^ 255);
		ordenador->s12 &= (ordenador->k12 ^ 255);
		ordenador->s13 &= (ordenador->k13 ^ 255);
		ordenador->s14 &= (ordenador->k14 ^ 255);
		ordenador->s15 &= (ordenador->k15 ^ 255);
		ordenador->js |= ordenador->jk;
	}

	return;
}

// resets the computer and loads the right ROMs

void ResetComputer () {

	static int bucle;

	Z80free_reset (&procesador);
	load_rom (ordenador->mode128k);

	// reset the AY-3-8912

	for (bucle = 0; bucle < 16; bucle++)
		ordenador->ay_registers[bucle] = 0;

	ordenador->aych_a = 0;
	ordenador->aych_b = 0;
	ordenador->aych_c = 0;
	ordenador->aych_n = 0;
	ordenador->aych_envel = 0;
	ordenador->vol_a = 0;
	ordenador->vol_b = 0;
	ordenador->vol_c = 0;

	ordenador->s8|=0x1F;
	ordenador->s9|=0x1F;
	ordenador->s10|=0x1F;
	ordenador->s11|=0x1F;
	ordenador->s12|=0x1F;
	ordenador->s13|=0x1F;
	ordenador->s14|=0x1F;
	ordenador->s15|=0x1F;
	ordenador->js=0;

	ordenador->updown=0;
	ordenador->leftright=0;

	ordenador->mport1 = 0;
	ordenador->mport2 = 0;
	ordenador->video_offset = 0;	// video in page 9 (page 5 in 128K)
	switch (ordenador->mode128k) {
	case 0:		// 48K
		ordenador->block0 = ordenador->memoria;
		ordenador->block1 = ordenador->memoria + 131072;	// video mem. in page 9 (page 5 in 128K)
		ordenador->block2 = ordenador->memoria + 65536;	// 2nd block in page 6 (page 2 in 128K)
		ordenador->block3 = ordenador->memoria + 65536;	// 3rd block in page 7 (page 3 in 128K)
		ordenador->mport1 = 32;	// access to port 7FFD disabled
	break;
	
	case 3:		// +2A/+3
		Z80free_Out (0x1FFD, 0);
	case 1:		// 128K
	case 2:		// +2
	case 4:		// spanish 128K
		Z80free_Out (0x7FFD, 0);
	break;
	}
	screen->reset(ordenador->mode128k);
	microdrive_reset();
}

// check if there's contention and waits the right number of tstates

void do_contention() {
	
	if (!ordenador->contended_zone)
		return;
	
	if (ordenador->cicles_counter<14335) {
		return;
	}

	int ccicles=(ordenador->cicles_counter-14335)%8;

	if (ccicles>5) {
		return;
	}
	
	emulate(6-ccicles);

}

void Z80free_Wr (register word Addr, register byte Value) {

	switch (Addr & 0xC000) {
	case 0x0000:
	// only writes in the first 16K if we are in +3 mode and bit0 of mport2 is 1

		if ((ordenador->mode128k == 3) && (1 == (ordenador->mport2 & 0x01)))
			*(ordenador->block0 + Addr) = (unsigned char) Value;
	break;

	case 0x4000:
		do_contention();
		*(ordenador->block1 + Addr) = (unsigned char) Value;
	break;
	
	case 0x8000:
		*(ordenador->block2 + Addr) = (unsigned char) Value;
	break;
	
	case 0xC000:
		*(ordenador->block3 + Addr) = (unsigned char) Value;
	break;
	}
}


byte Z80free_Rd (register word Addr) {

	if((ordenador->mdr_active)&&(ordenador->mdr_paged)&&(Addr<8192)) // Interface I
		return((byte)ordenador->shadowrom[Addr]);
	
	switch (ordenador->other_ret) {
	case 1:
		ordenador->other_ret = 0;
		return (201);	// RET instruction
	break;

	default:
		switch (Addr & 0xC000) {
		case 0x0000:
			return ((byte) (*(ordenador->block0 + Addr)));
		break;

		case 0x4000:
			do_contention();
			return ((byte) (*(ordenador->block1 + Addr)));
		break;

		case 0x8000:
			return ((byte) (*(ordenador->block2 + Addr)));
		break;

		case 0xC000:
			return ((byte) (*(ordenador->block3 + Addr)));
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
		screen->set_ulaplus_register(Value);
		return;
	}
	if (Port == 0xFF3B) {
		do_contention();
		screen->set_ulaplus_value(Value);
	}

	if(((Port &0x0018)!=0x0018)&&(ordenador->mdr_active))
		microdrive_out(Port,Value);
	
	// ULA port (A0 low)

	if (!(Port & 0x0001)) {
		ordenador->port254 = (unsigned char) Value;
		screen->border = ((((unsigned char) Value) & 0x07));

		if (OOTape->get_pause()) {
			if (Value & 0x10)
				ordenador->sound_bit = 1;
			else
				ordenador->sound_bit = 0;	// assign to SOUND_BIT the value
		}
	}

	// Memory page (7FFD & 1FFD)

	if (ordenador->mode128k==3) {
		maskport=0x0FFD;
	} else {
		maskport=0x3FFD;
	}
	
	if (((Port|maskport) == 0x7FFD) && (0 == (ordenador->mport1 & 0x20))) {
		ordenador->mport1 = (unsigned char) Value;
		screen->set_memory_pointers ();	// set the pointers
	}

	if (((Port|maskport) == 0x1FFD) && (0 == (ordenador->mport1 & 0x20))) {
		ordenador->mport2 = (unsigned char) Value;
		screen->set_memory_pointers ();	// set the pointers
	}

	// Sound chip (AY-3-8912)

	if (((Port|maskport) == 0xFFFD)&&(ordenador->ay_emul))
		ordenador->ay_latch = ((unsigned int) (Value & 0x0F));

	if (((Port|maskport) == 0xBFFD)&&(ordenador->ay_emul)) {
		ordenador->ay_registers[ordenador->ay_latch] = (unsigned char) Value;
		if (ordenador->ay_latch == 13)
			ordenador->ay_envel_way = 2;	// start cycle
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
		return (screen->read_ulaplus_value());
	}

	if (!(temporal_io & 0x0001)) {
		pines = 0xBF;	// by default, sound bit is 0
		if (!(temporal_io & 0x0100))
			pines &= ordenador->s8;
		if (!(temporal_io & 0x0200))
			pines &= ordenador->s9;
		if (!(temporal_io & 0x0400))
			pines &= ordenador->s10;
		if (!(temporal_io & 0x0800))
			pines &= ordenador->s11;
		if (!(temporal_io & 0x1000))
			pines &= ordenador->s12;
		if (!(temporal_io & 0x2000))
			pines &= ordenador->s13;
		if (!(temporal_io & 0x4000))
			pines &= ordenador->s14;
		if (!(temporal_io & 0x8000))
			pines &= ordenador->s15;

		if (OOTape->get_pause()) {
			if (ordenador->issue == 2)	{
				if (ordenador->port254 & 0x18)
					pines |= 0x40;
			} else {
				if (ordenador->port254 & 0x10)
					pines |= 0x40;
			}
			if (random()<(RAND_MAX/200)) { // add tape noise when paused
				pines |= 0x40;
			}
		} else {
			if (OOTape->read_signal() != 0)
				pines |= 0x40;	// sound input
			else
				pines &= 0xBF;	// sound input
		}
		return (pines);
	}

	// Joystick
	if (!(temporal_io & 0x0020)) {
		if (ordenador->joystick == 1) {
			return (ordenador->js);
		} else {
			return 0; // if Kempston is not selected, emulate it, but always 0
		}
	}

	if ((temporal_io == 0xFFFD)&&(ordenador->ay_emul))
		return (ordenador->ay_registers[ordenador->ay_latch]);

	// Microdrive access
	
	if(((Port &0x0018)!=0x0018)&&(ordenador->mdr_active))
		return(microdrive_in(Port));
	
	pines=bus_empty();

	return (pines);
}

