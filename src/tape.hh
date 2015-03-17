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


class Tape {

	class TapeBlock *blocks;
	class TapeBlock *current_block;
	bool paused;

	void add_block(class TapeBlock *);
	void delete_blocks();
	bool load_tap(char *);
	bool load_tzx(char *);
public:
	Tape();
	~Tape();
	void play(uint32_t);
	bool load_file(char *);
	void set_pause(bool pause);
	uint8_t read_signal();
	void rewind();
	bool fast_read(uint16_t,uint16_t,uint8_t);
};

#endif /* SRC_TAPE_HPP_ */
