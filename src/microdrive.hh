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

#ifndef H_MICRODRIVE
#define H_MICRODRIVE

#include <inttypes.h>
#include <stdio.h>
#include "z80free/Z80free.h"

class Microdrive {

	void increment_head();
	void restart();
	bool save_cartridge();

	uint32_t mdr_tapehead; // current position in the tape
	uint32_t mdr_bytes;      // number of bytes read or written in this transfer
	uint32_t mdr_maxbytes; // maximum number of bytes to read or write in this transfer
	uint32_t mdr_gap;         // TSTATEs remaining for GAP end
	uint32_t mdr_nogap;      // TSTATEs remaining for next GAP
	uint8_t mdr_cartridge[137923]; // current cartridge
	uint8_t mdr_drive; // current drive
	byte mdr_old_STATUS; // to detect an edge in COM CLK
	uint8_t mdr_modified; // if a sector is stored, this change to know that it must be stored in the file
	byte trash;

public:
	Microdrive();
	void reset();
	byte in(word);
	void out(word,byte);
	void emulate(int);
	int select_mdrfile(char *);
	int new_mdrfile(char *);
	bool get_protected();
	void set_protected(bool);

	bool mdr_active; // 0: not installed; 1: installed
	uint8_t mdr_paged; // 0: not pagined; 1: pagined
	char mdr_current_mdr[2049]; // current path and name for microdrive file
};

extern class Microdrive *microdrive;
#endif
