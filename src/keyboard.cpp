/*
 * keyboard.cpp
 *
 *  Created on: 27/03/2015
 *      Author: raster
 */

#include "keyboard.hh"
#include "computer.hh"
#include "llsound.hh"
#include "menus.hh"

class Keyboard *keyboard;

Keyboard::Keyboard() {

	this->s8 = this->s9 = this->s10 = this->s11 = this->s12 = this->s13 = this->s14 = this->s15 = 0xFF;
	this->k8 = this->k9 = this->k10 = this->k11 = this->k12 = this->k13 = this->k14 = this->k15 = 0;
	this->tab_extended = false;
	this->esc_again = false;

	this->js = 0x00;
	this->jk = 0x00;

	this->readed = 0;
	this->joystick = 0;
	this->mouse_x = 0;
	this->mouse_y = 0;
	this->mouse_left = false;
	this->mouse_right = false;
	this->mouse_center = false;
}

void Keyboard::reset() {

	this->s8|=0x1F;
	this->s9|=0x1F;
	this->s10|=0x1F;
	this->s11|=0x1F;
	this->s12|=0x1F;
	this->s13|=0x1F;
	this->s14|=0x1F;
	this->s15|=0x1F;
	this->js=0;
}

// Read the keyboard and mouse, and stores the flags

