/*
 * osd.cpp
 *
 *  Created on: 26/03/2015
 *      Author: raster
 */

#include "osd.hh"

OSD::OSD() {
	this->time = 0;
	this->lines = 0;
	this->text = "";
}

void OSD::set_message(string text, uint32_t ms) {

	this->text = text;
	this->lines = 1;
	for(int loop=0;loop< text.length(); loop++) {
		if (text[loop] == '\n') {
			this->lines++;
		}
	}
	this->time = ms/20; // set it to number of frames
}

uint32_t OSD::get_time() {
	if (this->time > 0) {
		this->time--;
	}
	return (this->time);
}

string OSD::get_text(uint8_t &lines) {

	lines = this->lines;
	return this->text;
}

void OSD::clear_message() {
	this->time = 0;
}
