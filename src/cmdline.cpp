/*
 * cmdline.cpp
 *
 *  Created on: 26/03/2015
 *      Author: raster
 */

using namespace std;
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cmdline.hh"

CMDLine::CMDLine(int argc,char **argv) {

	int argumento;

	this->nosound = false;
	this->mini = false;
	this->pulse = false;
	this->alsa = false;
	this->oss = false;
	this->rotate = false;
	this->fs = false;
	this->hw = false;
	this->db = false;
	this->ds = false;
	this->bw = false;
	this->color = false;
	this->ss = false;
	this->jump = 0;
	this->gamefile = "";

	argumento = 0;

	while(argumento<argc) {
		if ((0==strcmp(argv[argumento],"-h"))||(0==strcmp(argv[argumento],"--help"))) {
			printf("\nUsage: fbzx [-nosound] ");
#ifdef D_SOUND_ALSA
			printf("[-alsa] ");
#endif
#ifdef D_SOUND_OSS
			printf("[-oss] ");
#endif
#ifdef D_SOUND_PULSE
			printf("[-pulse] ");
#endif

			printf("[-mini] [-rotate] [-fs] [-hw] [-db] [-ds] [-ss] [-jump N] [gamefile]\n");
			printf("\n  -nosound: don't emulate sound\n");

#ifdef D_SOUND_ALSA
			printf("  -alsa: use ALSA for sound output\n");
#endif
#ifdef D_SOUND_OSS
			printf("  -oss: use OSS for sound output\n");
#endif
#ifdef D_SOUND_PULSE
			printf("  -pulse: use PulseAudio for sound output (default)\n");
#endif
			printf("  -mini: show screen at 320x240 in a rotated 640x480 screen\n");
			printf("  -rotate: rotate screen clockwise\n");
			printf("  -fs: start FBZX at fullscreen\n");
			printf("  -hw: use hardware buffer (best for console framebuffer)\n");
			printf("  -db: use double buffer\n");
			printf("  -ds: use doublescan (don't emulate TV black stripes)\n");
			printf("  -ss: force singlescan (emulate TV black stripes)\n");
			printf("  -bw: emulate black&white TV set\n");
			printf("  -color: emulate a color TV set\n");
			printf("  -jump N: show one TV refresh and jump over N refreshes (for slow systems)\n");
			printf("   gamefile: an optional .Z80 snapshot or .TAP/.TZX tape file\n\n");
			exit(0);
		} else if(0==strcmp(argv[argumento],"-nosound")) {
			this->nosound = true;
			argumento++;
		} else if(0==strcmp(argv[argumento],"-mini")) {
			this->mini = true;
			argumento++;
#ifdef D_SOUND_PULSE
		} else if(0==strcmp(argv[argumento],"-pulse")) {
			this->pulse = true;
			argumento++;
#endif
#ifdef D_SOUND_ALSA
		} else if(0==strcmp(argv[argumento],"-alsa")) {
			this->alsa = true;
			argumento++;
#endif
#ifdef D_SOUND_OSS
		} else if(0==strcmp(argv[argumento],"-oss")) {
			this->oss = true;
			argumento++;
#endif
		} else if(0==strcmp(argv[argumento],"-rotate")) {
			this->rotate = true;
			argumento++;
		} else if(0==strcmp(argv[argumento],"-fs")) {
			this->fs = true;
			argumento++;
		} else if(0==strcmp(argv[argumento],"-hw")) {
			this->hw = true;
			argumento++;
		} else if(0==strcmp(argv[argumento],"-db")) {
			this->db = true;
			argumento++;
		} else if(0==strcmp(argv[argumento],"-ds")) {
			this->ds = true;
			argumento++;
		} else if(0==strcmp(argv[argumento],"-bw")) {
			this->bw = true;
			argumento++;
		} else if(0==strcmp(argv[argumento],"-color")) {
			this->color = true;
			argumento++;
		} else if(0==strcmp(argv[argumento],"-ss")) {
			this->ss = true;
			argumento++;
		} else if(0==strncmp(argv[argumento],"-jump",5)) {
			/*jump_frames=(int)(argv[argumento][5]);
			jump_frames-=48;*/
			this->jump = (int)(argv[argumento][5]);
			this->jump -= '0';
			argumento++;
		} else {
			this->gamefile = argv[argumento];
			//strcpy(this->gamefile,argv[argumento]);
			argumento++;
		}
	}
}