void Keyboard::read_keyboard (SDL_Event *pevento2) {

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

	if (pevento->type==SDL_MOUSEMOTION) {
		this->mouse_x += pevento->motion.xrel;
		this->mouse_y += pevento->motion.yrel;
		return;
	}

	if (pevento->type==SDL_MOUSEBUTTONDOWN) {
		switch (pevento->button.button) {
		case SDL_BUTTON_LEFT:
			this->mouse_left = true;
		break;
		case SDL_BUTTON_MIDDLE:
			this->mouse_center = true;
		break;
		case SDL_BUTTON_RIGHT:
			this->mouse_right = true;
		break;
		}
	}

	if (pevento->type==SDL_MOUSEBUTTONUP) {
		switch (pevento->button.button) {
		case SDL_BUTTON_LEFT:
			this->mouse_left = false;
		break;
		case SDL_BUTTON_MIDDLE:
			this->mouse_center = false;
		break;
		case SDL_BUTTON_RIGHT:
			this->mouse_right = false;
		break;
		}
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

	this->k8 = this->k9 = this->k10 = this->k11 =
		this->k12 = this->k13 = this->k14 =
		this->k15 = 0;
		this->jk = 0;

	temporal_io = (unsigned int) pevento->key.keysym.sym;

	if ((pevento->type==SDL_KEYUP)&&(temporal_io==SDLK_TAB)) {
		if (!this->tab_extended) {
			this->tab_extended = true;
			osd->set_message("Function Key mode on",2000);
			return;
		} else {
			this->tab_extended = false;
			osd->clear_message();
			return;
		}
	}

	if ((pevento->type==SDL_KEYDOWN)&&(this->tab_extended))
		return;

	if ((pevento->type==SDL_KEYUP)&&(this->tab_extended)) {
		this->tab_extended = false;

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
			if (!this->esc_again) {
				this->esc_again = true;
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
			llsound->set_speed(ordenador->turbo);
			break;

		case SDLK_F6:	// PLAY tape
			OOTape->set_pause(false);
			if (ordenador->turbo_play) {
				llsound->set_speed(true);
			} else {
				llsound->set_speed(ordenador->turbo);
			}
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
		switch (this->joystick) {
		case 0:	// cursor
			temporal_io = SDLK_7;
		break;

		case 1:
			this->jk = 8;
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
		switch (this->joystick) {
		case 0:	// cursor
			temporal_io = SDLK_6;
		break;

		case 1:
			this->jk = 4;
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
		switch (this->joystick) {
		case 0:	// cursor
			temporal_io = SDLK_8;
		break;

		case 1:
			this->jk = 1;
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
		switch (this->joystick) {
		case 0:	// cursor
			temporal_io = SDLK_5;
		break;

		case 1:
			this->jk = 2;
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
		switch (this->joystick) {
		case 0:	// cursor
			temporal_io = SDLK_0;
		break;

		case 1:
			this->jk = 16;
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
		this->k15 = 1;
	break;

	case SDLK_RCTRL:
	case SDLK_LCTRL:
		this->k15 = 2;
	break;

	case SDLK_m:
		this->k15 = 4;
	break;

	case SDLK_n:
		this->k15 = 8;
	break;

	case SDLK_b:
		this->k15 = 16;
	break;

	case SDLK_RETURN:
		this->k14 = 1;
	break;

	case SDLK_l:
		this->k14 = 2;
	break;

	case SDLK_k:
		this->k14 = 4;
	break;

	case SDLK_j:
		this->k14 = 8;
	break;

	case SDLK_h:
		this->k14 = 16;
	break;

	case SDLK_p:
		this->k13 = 1;
	break;

	case SDLK_o:
		this->k13 = 2;
	break;

	case SDLK_i:
		this->k13 = 4;
	break;

	case SDLK_u:
		this->k13 = 8;
	break;

	case SDLK_y:
		this->k13 = 16;
	break;

	case SDLK_0:
		this->k12 = 1;
	break;

	case SDLK_9:
		this->k12 = 2;
	break;

	case SDLK_8:
		this->k12 = 4;
	break;

	case SDLK_7:
		this->k12 = 8;
	break;

	case SDLK_6:
		this->k12 = 16;
	break;

	case SDLK_1:
		this->k11 = 1;
	break;

	case SDLK_2:
		this->k11 = 2;
	break;

	case SDLK_3:
		this->k11 = 4;
	break;

	case SDLK_4:
		this->k11 = 8;
	break;

	case SDLK_5:
		this->k11 = 16;
	break;

	case SDLK_q:
		this->k10 = 1;
	break;

	case SDLK_w:
		this->k10 = 2;
	break;

	case SDLK_e:
		this->k10 = 4;
	break;

	case SDLK_r:
		this->k10 = 8;
	break;

	case SDLK_t:
		this->k10 = 16;
	break;

	case SDLK_a:
		this->k9 = 1;
	break;

	case SDLK_s:
		this->k9 = 2;
	break;

	case SDLK_d:
		this->k9 = 4;
	break;

	case SDLK_f:
		this->k9 = 8;
	break;

	case SDLK_g:
		this->k9 = 16;
	break;

	case SDLK_RSHIFT:
	case SDLK_LSHIFT:
		this->k8 = 1;
	break;

	case SDLK_z:
		this->k8 = 2;
	break;

	case SDLK_x:
		this->k8 = 4;
	break;

	case SDLK_c:
		this->k8 = 8;
	break;

	case SDLK_v:
		this->k8 = 16;
	break;

	case SDLK_BACKSPACE:
		this->k12 = 1;
		this->k8 = 1;
	break;
	case SDLK_PERIOD:
		this->k15 = 6;
	break;
	case SDLK_COMMA:
		this->k15 = 10;
	break;

	}

	if (pevento->type == SDL_KEYUP) {
		this->s8 |= this->k8;
		this->s9 |= this->k9;
		this->s10 |= this->k10;
		this->s11 |= this->k11;
		this->s12 |= this->k12;
		this->s13 |= this->k13;
		this->s14 |= this->k14;
		this->s15 |= this->k15;
		this->js &= (this->jk ^ 255);
	} else {
		this->s8 &= (this->k8 ^ 255);
		this->s9 &= (this->k9 ^ 255);
		this->s10 &= (this->k10 ^ 255);
		this->s11 &= (this->k11 ^ 255);
		this->s12 &= (this->k12 ^ 255);
		this->s13 &= (this->k13 ^ 255);
		this->s14 &= (this->k14 ^ 255);
		this->s15 &= (this->k15 ^ 255);
		this->js |= this->jk;
	}

	return;
}
