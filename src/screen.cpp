/*
 * screen.cpp
 *
 *  Created on: 26/03/2015
 *      Author: raster
 */

#include "llscreen.hh"
#include "screen.hh"
#include "emulator.hh"
#include "computer.hh"

class Screen *screen;

/* Registers the screen surface where the Spectrum will put the picture,
prepares the palette and creates two arrays (translate and translate2)
that gives the memory address for each scan */

Screen::Screen() {

	//int resx,resy;
	int bucle, bucle2, bucle3, bucle4, bucle5;

	// we prepare the scanline transform arrays

	bucle5 = 0;
	for (bucle = 0; bucle < 3; bucle++)
		for (bucle2 = 0; bucle2 < 8; bucle2++)
			for (bucle3 = 0; bucle3 < 8; bucle3++)
				for (bucle4 = 0; bucle4 < 32; bucle4++) {
					this->translate[bucle5] = 147456 + bucle * 2048 + bucle2 * 32 + bucle3 * 256 + bucle4;
					this->translate2[bucle5] = 153600 + bucle * 256 + bucle2 * 32 + bucle4;
					bucle5++;
				}
	this->tstados_counter = 0;

	this->border = 0;
	this->currline = 0;
	this->currpix = 0;
	this->flash = 0;

	switch (ordenador->zaurus_mini) {
	case 0:
		this->init_line = 0;
		this->next_line = 640;
		this->next_scanline = 640;
		this->first_line = 40;
		this->last_line = 280;
		this->first_pixel = 16;
		this->last_pixel = 336;
		this->next_pixel = 1;
		this->jump_pixel = 16;
		break;
	case 1:
		this->init_line = 65;
		this->next_line = 160;
		this->next_scanline = 160;
		this->first_line = 40;
		this->last_line = 280;
		this->first_pixel = 0;
		this->last_pixel = 351;
		this->next_pixel = 1;
		this->jump_pixel = 8;
		break;
	case 2:
		this->init_line = 479;
		this->next_line = -(307202);
		this->next_scanline = -1;
		this->first_line = 40;
		this->last_line = 280;
		this->first_pixel = 16;
		this->last_pixel = 336;
		this->next_pixel = 480;
		this->jump_pixel = 7680;
		break;
	case 3:
		this->init_line = 0;
		this->next_line = 0;
		this->next_scanline = 0;
		this->first_line = 40;
		this->last_line = 280;
		this->first_pixel = 0;
		this->last_pixel = 319;
		this->next_pixel = 1;
		this->jump_pixel = 4;
		break;
	}

	this->p_translt = this->translate;
	this->p_translt2 = this->translate2;

	this->contador_flash = 0;
	this->pixancho = 447;
	this->pixalto = 311;	// values for 48K mode

	this->ulaplus = false;
	this->ulaplus_reg = 0;
	this->next_line *= llscreen->bpp;
	this->next_scanline *= llscreen->bpp;
	this->init_line *= llscreen->bpp;
	this->next_pixel *= llscreen->bpp;
	this->jump_pixel *= llscreen->bpp;

	llscreen->set_paletes(ordenador->bw);

	this->base_pixel = ((unsigned char *) (llscreen->llscreen->pixels));
	this->pixel = this->base_pixel + this->init_line;
}

