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

#include <string.h>

#include "llscreen.hh"
#include "font.h"
#include "osd.hh"

LLScreen *llscreen = NULL;

LLScreen::LLScreen(int16_t resx,int16_t resy,uint8_t depth,bool fullscreen,bool dblbuffer,bool hwsurface) {

	int retorno,bucle,valores;

	this->rotate = false;
	this->cheight = (charset.max_top > charset.height_maxtop) ? charset.max_top : charset.height_maxtop;
	this->lines_in_screen = 480 / this->cheight;

	retorno=SDL_Init(SDL_INIT_VIDEO);
	if(retorno!=0) {
		printf("Can't initialize SDL library. Exiting\n");
		exit(1);
	}

	if (SDL_InitSubSystem(SDL_INIT_JOYSTICK)) {
		this->joystick = false;
		printf("Can't initialize JoyStick subsystem\n");
	} else {
		printf("JoyStick subsystem initialized\n");
		this->joystick = true;
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
	valores=SDL_HWPALETTE|SDL_ANYFORMAT;
	if (fullscreen) {
		valores|=SDL_FULLSCREEN;
	}

	if (dblbuffer) {
		valores|=SDL_DOUBLEBUF;
	}

	if (hwsurface) {
		valores|=SDL_HWSURFACE;
	} else {
		valores|=SDL_SWSURFACE;
	}

	this->llscreen=SDL_SetVideoMode(resx,resy,depth,valores);
	if(this->llscreen==NULL) {
		printf("Can't assign SDL Surface. Exiting\n");
		exit(1);
	}

	this->bpp = this->llscreen->format->BytesPerPixel;
	this->width = this->llscreen->w;
	this->memory = (unsigned char *)this->llscreen->pixels;
	printf("Bytes per pixel: %d\n",this->bpp);

	if(SDL_MUSTLOCK(this->llscreen)) {
		this->mustlock = true;
		SDL_LockSurface(this->llscreen);
	} else
		this->mustlock = false;

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

	printf("Locking screen\n");
}

FILE *LLScreen::myfopen(char *filename,char *mode) {

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

LLScreen::~LLScreen(){
	if(this->mustlock) {
		SDL_UnlockSurface(this->llscreen);
	}
	SDL_Quit();
}

void LLScreen::do_flip() {

	if (this->mustlock) {
		SDL_UnlockSurface (this->llscreen);
		SDL_Flip(this->llscreen);
		SDL_LockSurface(this->llscreen);
	} else {
		SDL_Flip(this->llscreen);
	}
}

void LLScreen::paint_one_pixel(uint8_t value,unsigned char *address) {

	unsigned char *colour = (unsigned char *)(this->colors+value);
#if __BYTE_ORDER == __LITTLE_ENDIAN
	switch(this->bpp) {
	case 1:
		*address=*colour;
	break;
	case 3:
		*(address++)=*(colour++);
	case 2:
		*(address++)=*(colour++);
		*(address++)=*(colour++);
	break;
	case 4:
		*((unsigned int *)address)=*((unsigned int *)colour);
	break;
	}
#else // BIG_ENDIAN
	switch(this->bpp) {
		case 1:
			*address=*(colour+3);
		break;
		case 3:
			*(address++)=*(++colour);
		case 2:
			*(address++)=*(++colour);
			*(address++)=*(++colour);
		break;
		case 4:
			*((unsigned int *)address)=*((unsigned int *)colour);
		break;
	}
#endif
}

void LLScreen::set_palete_entry(uint8_t entry, uint8_t Value, bool bw) {


	SDL_Color color;

	this->ulaplus_palete[entry]=Value;

	color.r = ((Value<<3)&0xE0)+((Value)&0x1C)+((Value>>3)&0x03);
	color.g = (Value&0xE0)+((Value>>3)&0x1C)+((Value>>6)&0x03);
	color.b = ((Value<<6)&0xC0)+((Value<<4)&0x30)+((Value<<2)&0x0C)+((Value)&0x03);

	if (bw) {
		int final;
		final=(((int)color.r)*3+((int)color.g)*6+((int)color.b))/10;
		color.r=color.g=color.b=(unsigned char)final;
	}
	// Color mode

	SDL_SetColors (this->llscreen, &color, 32+entry, 1); // set 16 colors from the 16th

	if (this->bpp != 1) {
		colors[entry+16]=SDL_MapRGB(this->llscreen->format,color.r,color.g,color.b);
	}
}

uint8_t LLScreen::get_palete_entry(uint8_t entry) {
	return (this->ulaplus_palete[entry]);
}

void LLScreen::fullscreen_switch() {

	Uint32 flags = this->llscreen->flags;
	if ( flags & SDL_FULLSCREEN )
		flags &= ~SDL_FULLSCREEN;
	else
		flags |= SDL_FULLSCREEN;

	this->llscreen = SDL_SetVideoMode(this->llscreen->w, this->llscreen->h, this->llscreen->format->BitsPerPixel,flags);
}

void LLScreen::set_paletes(bool bw) {

	SDL_Color colores[16];

	if (!bw) {
		// Color mode

		colores[0].r = 0;
		colores[0].g = 0;
		colores[0].b = 0;
		colores[1].r = 0;
		colores[1].g = 0;
		colores[1].b = 192;
		colores[2].r = 192;
		colores[2].g = 0;
		colores[2].b = 0;
		colores[3].r = 192;
		colores[3].g = 0;
		colores[3].b = 192;
		colores[4].r = 0;
		colores[4].g = 192;
		colores[4].b = 0;
		colores[5].r = 0;
		colores[5].g = 192;
		colores[5].b = 192;
		colores[6].r = 192;
		colores[6].g = 192;
		colores[6].b = 0;
		colores[7].r = 192;
		colores[7].g = 192;
		colores[7].b = 192;
		colores[8].r = 0;
		colores[8].g = 0;
		colores[8].b = 0;
		colores[9].r = 0;
		colores[9].g = 0;
		colores[9].b = 255;
		colores[10].r = 255;
		colores[10].g = 0;
		colores[10].b = 0;
		colores[11].r = 255;
		colores[11].g = 0;
		colores[11].b = 255;
		colores[12].r = 0;
		colores[12].g = 255;
		colores[12].b = 0;
		colores[13].r = 0;
		colores[13].g = 255;
		colores[13].b = 255;
		colores[14].r = 255;
		colores[14].g = 255;
		colores[14].b = 0;
		colores[15].r = 255;
		colores[15].g = 255;
		colores[15].b = 255;

		SDL_SetColors (this->llscreen, colores, 16, 16);	// set 16 colors from the 16th

		if (this->bpp != 1) {
			colors[0]=SDL_MapRGB(this->llscreen->format,0,0,0);
			colors[1]=SDL_MapRGB(this->llscreen->format,0,0,192);
			colors[2]=SDL_MapRGB(this->llscreen->format,192,0,0);
			colors[3]=SDL_MapRGB(this->llscreen->format,192,0,192);
			colors[4]=SDL_MapRGB(this->llscreen->format,0,192,0);
			colors[5]=SDL_MapRGB(this->llscreen->format,0,192,192);
			colors[6]=SDL_MapRGB(this->llscreen->format,192,192,0);
			colors[7]=SDL_MapRGB(this->llscreen->format,192,192,192);
			colors[8]=SDL_MapRGB(this->llscreen->format,0,0,0);
			colors[9]=SDL_MapRGB(this->llscreen->format,0,0,255);
			colors[10]=SDL_MapRGB(this->llscreen->format,255,0,0);
			colors[11]=SDL_MapRGB(this->llscreen->format,255,0,255);
			colors[12]=SDL_MapRGB(this->llscreen->format,0,255,0);
			colors[13]=SDL_MapRGB(this->llscreen->format,0,255,255);
			colors[14]=SDL_MapRGB(this->llscreen->format,255,255,0);
			colors[15]=SDL_MapRGB(this->llscreen->format,255,255,255);
		}
	} else {

		// B&W mode

		colores[0].r = 0;
		colores[0].g = 0;
		colores[0].b = 0;

		colores[1].r = 22;
		colores[1].g = 22;
		colores[1].b = 22;

		colores[2].r = 57;
		colores[2].g = 57;
		colores[2].b = 57;

		colores[3].r = 79;
		colores[3].g = 79;
		colores[3].b = 79;

		colores[4].r = 113;
		colores[4].g = 113;
		colores[4].b = 113;

		colores[5].r = 135;
		colores[5].g = 135;
		colores[5].b = 135;

		colores[6].r = 160;
		colores[6].g = 160;
		colores[6].b = 160;

		colores[7].r = 192;
		colores[7].g = 192;
		colores[7].b = 192;

		colores[8].r = 0;
		colores[8].g = 0;
		colores[8].b = 0;

		colores[9].r = 29;
		colores[9].g = 29;
		colores[9].b = 29;

		colores[10].r = 76;
		colores[10].g = 76;
		colores[10].b = 76;

		colores[11].r = 105;
		colores[11].g = 105;
		colores[11].b = 105;

		colores[12].r = 150;
		colores[12].g = 150;
		colores[12].b = 150;

		colores[13].r = 179;
		colores[13].g = 179;
		colores[13].b = 179;

		colores[14].r = 226;
		colores[14].g = 226;
		colores[14].b = 226;

		colores[15].r = 255;
		colores[15].g = 255;
		colores[15].b = 255;

		SDL_SetColors (this->llscreen, colores, 16, 16);	// set 16 colors from the 16th

		if (this->bpp!=1) {
			colors[0]=SDL_MapRGB(this->llscreen->format,0,0,0);
			colors[1]=SDL_MapRGB(this->llscreen->format,22,22,22);
			colors[2]=SDL_MapRGB(this->llscreen->format,57,57,57);
			colors[3]=SDL_MapRGB(this->llscreen->format,79,79,79);
			colors[4]=SDL_MapRGB(this->llscreen->format,113,113,113);
			colors[5]=SDL_MapRGB(this->llscreen->format,135,135,135);
			colors[6]=SDL_MapRGB(this->llscreen->format,160,160,160);
			colors[7]=SDL_MapRGB(this->llscreen->format,192,192,192);
			colors[8]=SDL_MapRGB(this->llscreen->format,0,0,0);
			colors[9]=SDL_MapRGB(this->llscreen->format,29,29,29);
			colors[10]=SDL_MapRGB(this->llscreen->format,76,76,76);
			colors[11]=SDL_MapRGB(this->llscreen->format,105,105,105);
			colors[12]=SDL_MapRGB(this->llscreen->format,150,150,150);
			colors[13]=SDL_MapRGB(this->llscreen->format,179,179,179);
			colors[14]=SDL_MapRGB(this->llscreen->format,226,226,226);
			colors[15]=SDL_MapRGB(this->llscreen->format,255,255,255);
		}
	}

	unsigned int c;

	for(c=0x10;c<60;c++) {
		colors[c]=0x00000000;
	}

	if (this->bpp == 1) {
		unsigned int v;
		for (c=0x10;c<0x60;c++) {
			v=c+((c<<8)&0x0000FF00)+((c<<16)&0x00FF0000)+((c<<24)&0xFF000000);
			colors[c-0x10]=v;
		}
	}
	for(c=0;c<64;c++) {
		set_palete_entry((unsigned char)c,this->ulaplus_palete[c],bw);
	}

}

// prints the ASCII character CHARAC in the framebuffer MEMO, at X,Y with ink color COLOR and paper color BACK, asuming that the screen width is WIDTH

uint8_t LLScreen::printchar(uint8_t carac, int16_t x, int16_t y, uint8_t color, uint8_t back) {

	int bucle1, bucle2, offset;
	unsigned char *lugar, *lugar2;
	uint8_t width;
	uint8_t width2;
	uint8_t height;
	uint8_t height2;
	int8_t left;
	int8_t top;
	int8_t right;
	int8_t bottom;
	uint8_t *counter;
	int8_t *counter2;

	carac-=32;

	offset = charset.offsets[carac];
	counter = charset.data+offset;
	width = *(counter++);
	height = *(counter++);
	counter2 = (int8_t *) counter;
	left = *(counter2++);
	top = *(counter2++);
	right = *(counter2++);
	bottom = *(counter2++);
	counter+=4;

	//y+=charset.max_top;
	lugar=this->memory + (y * this->width + x) * this->bpp;

	if (width < MIN_WIDTH) {
		width2 = MIN_WIDTH;
	} else {
		width2 = width+1;
	}

	for (bucle1=0;bucle1<this->cheight;bucle1++) {
		lugar2=lugar;
		if (bucle1+y >= 480) {
			break;
		}
		for (bucle2=0;bucle2 < width2;bucle2++) {
			if ((bucle2 < width) && (bucle1>=(charset.max_top-top-charset.min_top-1) && (bucle1 < (charset.max_top-top+height-charset.min_top-1))&& ((*(counter++))>127))) {
				paint_one_pixel (color, lugar2);
			} else {
				paint_one_pixel (back, lugar2);
			}
			lugar2+=this->bpp;
		}
		lugar+=(this->width * this->bpp);
	}
	return width2;
}

void LLScreen::paint_picture(string filename) {

	FILE *fichero;
	int bucle1,bucle2;
	unsigned char *buffer,*buffer2,valor;

	buffer=(unsigned char *)this->llscreen->pixels;

	this->clear_screen();
	fichero=myfopen((char *)filename.c_str(),"r");
	if (fichero==NULL) {
		osd->set_message("Keymap picture not found",2000);
		return;
	}
	if (!this->rotate) {
		for (bucle1=0;bucle1<344;bucle1++)
			for(bucle2=0;bucle2<640;bucle2++) {
				fscanf(fichero,"%c",&valor);
				paint_one_pixel(valor,buffer);
				buffer += this->bpp;
			}
	} else {
		buffer += (479 * this->bpp);
		for(bucle1 = 0;bucle1 < 344;bucle1++) {
			buffer2 = buffer;
			for(bucle2 = 0;bucle2 < 640;bucle2++) {
				fscanf(fichero,"%c",&valor);
				paint_one_pixel(valor,buffer);
				buffer += (480 * this->bpp);
			}
			buffer = buffer2 - this->bpp;
		}
	}

}

void LLScreen::clear_screen() {

	int bucle;
	unsigned char *buffer;

	buffer=(unsigned char *)this->llscreen->pixels;

	for(bucle=0;bucle<((this->llscreen->h)*(this->llscreen->w)*(this->bpp));bucle++)
		*(buffer++)=0;

}

// prints the string CADENA in X,Y (centered if X=-1), with colors COLOR and BACK

void LLScreen::print_string(string o_cadena, int16_t x, int16_t y, uint8_t ink, uint8_t paper) {

	int length, ncarac, bucle, xx;
	uint16_t xxx, yyy;
	unsigned char *cadena = (unsigned char *)o_cadena.c_str();
	unsigned char *str2;

	length = 0;
	ncarac = 0;
	for (str2=cadena;*str2;str2++) {
		uint8_t c;
		uint8_t l;
		uint32_t offset;

		c = *str2;
		if ( c >=' ') {
			ncarac++;
			offset = charset.offsets[c-32];
			l = *(charset.data + offset);
			if (l < MIN_WIDTH) {
				l = MIN_WIDTH;
			} else {
				l++;
			}
			length += l;
		}
	}

	if (length > this->width) {
		if (x>=0)
			xx=x;
		else
			xx=0;
	} else {
		if (x == -1) // we want it centered
			xx=(this->width / 2) - (length / 2);
		else
			xx=x;
	}

	xxx=xx;
	if (y < 0) {
		yyy = 480 + y * this->cheight;
	} else {
		yyy=y * this->cheight;
	}
	str2=cadena;
	for (bucle=0;bucle<ncarac;bucle++) {
		uint8_t increment;
		while ((*str2)<' ') {
			if ((*str2)==1) {
				ink=*(str2+1);
				str2+=2;
				continue;
			}
			if (*str2==2) {
				paper=*(str2+1);
				str2+=2;
				continue;
			}
			printf("Error de cadena %d %s\n",*str2,cadena);
			str2++;
		}
		increment = this->printchar (*str2, xxx, yyy, ink, paper);
		xxx+=increment;
		if (xxx >= width - charset.max_w) {
			xxx=0;
			yyy += this->cheight;
		}
		str2++;
	}
}
