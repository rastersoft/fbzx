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

#include <inttypes.h>

#ifndef H_SPK_AY
#define H_SPK_AY

class SPK_AY {

	unsigned int tstados_counter_sound;
	unsigned char num_buff;
	unsigned int sound_cuantity; // counter for the buffer
	unsigned char ay_registers[16]; // registers for the AY emulation
	unsigned int aych_a,aych_b,aych_c,aych_n,aych_envel; // counters for AY emulation
	unsigned char ayval_a,ayval_b,ayval_c,ayval_n;

	unsigned char vol_a,vol_b,vol_c;
	unsigned int tst_ay;
	unsigned int tst_ay2;
	uint8_t ay_latch;
	signed char ay_envel_value;
	unsigned char ay_envel_way;
	unsigned char sound_current_value;

public:
	unsigned char ay_emul; // 0: no AY emulation; 1: AY emulation

	SPK_AY();
	void play_ay (int);
	void play_sound (int);
	void reset();
	void set_latch(byte);
	void set_value(byte);
	byte get_latch();
	byte get_value();
	byte get_value(int);
};

extern class SPK_AY *spk_ay;

#endif
