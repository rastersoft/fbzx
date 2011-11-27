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

#include "z80free/Z80free.h"
#include "computer.h"
#include "emulator.h"
#include "cargador.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "characters.h"
#include "menus.h"
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include "sound.h"
#include "tape.h"
#include "microdrive.h"

char debug_var=1;

Z80FREE procesador;
struct computer ordenador;
SDL_Surface *screen;
char salir,sound_aborted;
unsigned char *sound[NUM_SNDBUF];
char path_snaps[2049];
char path_taps[2049];
char path_mdrs[2049];
unsigned int colors[80];
unsigned int jump_frames,curr_frames;
char *filenames[5];

void SDL_Fullscreen_Switch()
{
	Uint32 flags = screen->flags;
	if ( flags & SDL_FULLSCREEN )
		flags &= ~SDL_FULLSCREEN;
	else
		flags |= SDL_FULLSCREEN;

	screen = SDL_SetVideoMode(screen->w, screen->h, screen->format->BitsPerPixel,flags);
}

FILE *myfopen(char *filename,char *mode) {
	
	char tmp[4096];
	FILE *fichero;
	
	fichero=fopen(filename,mode);
	if (fichero!=NULL) {
		return (fichero);
	}
	sprintf(tmp,"/usr/share/%s",filename);
	fichero=fopen(tmp,mode);
	if (fichero!=NULL) {
		return (fichero);
	}
	sprintf(tmp,"/usr/local/share/%s",filename);
	fichero=fopen(tmp,mode);
	if (fichero!=NULL) {
		return (fichero);
	}
	return (NULL);
}

char *load_a_rom(char **filenames) {
	
	char **pointer;
	int offset=0;
	FILE *fichero;
	int size;
	
	for(pointer=filenames;*pointer!=NULL;pointer++) {
		fichero=myfopen(*pointer,"r");
		if(fichero==NULL) {
			return (*pointer);
		}
		size=fread(ordenador.memoria+offset,16384,1,fichero);
		offset+=16384;
		fclose(fichero);
	}
	return (NULL);
}

void load_rom(char type) {

	char *retval;
	FILE *fichero;
	int size;

	switch(type) {
	case 0:
		filenames[0]="spectrum-roms/48.rom";
		filenames[1]=NULL;
		retval=load_a_rom(filenames);
		if (retval) {
			printf("Can't load file %s\n",retval);
			exit(1);
		}
	break;
	case 1:
		filenames[0]="spectrum-roms/128-0.rom";
		filenames[1]="spectrum-roms/128-1.rom";
		filenames[2]=NULL;
		retval=load_a_rom(filenames);
		if (retval) {
			printf("Can't load file %s\n",retval);
			exit(1);
		}
	break;
	case 2:
		filenames[0]="spectrum-roms/plus2-0.rom";
		filenames[1]="spectrum-roms/plus2-1.rom";
		filenames[2]=NULL;
		retval=load_a_rom(filenames);
		if (retval) {
			printf("Can't load file %s\n",retval);
			exit(1);
		}
	break;
	case 3:
		// first, try last version of PLUS3 roms
		
		filenames[0]="spectrum-roms/plus3-41-0.rom";
		filenames[1]="spectrum-roms/plus3-41-1.rom";
		filenames[2]="spectrum-roms/plus3-41-2.rom";
		filenames[3]="spectrum-roms/plus3-41-3.rom";
		filenames[4]=NULL;
		retval=load_a_rom(filenames);
		if (retval) {
			printf("Can't load the Spectrum +3 ROM version 4.1. Trying with version 4.0\n");
			filenames[0]="spectrum-roms/plus3-40-0.rom";
			filenames[1]="spectrum-roms/plus3-40-1.rom";
			filenames[2]="spectrum-roms/plus3-40-2.rom";
			filenames[3]="spectrum-roms/plus3-40-3.rom";
			filenames[4]=NULL;
			retval=load_a_rom(filenames);
			if (retval) {
				printf("Can't load the Spectrum +3 ROM version 4.0. Trying with legacy filenames\n");
				filenames[0]="spectrum-roms/plus3-0.rom";
				filenames[1]="spectrum-roms/plus3-1.rom";
				filenames[2]="spectrum-roms/plus3-2.rom";
				filenames[3]="spectrum-roms/plus3-3.rom";
				filenames[4]=NULL;
				retval=load_a_rom(filenames);
				if (retval) {
					printf("Can't load file %s\n",retval);
					exit(1);
				}
			}
		}
	break;
	case 4:
		filenames[0]="spectrum-roms/128-spanish-0.rom";
		filenames[1]="spectrum-roms/128-spanish-1.rom";
		filenames[2]=NULL;
		retval=load_a_rom(filenames);
		if (retval) {
			printf("Can't load file %s\n",retval);
			exit(1);
		}
	break;
	}
  
	fichero=myfopen("spectrum-roms/if1-2.rom","r"); // load Interface1 ROM
	if(fichero==NULL) {
		// try legacy name
		fichero=myfopen("spectrum-roms/if1-v2.rom","r");
		if(fichero==NULL) {
			printf("Can't open Interface1 ROM file\n");
			exit(1);
		}
	}
	size=fread(ordenador.shadowrom,8192,1,fichero);
  	fclose(fichero);
}

