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

#ifndef SOUND_H
#define SOUND_H
#include <inttypes.h>
#include <SDL/SDL.h>

#if 0
#define D_SOUND_PULSE
#define D_SOUND_ALSA
#define D_SOUND_OSS
#endif

enum e_soundtype {SOUND_NO, SOUND_OSS, SOUND_ALSA, SOUND_PULSEAUDIO, SOUND_AUTOMATIC};

extern class LLSound *llsound;

class LLSound {

	uint8_t format; // 0: 8 bits, 1: 16 bits LSB, 2: 16 bits MSB
	uint8_t channels; // number of channels

#ifdef D_SOUND_OSS
	int init_oss();
#endif
#ifdef D_SOUND_ALSA
	int init_alsa();
#endif
#ifdef D_SOUND_PULSE
	int init_pulse();
#endif
	void remove_dc(unsigned char *sound_buffer,int size);
	int init_sound();
	void show_volume();

public:
	uint32_t freq; // frequency for reproduction
	uint32_t tst_sample; // number of tstates per sample
	int8_t sign; // 0: unsigned; 1: signed
	bool sound_aborted;
	unsigned char *sound;
	unsigned char *current_buffer;
	uint32_t buffer_len; // sound buffer length (in samples)
	uint32_t increment; // cuantity to add to jump to the next sample
	uint8_t volume; // volume
	enum e_soundtype sound_type;

	LLSound(enum e_soundtype);
	~LLSound();
	void play();
	void increase_volume();
	void decrease_volume();
	void set_volume(uint8_t);
	void set_speed(bool);
};

extern volatile unsigned char *sdl_sound_buffer;

void sdlcallback(void *userdata, Uint8 *stream, int len);

#endif
