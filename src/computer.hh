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

extern char salir;

extern class computer *ordenador;

enum tapmodes {TAP_GUIDE, TAP_DATA, TAP_PAUSE, TAP_TRASH, TAP_STOP, TAP_PAUSE2, TZX_PURE_TONE,
	TZX_SEQ_PULSES, TAP_FINAL_BIT, TAP_PAUSE3};

enum taptypes {TAP_TAP, TAP_TZX};

class computer : public Signals {
public:
	unsigned int temporal_io;

	unsigned char zaurus_mini;
	bool dblscan;
	bool bw;

	bool contended_zone; // 0-> no contention; 1-> contention possible
	int cicles_counter; // counts how many pixel clock cicles passed since las interrupt

	// Linux joystick private global variables

	unsigned char updown,leftright;

	// sound global variables
	unsigned char sound_bit;
	unsigned int tstados_counter_sound;
	unsigned char num_buff;
	unsigned int sound_cuantity; // counter for the buffer
	unsigned char ay_registers[16]; // registers for the AY emulation
	unsigned int aych_a,aych_b,aych_c,aych_n,aych_envel; // counters for AY emulation
	unsigned char ayval_a,ayval_b,ayval_c,ayval_n;
	unsigned char ay_emul; // 0: no AY emulation; 1: AY emulation
	unsigned char vol_a,vol_b,vol_c;
	unsigned int tst_ay;
	unsigned int tst_ay2;
	unsigned int ay_latch;
	signed char ay_envel_value;
	unsigned char ay_envel_way;
	unsigned char sound_current_value;

	// bus global variables

	unsigned char bus_counter;
	unsigned char bus_value;
	unsigned char issue; // 2= 48K issue 2, 3= 48K issue 3
	unsigned char mode128k; // 0=48K, 1=128K, 2=+2, 3=+3
	unsigned char port254;


	// tape global variables

	string current_tap;
	bool tape_write; // FALSE can't write; TRUE can write
	bool tape_fast_load; // FALSE normal load; TRUE fast load

	// Microdrive global variables
	FILE *mdr_file;                  // Current microdrive file
	char mdr_current_mdr[2049]; // current path and name for microdrive file
	unsigned char mdr_active;	// 0: not installed; 1: installed
	unsigned char mdr_paged;	// 0: not pagined; 1: pagined
	unsigned int mdr_tapehead; // current position in the tape
	unsigned int mdr_bytes;      // number of bytes read or written in this transfer
	unsigned int mdr_maxbytes; // maximum number of bytes to read or write in this transfer
	unsigned int mdr_gap;         // TSTATEs remaining for GAP end
	unsigned int mdr_nogap;      // TSTATEs remaining for next GAP
	unsigned char mdr_cartridge[137923]; // current cartridge
	unsigned char mdr_drive; // current drive
	byte mdr_old_STATUS; // to detect an edge in COM CLK
	unsigned char mdr_modified; // if a sector is stored, this change to know that it must be stored in the file

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

	unsigned char turbo;

	computer();
	~computer();
	bool callback_receiver(string, class Signals *);
	byte bus_empty();
	void emulate(int);
	void do_contention();
};

void fill_audio(void *udata,Uint8 *,int);
void ResetComputer();

#endif
