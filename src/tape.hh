/*
 * tape.hpp
 *
 *  Created on: 15/03/2015
 *      Author: raster
 */

#include <inttypes.h>
#include <stdio.h>

#include "computer.hh"

#ifndef SRC_TAPE_HPP_
#define SRC_TAPE_HPP_

enum FastLoadReturn {FASTLOAD_OK, FASTLOAD_NO_BLOCK, FASTLOAD_BLOCK_SHORT, FASTLOAD_BLOCK_LONG};

class Tape {

	class TapeBlock *blocks;
	class TapeBlock *current_block;
	bool paused;
	bool block_accesed;

	void add_block(class TapeBlock *);
	void delete_blocks();
	bool load_tap(char *);
	bool load_tzx(char *);
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
	bool load_file(char *filename);
	/**
	 * Pauses or releases the tape emulation
	 * @param pause TRUE to pause the tape, FALSE to release it
	 */
	void set_pause(bool pause);
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
	 * @param address The processor address where to load the block
	 * @param length The desired length to read
	 * @param the flag value
	 * @return FASTLOAD_OK if everything went fine; FASTLOAD_NO_BLOCK if the next block can't be fast-loaded; FASTLOAD_BLOCK_SHORT if the loaded block is shorter than the desired size; FASTLOAD_BLOCK_LONG if the loaded block is larger than the desired size
	 */
	enum FastLoadReturn fast_read(uint16_t address,uint16_t length,uint8_t flag);
};

#endif /* SRC_TAPE_HPP_ */
