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

/*
 * tape.hpp
 *
 *  Created on: 15/03/2015
 *      Author: raster
 */

using namespace std;
#include <string>
#include <inttypes.h>
#include <stdio.h>

#include "computer.hh"
#include "emulator.hh"
#include "signals.hh"

#ifndef SRC_TAPE_HPP_
#define SRC_TAPE_HPP_

/*
class TapeSignal {

	class Tape *tape_object;
	~TapeSignal();
public:
	virtual void callback_pause(bool pause) = 0;
	void register_tape_object(class Tape *tape);
};

typedef void (TapeSignal::*signal_pause) (bool);
*/

enum FastLoadReturn {FASTLOAD_OK, FASTLOAD_NO_BLOCK, FASTLOAD_NO_TAPE, FASTLOAD_NO_FLAG, FASTLOAD_END_TAPE};

class Tape : public Signals {

	class TapeBlock *blocks;
	class TapeBlock *current_block;
	bool paused;
	bool block_accesed;

	void add_block(class TapeBlock *);
	void delete_blocks();
	bool load_tap(string);
	bool load_tzx(string);
public:
	Tape();
	~Tape();
	/**
	 * Emulates the tape for an specified amount of time
	 * @param tstates The number of tstates to emulate
	 */
	void play(uint32_t tstates);
	/**
	 * Loads a TAP or TZX file. It also pauses the tape.
	 * @param filename The full path to the file
	 * @return TRUE if there was an error; FALSE if the file was loaded fine
	 */
	bool load_file(string);
	/**
	 * Pauses or releases the tape emulation
	 * @param pause TRUE to pause the tape, FALSE to release it
	 */
	void set_pause(bool pause);

	/**
	 * Returns whether the tape is paused or not
	 * @return TRUE if paused; FALSE if not paused
	 */
	bool get_pause();
	/**
	 * Reads the current signal being "emited" by the tape.
	 * @return the current signal value, which can be 0 or 1.
	 */
	uint8_t read_signal();
	/**
	 * Rewinds the tape, putting it at the start, ready to begin again.
	 */
	void rewind();
	/**
	 * Tries to fast-read the next block in the tape
	 * @param data Where to return the block's data
	 * @param length The length of the data
	 * @param the desired flag
	 * @return FASTLOAD_OK if everything went fine; FASTLOAD_NO_BLOCK if the next block can't be fast-loaded; FASTLOAD_NO_FLAG if the flag is not valid; FASTLOAD_END_TAPE if the tape reached end and must be rewind
	 */
	enum FastLoadReturn fast_read(uint8_t *data,uint16_t &length,uint8_t flag);

	void send_pause_signal();
	void send_pause_signal48k();
};

#endif /* SRC_TAPE_HPP_ */
