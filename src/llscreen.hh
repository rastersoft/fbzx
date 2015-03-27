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

#ifndef __LLSCREEN_H_
#define __LLSCREEN_H_

using namespace std;

#include <inttypes.h>
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

extern struct Charset charset;

#define MIN_WIDTH 4

class LLScreen {

	uint8_t *memory;
	uint32_t width;
	uint8_t ulaplus_palete[64]; // contains the current palete
	uint32_t colors[80];

	uint8_t printchar(uint8_t character, int16_t x, int16_t y, uint8_t color, uint8_t back);
public:
	bool joystick;
	bool mustlock;
	bool rotate;
	uint32_t bpp;
	SDL_Surface *llscreen;
	uint32_t cheight;
	uint32_t lines_in_screen;

	LLScreen(int16_t resx, int16_t resy, uint8_t depth, bool fullscreen, bool dblbuffer, bool hwsurface);
	~LLScreen();
	FILE *myfopen(char *filename,char *mode);
	void print_string(string message, int16_t x, int16_t y, uint8_t ink, uint8_t paper);
	void set_paletes(bool);
	void paint_one_pixel(uint8_t value,unsigned char *address);
	void fullscreen_switch();
	void set_palete_entry(uint8_t entry, uint8_t Value, bool bw);
	uint8_t get_palete_entry(uint8_t entry);
	void clear_screen();
	void paint_picture(string filename);
	void do_flip();
};

extern LLScreen *llscreen;

#endif