void Screen::set_memory_pointers () {

	static unsigned int rom, ram;

	// assign the offset for video page

	if (ordenador->mport1 & 0x08)
		ordenador->video_offset = 32768;	// page 7
	else
		ordenador->video_offset = 0;	// page 5

	// assign ROMs and, if in special mode, RAM for the whole blocks

	if ((ordenador->mode128k == 3)) {
		if (ordenador->mport2 & 0x01) {		// +2A/+3 special mode
			ordenador->page48k = 0; // no 48K ROM paged in
			ram = (unsigned int) (ordenador->mport1 & 0x06);	// bits 1&2
			switch (ram) {
			case 0:
				ordenador->block0 = ordenador->memoria + 65536;
				ordenador->block1 = ordenador->memoria + 65536;
				ordenador->block2 = ordenador->memoria + 65536;
				ordenador->block3 = ordenador->memoria + 65536;
				break;
			case 2:
				ordenador->block0 = ordenador->memoria + 131072;
				ordenador->block1 = ordenador->memoria + 131072;
				ordenador->block2 = ordenador->memoria + 131072;
				ordenador->block3 = ordenador->memoria + 131072;
				break;
			case 4:
				ordenador->block0 = ordenador->memoria + 131072;
				ordenador->block1 = ordenador->memoria + 131072;
				ordenador->block2 = ordenador->memoria + 131072;
				ordenador->block3 = ordenador->memoria + 65536;
				break;
			case 6:
				ordenador->block0 = ordenador->memoria + 131072;
				ordenador->block1 = ordenador->memoria + 163840;
				ordenador->block2 = ordenador->memoria + 131072;
				ordenador->block3 = ordenador->memoria + 65536;
				break;
			default:
				printf("FullRAM desconocido\n");
			}
			return;
		} else {		// ROMs for +2A/+3 normal mode
			rom = 0;
			if (ordenador->mport1 & 0x10)
				rom++;
			if (ordenador->mport2 & 0x04)
				rom += 2;
			// assign the first block pointer to the right block bank
			ordenador->block0 = ordenador->memoria + (16384 * rom);
			ordenador->page48k = (rom==3) ? 1 : 0; // 48K ROM is in ROM page 3
		}
	} else {			// ROMs for 128K/+2 mode
		if (ordenador->mport1 & 0x10) {
			ordenador->block0 = ordenador->memoria + 16384;
			ordenador->page48k = 1;
		} else {
			ordenador->block0 = ordenador->memoria;
			ordenador->page48k = 0;
		}
	}

	// RAMs for 128K/+2 mode, and +2A/+3 in normal mode

	ordenador->block1 = ordenador->memoria + 131072;	// page 5 minus 16384
	ordenador->block2 = ordenador->memoria + 65536;	// page 2 minus 32768

	ram = 1 + ((unsigned int) (ordenador->mport1 & 0x07));	// RAM page for block3 plus 1
	ordenador->block3 = ordenador->memoria + (16384 * ram);	// page n minus 49152
}

/* Paints the spectrum screen during the TSTADOS tstates that the Z80 used
to execute last instruction */

void Screen::show_screen (int tstados) {

	static unsigned char temporal, ink, paper, fflash, tmp2;

	if((procesador.I>=0x40)&&(procesador.I<=0x7F)) {
		this->screen_snow = true;
	} else {
		this->screen_snow = false;
	}

	this->tstados_counter += tstados;
	ordenador->cicles_counter += tstados;

	if (curr_frames!=jump_frames) {
		if (this->tstados_counter>=69888) {
			this->tstados_counter-=69888;
			ordenador->interr = 1;
			curr_frames++;
		}
		return;
	}

	fflash = 0; // flash flag
	while (this->tstados_counter > 3) {
		this->tstados_counter -= 4;

		// test if current pixel is for border or for user area

		if ((this->currline < 64) || (this->currline > 255)
			|| (this->currpix < 48) || (this->currpix > 303)) {

			// is border

			ordenador->contended_zone = false; // no contention here
			if (this->ulaplus) {
				this->paint_pixels(255, this->border+24, 0);	// paint 8 pixels with BORDER color
			} else {
				this->paint_pixels(255, this->border, 0);	// paint 8 pixels with BORDER color
			}

			ordenador->bus_value = 255;

		} else {

			// is user area. We search for ink and paper colours

			ordenador->contended_zone = true; // can have contention

			temporal = ordenador->memoria[(*this->p_translt2) + ordenador->video_offset];	// attributes
			ordenador->bus_value = temporal;
			ink = temporal & 0x07;	// ink colour
			paper = (temporal >> 3) & 0x07;	// paper colour
			if (this->ulaplus) {
				tmp2=0x10+((temporal>>2)&0x30);
				ink+=tmp2;
				paper+=8+tmp2;
			} else {
				if (temporal & 0x40) {	// bright flag?
					ink += 8;
					paper += 8;
				}
				fflash = temporal & 0x80;	// flash flag
			}

			// Snow Effect

			if(this->screen_snow) {
				temporal = ordenador->memoria[(((*this->p_translt) + (ordenador->video_offset))&0xFFFFFF00)+(procesador.R)];	// data with snow
				this->screen_snow = false; // no more snow for now
			} else
				temporal = ordenador->memoria[(*this->p_translt) + ordenador->video_offset];	// data

			this->p_translt++;
			this->p_translt2++;
			if ((fflash) && (this->flash))
				paint_pixels (temporal, paper, ink);	// if FLASH, invert PAPER and INK
			else
				paint_pixels (temporal, ink, paper);
		}
		this->currpix += 8;
		if (this->currpix > this->pixancho) {
			this->currpix = 0;
			this->currline++;
			if (this->currline > this->first_line) {
				this->pixel += this->next_line;
			}
		}

		if ((this->currline > this->pixalto)&&(this->currpix>=64)) {
			this->currpix=64;
			if (osd->get_time() != 0) {
				uint8_t lines;
				string text;
				text = osd->get_text(lines);
				llscreen->print_string(text, -1,-lines, 12, 0);
			} else {
				ordenador->tab_extended=0;
				ordenador->esc_again=0;
			}

			llscreen->do_flip();

			curr_frames=0;
			this->currline = 0;
			ordenador->interr = 1;
			ordenador->cicles_counter = 0;
			this->pixel = this->base_pixel+this->init_line;
			this->p_translt = this->translate;
			this->p_translt2 = this->translate2;
			this->contador_flash++;
			if (this->contador_flash == 16) {
				this->flash = 1 - this->flash;
				this->contador_flash = 0;
			}
		}
	}
}


