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

enum e_soundtype {SOUND_NO, SOUND_OSS, SOUND_ALSA, SOUND_PULSEAUDIO, SOUND_AUTOMATIC};

extern enum e_soundtype sound_type;

int sound_init();
void sound_play();
void sound_close();

extern volatile unsigned char *sdl_sound_buffer;

int sound_init_oss();
int sound_init_alsa();
int sound_init_pulse();

void sdlcallback(void *userdata, Uint8 *stream, int len);

#endif