void init_screen(int resx,int resy,int depth,int fullscreen,int dblbuffer,int hwsurface) {

	int retorno,bucle,bucle2,valores,ret2;
	unsigned char value;

	//if (sound_type!=3)
	retorno=SDL_Init(SDL_INIT_VIDEO);
	/*else
		retorno=SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);*/
	if(retorno!=0) {
		printf("Can't initialize SDL library. Exiting\n");
		exit(1);
	}

	if (SDL_InitSubSystem(SDL_INIT_JOYSTICK)) {
		ordenador.use_js=0;
		printf("Can't initialize JoyStick subsystem\n");
	} else {
		printf("JoyStick subsystem initialized\n");
		ordenador.use_js=1;
		if(SDL_NumJoysticks()>0){
			// Open joystick
			for (bucle=0;bucle<SDL_NumJoysticks();bucle++) {
	  			if (NULL==SDL_JoystickOpen(bucle)) {
	  				printf("Can't open joystick %d\n",bucle);
	  			}
  			}
  		}
	}

	// screen initialization
	valores=SDL_HWPALETTE;//|SDL_ANYFORMAT;
	if (fullscreen==1)
		valores|=SDL_FULLSCREEN;
  
	if (dblbuffer==1)
		valores|=SDL_DOUBLEBUF;
	if (hwsurface==1)
		valores|=SDL_HWSURFACE;
	else
		valores|=SDL_SWSURFACE;
  
	depth=8;
	screen=SDL_SetVideoMode(resx,resy,depth,valores);
	if(screen==NULL) {
		printf("Can't assign SDL Surface. Exiting\n");
		exit(1);
	}

	ordenador.bpp=screen->format->BytesPerPixel;
	printf("Bytes per pixel: %d\n",ordenador.bpp);
  
	if(SDL_MUSTLOCK(screen)) {
		ordenador.mustlock=1;
		SDL_LockSurface(screen);
	} else
		ordenador.mustlock=0;

	// sound initialization

	if (sound_type==SOUND_AUTOMATIC) {
		ret2=sound_init(1); // check all sound systems
	} else {
		ret2=sound_init(0); // try with the one specified in command line
	}
	if(ret2==0) {
		sound_aborted=0;
	} else { // if fails, run without sound
		sound_type=SOUND_NO;
		sound_init(0);
		sound_aborted=1;
	}

	if(ordenador.format)
		ordenador.increment=2*ordenador.channels;
	else
		ordenador.increment=ordenador.channels;
	
	value=0;
	for(bucle2=0;bucle2<NUM_SNDBUF;bucle2++) {
		sound[bucle2]=(unsigned char *)malloc(ordenador.buffer_len*ordenador.increment+4);
		for(bucle=0;bucle<ordenador.buffer_len*ordenador.increment+4;bucle++)
			sound[bucle2][bucle]=value;
			value+=4; 
	}

	ordenador.tst_sample=3500000/ordenador.freq;
	set_volume(70);
}

void end_system() {
	
	sound_close();
	
	if(ordenador.mustlock)
		SDL_UnlockSurface(screen);

	if(ordenador.tap_file!=NULL)
		fclose(ordenador.tap_file);

	SDL_Quit();
}

