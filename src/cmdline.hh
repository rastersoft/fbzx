/*
 * cmdline.hh
 *
 *  Created on: 26/03/2015
 *      Author: raster
 */

#ifndef SRC_CMDLINE_HH_
#define SRC_CMDLINE_HH_

class CMDLine {

public:

	bool nosound;
	bool mini;
	bool pulse;
	bool alsa;
	bool oss;
	bool rotate;
	bool fs;
	bool hw;
	bool db;
	bool ds;
	bool bw;
	bool color;
	bool ss;
	int jump;
	string gamefile;

	CMDLine(int argc,char **argv);


};



#endif /* SRC_CMDLINE_HH_ */