/* PAINT_PIXELS paints one byte with INK color for 1 bits and PAPER color
for 0 bits, and increment acordingly the pointer PIXEL */

void Screen::paint_pixels (uint8_t octet,uint8_t ink,uint8_t paper) {

	static int bucle,valor;
	static unsigned int *p;
	static unsigned char mask;

	if ((this->currpix < 16) || (this->currpix >= 336)
	    || (this->currline < 40) || (this->currline >= 280))
		return;

	mask = 0x80;
	for (bucle = 0; bucle < 8; bucle++) {
		valor = (octet & mask) ? (int) ink : (int) paper;
		llscreen->paint_one_pixel(valor,this->pixel);
		if ((ordenador->zaurus_mini!=1)&&(ordenador->zaurus_mini!=3)) {
			if (ordenador->dblscan) {
				llscreen->paint_one_pixel(valor,this->pixel+this->next_scanline);
			} else {
				llscreen->paint_one_pixel(0,this->pixel+this->next_scanline);
			}
		}
		this->pixel+=this->next_pixel;
		if ((ordenador->zaurus_mini!=1)&&(ordenador->zaurus_mini!=3)) {
			llscreen->paint_one_pixel(valor,this->pixel);
			if (ordenador->dblscan) {
				llscreen->paint_one_pixel(valor,this->pixel+this->next_scanline);
			} else {
				llscreen->paint_one_pixel(0,this->pixel+this->next_scanline);
			}
			this->pixel+=this->next_pixel;
		}
		mask = ((mask >> 1) & 0x7F);
	}
}

void Screen::reset(uint8_t model) {

	this->ulaplus = false;
	switch (model) {
	case 0: // 48K
		this->pixancho = 447;
		this->pixalto = 311;
	break;
	case 1:
	case 2:
	case 3:
	case 4:
		this->pixancho = 455;
		this->pixalto = 310;
	break;
	}
}

void Screen::set_ulaplus_register(uint8_t reg) {

	this->ulaplus_reg = reg;
}

void Screen::set_ulaplus_value(uint8_t Value) {

	if ((this->ulaplus_reg & 0xC0)==0x40) { // mode
		this->ulaplus = (Value & 0x01) == 1 ? true : false;
		return;
	}
	if (this->ulaplus_reg < 0x40) { // register set mode
		llscreen->set_palete_entry(this->ulaplus_reg,Value,ordenador->bw);
	}
}

uint8_t Screen::read_ulaplus_value() {

	if ((this->ulaplus_reg & 0xC0)==0x40) { // mode
		return (this->ulaplus ? 1 : 0);
	}
	if (this->ulaplus_reg < 0x40) { // register set mode
		return (llscreen->get_palete_entry(this->ulaplus_reg));
	}
	return 0;
}

