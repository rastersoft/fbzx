/*
 * keyboard.hh
 *
 *  Created on: 27/03/2015
 *      Author: raster
 */

#ifndef SRC_KEYBOARD_HH_
#define SRC_KEYBOARD_HH_

#include <inttypes.h>
#include <SDL/SDL.h>

class Keyboard {

	// keyboard private global variables

	uint8_t k8,k9,k10,k11,k12,k13,k14,k15;
	uint8_t readed;

	// kempston joystick private global variables

	uint8_t jk;

public:
	Keyboard();
	void reset();
	void read_keyboard(SDL_Event *);

	uint8_t s8,s9,s10,s11,s12,s13,s14,s15;
	uint8_t js;
	uint8_t joystick; // 0=cursor, 1=kempston, 2=sinclair1, 3=sinclair2
	int32_t mouse_x;
	int32_t mouse_y;
	bool mouse_left;
	bool mouse_center;
	bool mouse_right;
	bool tab_extended;
	bool esc_again;
};

extern class Keyboard *keyboard;


#endif /* SRC_KEYBOARD_HH_ */
