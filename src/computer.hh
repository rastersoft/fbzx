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

#ifndef computer_h
#define computer_h

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

#include "tape.hh"
#include "z80free/Z80free.h"
#include "llscreen.hh"
#include "signals.hh"
#include "screen.hh"

// #define MUT

#define CONTENTION_RD 3
#define CONTENTION_WR 3
#define CONTENTION_IN 3
#define CONTENTION_OUT 3

extern char salir;

extern class computer *ordenador;

enum tapmodes {TAP_GUIDE, TAP_DATA, TAP_PAUSE, TAP_TRASH, TAP_STOP, TAP_PAUSE2, TZX_PURE_TONE,
	TZX_SEQ_PULSES, TAP_FINAL_BIT, TAP_PAUSE3};

enum taptypes {TAP_TAP, TAP_TZX};

enum CurrentMode {MODE_48K, MODE_128K, MODE_P2, MODE_P3, MODE_128K_SPA};

class computer : public Signals {
public:
	unsigned int temporal_io;

	unsigned char zaurus_mini;
	bool dblscan;
	bool bw;

	int memcontended_zone; // memory contended tstates from this instant up to the next non-contended block
	int cicles_counter; // counts how many pixel clock cicles passed since las interrupt
	int contended_cicles; // cicles used during contention (must not be counted after ending the execution of an instruction)

	// Linux joystick private global variables

	unsigned char updown,leftright;

	// Sound variables
	unsigned char sound_bit;

	// bus global variables

	unsigned char bus_counter;
	unsigned char bus_value;
	unsigned char bus_value2;
	bool issue_3; // 2= 48K issue 2, 3= 48K issue 3
	enum CurrentMode current_mode;
	unsigned char port254;


	// tape global variables

	string current_tap;
	bool tape_write; // FALSE can't write; TRUE can write
	bool tape_fast_load; // FALSE normal load; TRUE fast load

	// pagination global variables

	unsigned char mport1,mport2; // ports for memory management (128K and +3)
	unsigned int video_offset; // 0 for page 5, and 32768 for page 7
	unsigned char *block0,*block1,*block2,*block3; // pointers for memory access (one for each 16K block).
	unsigned char page48k; // 1 if the 48K ROM page is currently paged into

	// public

	unsigned char memoria[196608]; // memory (12 pages of 16K each one). 4 for ROM, and 8 for RAM
	unsigned char shadowrom[8192]; // space for Interface I's ROMs
	unsigned char interr;
	unsigned char other_ret; // 0=no change; 1=memory returns RET (201)

	bool turbo;
	bool turbo_play;

	computer();
	~computer();
	bool callback_receiver(string, class Signals *);
	byte bus_empty();
	void emulate(int);
	void do_contention(bool io, word addr);
	uint8_t read_memory(uint16_t Addr);
	void write_memory (uint16_t Addr, uint8_t Value);
};

void fill_audio(void *udata,Uint8 *,int);
void ResetComputer();

#endif
