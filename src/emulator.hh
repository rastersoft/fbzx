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

#include "z80free/Z80free.h"
#include "osd.hh"
#include "screen.hh"

#ifndef emulator_h
#define emulator_h

#define NUM_SNDBUF 2

extern bool debug_var;

extern Z80FREE procesador;
extern char path_snaps[2049];
extern char path_taps[2049];
extern char path_mdrs[2049];
extern unsigned int jump_frames,curr_frames;

void load_rom(char);
void load_main_game(const char *nombre);
void do_fast_load();
void print_status();
void do_push(uint16_t value);

#endif
