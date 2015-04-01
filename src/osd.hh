/*
 * osd.hh
 *
 *  Created on: 26/03/2015
 *      Author: raster
 */

#ifndef SRC_OSD_HH_
#define SRC_OSD_HH_

using namespace std;
#include <string>
#include <stdint.h>

extern class OSD *osd;

class OSD {

	string text;
	uint8_t lines;
	uint32_t time;

public:
	OSD();
	void set_message(string, uint32_t);
	uint32_t get_time();
	string get_text(uint8_t &);
	void clear_message();

};


#endif /* SRC_OSD_HH_ */