void load_main_game(char *nombre) {

	int longitud,retval;
	char *puntero;
	
	longitud=strlen(nombre);
	if (longitud<5) {
		return;
	}
	puntero=nombre+(longitud-4);
	if ((0==strcasecmp(".z80",puntero))||(0==strcasecmp(".sna",puntero))) {
		load_z80(nombre);
		return;
	}
	
	if ((0==strcasecmp(".tap",puntero))||(0==strcasecmp(".tzx",puntero))) {
		char char_id[10];
		ordenador.tape_write = 0; // by default, can't record
		ordenador.tap_file=fopen(nombre,"r+"); // read and write
		if(ordenador.tap_file==NULL)
			return;

		strcpy(ordenador.current_tap,nombre);
   
		retval=fread(char_id,10,1,ordenador.tap_file); // read the (maybe) TZX header
		if((!strncmp(char_id,"ZXTape!",7)) && (char_id[7]==0x1A)&&(char_id[8]==1)) {
			ordenador.tape_file_type = TAP_TZX;
			rewind_tape(ordenador.tap_file,1);	  
		} else {
			ordenador.tape_file_type = TAP_TAP;
			rewind_tape(ordenador.tap_file,1);
		}
		return;
	}
}

void save_config(struct computer *object) {
	
	char config_path[1024];
	int length;
	FILE *fconfig;
	
	strcpy(config_path,getenv("HOME"));
	length=strlen(config_path);
	if ((length>0)&&(config_path[length-1]!='/'))
		strcat(config_path,"/");
	strcat(config_path,".fbzx");
	fconfig = fopen(config_path,"wb");
	if (fconfig==NULL) {
		return;
	}
	fprintf(fconfig,"mode=%c%c",48+object->mode128k,10);
	fprintf(fconfig,"issue=%c%c",48+object->issue,10);
	fprintf(fconfig,"joystick=%c%c",48+object->joystick,10);
	fprintf(fconfig,"ay_sound=%c%c",48+object->ay_emul,10);
	fprintf(fconfig,"interface1=%c%c",48+object->mdr_active,10);
	fprintf(fconfig,"doublescan=%c%c",48+object->dblscan,10);
	fprintf(fconfig,"volume=%c%c",65+(object->volume/4),10);
	fprintf(fconfig,"bw=%c%c",48+object->bw,10);
	fclose(fconfig);
}

void load_config(struct computer *object) {
	
	char config_path[1024];
	char line[1024],carac,done;
	int length,pos;
	FILE *fconfig;
	unsigned char volume=255,mode128k=255,issue=255,joystick=255,ay_emul=255,mdr_active=255,dblscan=255,bw=255;
	
	strcpy(config_path,getenv("HOME"));
	length=strlen(config_path);
	if ((length>0)&&(config_path[length-1]!='/'))
		strcat(config_path,"/");
	strcat(config_path,".fbzx");
	fconfig = fopen(config_path,"rb");
	if (fconfig==NULL) {
		return;
	}
	
	done=1;
	pos=0;
	line[0]=0;
	while(!feof(fconfig)) {
		if (done) {
			line[0]=0;
			pos=0;
			done=0;
		}
		if (0!=fread(&carac,1,1,fconfig)) {
			if ((carac!=13)&&(carac!=10)) {
				line[pos]=carac;
				if (pos<1023) {
					pos++;
				}
				continue;
			}
		}
		done=1;
		line[pos]=0;
		if (line[0]=='#') { // coment
			continue;
		}
		if (!strncmp(line,"mode=",5)) {
			mode128k=line[5]-'0';
			continue;
		}
		if (!strncmp(line,"issue=",6)) {
			issue=line[6]-'0';
			continue;
		}
		if (!strncmp(line,"joystick=",9)) {
			joystick=line[9]-'0';
			continue;
		}
		if (!strncmp(line,"ay_sound=",9)) {
			ay_emul=line[9]-'0';
			continue;
		}
		if (!strncmp(line,"interface1=",11)) {
			mdr_active=line[11]-'0';
			continue;
		}
		if (!strncmp(line,"doublescan=",11)) {
			dblscan=line[11]-'0';
			continue;
		}
		if (!strncmp(line,"volume=",7)) {
			volume=4*(line[7]-'A');
			continue;
		}
		if (!strncmp(line,"bw=",3)) {
			bw=(line[3]-'0');
			continue;
		}
	}
	
	if (mode128k<5) {
		object->mode128k=mode128k;
	}
	if (issue<4) {
		object->issue=issue;
	}
	if (joystick<4) {
		object->joystick=joystick;
	}
	if (ay_emul<2) {
		object->ay_emul=ay_emul;
	}
	if (mdr_active<2) {
		object->mdr_active=mdr_active;
	}
	if (dblscan<2) {
		object->dblscan=dblscan;
	}
	if (bw<2) {
		object->bw=bw;
	}
	if (volume<255) {
		object->volume=volume;
		set_volume(volume);
	}
	
	fclose(fconfig);
}

