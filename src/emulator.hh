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

#include "computer.hh"
#include "z80free/Z80free.h"

#ifndef emulator_h
#define emulator_h

#define NUM_SNDBUF 2

extern char debug_var;

extern SDL_Surface *screen;
extern Z80FREE procesador;
extern struct computer ordenador;
extern unsigned char *sound[NUM_SNDBUF];
extern char path_snaps[2049];
extern char path_taps[2049];
extern char path_mdrs[2049];
extern unsigned int colors[80];
extern unsigned int jump_frames,curr_frames;

void SDL_Fullscreen_Switch(void);
void load_rom(char);
void load_main_game(char *nombre);
FILE *myfopen(char *filename,char *mode);

#endif