int main(int argc,char *argv[]) {

	int bucle,tstados,argumento,fullscreen,dblbuffer,hwsurface,length;
	char gamefile[4096];
	word PC=0;

	// by default, try all sound modes
	sound_type=SOUND_AUTOMATIC;
	gamefile[0]=0;
	ordenador.zaurus_mini=0;
	ordenador.ulaplus=0;
	ordenador.ulaplus_reg=0;
	fullscreen=0;
	dblbuffer=0;
	hwsurface=0;
	
	argumento=0;
	jump_frames=0;
	curr_frames=0;
	ordenador.dblscan=0;
	ordenador.bw=0;

	// load current config
	load_config(&ordenador);

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
			printf("  -mini: show screen at 320x240\n");
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
			sound_type=SOUND_NO;
			argumento++;
		} else if(0==strcmp(argv[argumento],"-mini")) {
			ordenador.zaurus_mini=1;
			argumento++;
#ifdef D_SOUND_PULSE
		} else if(0==strcmp(argv[argumento],"-pulse")) {
			sound_type=SOUND_PULSEAUDIO;
			argumento++;
#endif
#ifdef D_SOUND_ALSA
		} else if(0==strcmp(argv[argumento],"-alsa")) {
			sound_type=SOUND_ALSA;
			argumento++;
#endif
#ifdef D_SOUND_OSS
		} else if(0==strcmp(argv[argumento],"-oss")) {
			sound_type=SOUND_OSS;
			argumento++;
#endif
		} else if(0==strcmp(argv[argumento],"-rotate")) {
			ordenador.zaurus_mini=2;
			argumento++;
		} else if(0==strcmp(argv[argumento],"-fs")) {
			fullscreen=1;
			argumento++;
		} else if(0==strcmp(argv[argumento],"-hw")) {
			hwsurface=1;
			argumento++;
		} else if(0==strcmp(argv[argumento],"-db")) {
			dblbuffer=1;
			argumento++;
		} else if(0==strcmp(argv[argumento],"-ds")) {
			ordenador.dblscan=1;
			argumento++;
		} else if(0==strcmp(argv[argumento],"-bw")) {
			ordenador.bw=1;
			argumento++;
		} else if(0==strcmp(argv[argumento],"-color")) {
			ordenador.bw=0;
			argumento++;
		} else if(0==strcmp(argv[argumento],"-ss")) {
			ordenador.dblscan=0;
			argumento++;
		} else if(0==strncmp(argv[argumento],"-jump",5)) {
			jump_frames=(int)(argv[argumento][5]);
			jump_frames-=48;
			argumento++;
			printf ("Jump %d\n",jump_frames);
		} else {
			strcpy(gamefile,argv[argumento]);
			argumento++;
		}
	}
	
	atexit(end_system);
	switch(ordenador.zaurus_mini) {
	case 0:
		init_screen(640,480,0,0,dblbuffer,hwsurface);
	break;
	case 1:
	case 2:
		init_screen(480,640,0,0,dblbuffer,hwsurface);
	break;
	
	}

	computer_init();
	register_screen(screen);
	if(fullscreen) {
		SDL_Fullscreen_Switch();
	}
	SDL_WM_SetCaption("FBZX","");
	ordenador.interr=0;

	ordenador.screenbuffer=ordenador.screen->pixels;
	ordenador.screen_width=ordenador.screen->w;

	// assign initial values for PATH variables

	strcpy(path_snaps,getenv("HOME"));
	length=strlen(path_snaps);
	if ((length>0)&&(path_snaps[length-1]!='/'))
		strcat(path_snaps,"/");
	strcpy(path_taps,path_snaps);
	strcpy(path_mdrs,path_snaps);
	ordenador.current_tap[0]=0;

	// assign random values to the memory before start execution

	for(bucle=0;bucle<196608;bucle++)
		ordenador.memoria[bucle]=(unsigned char) rand();

	ordenador.tap_file=NULL;
	
	// we filter all the events, except keyboard events

	SDL_EventState(SDL_ACTIVEEVENT,SDL_IGNORE);
	SDL_EventState(SDL_MOUSEMOTION,SDL_IGNORE);
	SDL_EventState(SDL_MOUSEBUTTONDOWN,SDL_IGNORE);
	SDL_EventState(SDL_MOUSEBUTTONUP,SDL_IGNORE);
	SDL_EventState(SDL_JOYAXISMOTION,SDL_ENABLE);
	SDL_EventState(SDL_JOYBALLMOTION,SDL_ENABLE);
	SDL_EventState(SDL_JOYHATMOTION,SDL_ENABLE);
	SDL_EventState(SDL_JOYBUTTONDOWN,SDL_ENABLE);
	SDL_EventState(SDL_JOYBUTTONUP,SDL_ENABLE);
	SDL_EventState(SDL_QUIT,SDL_ENABLE);
	SDL_EventState(SDL_SYSWMEVENT,SDL_IGNORE);
	SDL_EventState(SDL_VIDEORESIZE,SDL_IGNORE);
	SDL_EventState(SDL_USEREVENT,SDL_IGNORE);

	SDL_ShowCursor(SDL_DISABLE);
	salir=1;
  
	microdrive_init();

	ResetComputer();

	sleep(1);

	clean_screen();

	if (sound_aborted==1) {
		strcpy(ordenador.osd_text,"Running without sound (read the FAQ)");
		ordenador.osd_time=100;
	}

	load_main_game(gamefile);

	sprintf(ordenador.osd_text,"Press F1 for help");
	ordenador.osd_time=200;

	printf("BPP: %d\n",ordenador.bpp);
	while(salir) {

		do {
			tstados=Z80free_ustep(&procesador);
			if(tstados<0) {
				printf("Error %X\n",procesador.PC);
				exit(1);
			}
			emulate(tstados); // execute the whole hardware emulation for that number of TSTATES
		} while(procesador.Status!=Z80XX);
			
		PC=procesador.PC;
				
		/* if PC is 0x0556, a call to LD_BYTES has been made, so if
		FAST_LOAD is 1, we must load the block in memory and return */

		if((!ordenador.mdr_paged)&&(PC==0x0556) && (ordenador.tape_fast_load==1)&&(ordenador.tape_file_type==TAP_TAP)) {
			if(ordenador.tap_file!=NULL)
				fastload_block(ordenador.tap_file);
			else {
				sprintf(ordenador.osd_text,"No TAP file selected");
				ordenador.osd_time=50;
			}
		}
		
		/* if PC is 0x04C2, a call to SA_BYTES has been made, so if
		we want to save to the TAP file, we do it */
		
		if((!ordenador.mdr_paged)&&(PC==0x04C2)&&(ordenador.tape_write==1)&&(ordenador.tape_file_type==TAP_TAP)) {
			if(ordenador.tap_file!=NULL)
				save_file(ordenador.tap_file);
			else {
				sprintf(ordenador.osd_text,"No TAP file selected");
				ordenador.osd_time=50;
			}
		}
		
		/* if ordenador.mdr_paged is 2, we have executed the RET at 0x0700, so
		we have to return to the classic ROM */
		
		if(ordenador.mdr_paged==2)
			ordenador.mdr_paged=0;
		
		/* if PC is 0x0008 or 0x1708, and we have a microdrive, we have to page
		the Interface 1 ROM */
		
		if(((PC==0x0008)||(PC==0x1708))&&(ordenador.mdr_active))
			ordenador.mdr_paged = 1;
		
		/* if PC is 0x0700 and we have a microdrive, we have to unpage
		the Interface 1 ROM after the last instruction */
		
		if((PC==0x0700)&&(ordenador.mdr_active))
			ordenador.mdr_paged = 2;

		if(ordenador.interr==1) {
			read_keyboard (NULL);	// read the physical keyboard
			Z80free_INT(&procesador,bus_empty());
			ordenador.interr=0;
		}
	}

	save_config(&ordenador);
	return 0;
}
