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
#include "keyboard.hh"

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
	this->screen_snow = false;
	this->bus_value = 255;
	int height;
	int width;

	switch (ordenador->zaurus_mini) {
	case 0:
		this->init_line = 0;
		this->next_line = 640;
		this->next_scanline = 640;
		this->first_line = 40;
		this->last_line = 278;
		this->first_column = 12;
		this->next_pixel = 1;
		this->jump_pixel = 16;
		width = 640;
		height = 480;
		break;
	case 1:
		this->init_line = 80;
		this->next_line = 160;
		this->next_scanline = 160;
		this->first_line = 40;
		this->last_line = 348;
		this->first_column = 12;
		this->next_pixel = 1;
		this->jump_pixel = 8;
		width = 480;
		height = 240;
		break;
	case 2:
		this->init_line = 480*640;
		this->next_line = -(307202);
		this->next_scanline = -1;
		this->first_line = 40;
		this->last_line = 278;
		this->first_column = 24;
		this->next_pixel = 480;
		this->jump_pixel = 7680;
		width = 480;
		height = 640;
		break;
	case 3:
		this->init_line = 0;
		this->next_line = 0;
		this->next_scanline = 0;
		this->first_line = 40;
		this->last_line = 278;
		this->first_column = 24;
		this->next_pixel = 1;
		this->jump_pixel = 4;
		width = 480;
		height = 240;
		break;
	}

	this->last_column = 160;
	this->p_translt = this->translate;
	this->p_translt2 = this->translate2;

	this->contador_flash = 0;
	this->pixancho = 224;
	this->pixalto = 312;	// values for 48K mode

	this->ulaplus = false;
	this->ulaplus_reg = 0;
	this->next_line *= llscreen->bpp;
	this->next_scanline *= llscreen->bpp;
	this->init_line *= llscreen->bpp;
	this->next_pixel *= llscreen->bpp;
	this->jump_pixel *= llscreen->bpp;
	this->int_counter = 0;

	llscreen->set_paletes(ordenador->bw);

	this->base_pixel = ((unsigned char *) (llscreen->llscreen->pixels));
	this->max_pixel = this->base_pixel + ((height - 1) * width * llscreen->bpp);
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

	if ((ordenador->current_mode == MODE_P3)) {
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
			ordenador->page48k = (rom == 3) ? 1 : 0; // 48K ROM is in ROM page 3
		}
	} else if (ordenador->current_mode == MODE_48K	) {
		ordenador->block0 = ordenador->memoria;
		ordenador->page48k = 1;
	} else {			// ROMs for 128K/128Kspa/+2 mode
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

uint8_t Screen::get_bus_value(int tstates) {

	return this->bus_value;

}

/* Paints the spectrum screen during the TSTADOS tstates that the Z80 used
to execute last instruction */

void Screen::show_screen (int tstados) {

	static uint8_t temporal, ink, paper, fflash, tmp2;
	static int loop;

	if((procesador.I>=0x40)&&(procesador.I<=0x7F)) {
		this->screen_snow = true;
	} else {
		this->screen_snow = false;
	}


	this->tstados_counter2 += tstados;
	ordenador->cicles_counter += tstados;


	fflash = 0; // flash flag

	for(loop = 0; loop < tstados; loop++) {
		if (this->int_counter > 0) {
			this->int_counter --;
			if (this->int_counter == 0) {
				Z80free_INTserved(&procesador);
			}
		}
		this->currpix = (this->tstados_counter + this->first_column + this->offset + this->hoffset) % this->pixancho;
		this->currline = (this->tstados_counter + this->first_column + this->offset + this->hoffset) / this->pixancho;
		if ((this->currline > this->first_line)&&(this->currpix == 0)) {
			this->pixel += this->next_line;
		}
		this->bus_value = 0xFF;
		ordenador->memcontended_zone = 0; // no contention here
		if (((this->tstados_counter + this->offset) %4) == 0) {
			if (((this->tstados_counter + this->offset + this->offset2) < this->tstate_contention) || ((this->tstados_counter + this->offset + this->offset2) >= this->tstate_contention2) || (((this->tstados_counter + this->offset + this->offset2) % this->pixancho) >= 128)) {
				// is border
				if (this->ulaplus) {
					this->paint_pixels(255, this->border+24, 0);	// paint 8 pixels with BORDER color
				} else {
					this->paint_pixels(255, this->border, 0);	// paint 8 pixels with BORDER color
				}
			} else {
				// is user area. We search for ink and paper colours
				this->paint_pixels(this->user_pixels,this->user_ink,this->user_paper);
			}
		}

		if ((this->tstados_counter >= this->tstate_contention) && (this->tstados_counter < this->tstate_contention2)) {
			int p;
			if (((this->tstados_counter - this->tstate_contention) % this->pixancho) < 128) {
				int zone = ((this->tstados_counter - this->tstate_contention)% this->pixancho) % 8;
				switch(zone) {
					case 0:
					case 5:
						ordenador->memcontended_zone = 6 - zone;
						break;
					case 1:
					case 3:
						ordenador->memcontended_zone = 6 - zone;
						p = *this->p_translt;
						if ((!this->screen_snow) || (rand() % 8)) {
							this->bus_value = ordenador->memoria[p + ordenador->video_offset];
						} else {
							this->bus_value = ordenador->memoria[p + ordenador->video_offset - 1];
						}
						this->p_translt++;
						this->user_pixels = this->bus_value;
						break;
					case 2:
					case 4:
						ordenador->memcontended_zone = 6 - zone;
						p = *this->p_translt2;
						this->bus_value = ordenador->memoria[p + ordenador->video_offset];	// attributes
						this->p_translt2++;
						ink = this->bus_value & 0x07;	// ink colour
						paper = (this->bus_value >> 3) & 0x07;	// paper colour
						if (this->ulaplus) {
							tmp2=0x10+((this->bus_value>>2)&0x30);
							ink+=tmp2;
							paper+=8+tmp2;
						} else {
							if (this->bus_value & 0x40) {	// bright flag?
								ink += 8;
								paper += 8;
							}
							fflash = this->bus_value & 0x80;	// flash flag
							if ((fflash) && (this->flash)) {
								//paint_pixels (temporal, paper, ink);	// if FLASH, invert PAPER and INK
								this->user_ink = paper;
								this->user_paper = ink;
							} else {
								//paint_pixels (temporal, ink, paper);
								this->user_ink = ink;
								this->user_paper = paper;
							}
						}
						break;
				}
				if (this->offset_p3 != 0) {
					ordenador->memcontended_zone = (15 - (zone + this->offset_p3)) % 8;
				}
			}
		}

		if (this->tstados_counter == this->tstates_screen) {
			this->currline = 0;
			if (osd->get_time() != 0) {
				uint8_t lines;
				string text;
				text = osd->get_text(lines);
				llscreen->print_string(text, -1,-lines, 12, 0);
			} else {
				keyboard->tab_extended = false;
				keyboard->esc_again = false;
			}

			llscreen->do_flip();

			ordenador->interr = 1;
			this->int_counter = 32;
			Z80free_INT(&procesador,ordenador->bus_empty());
			ordenador->cicles_counter = 0;
			this->pixel = this->base_pixel + this->init_line;
			this->p_translt = this->translate;
			this->p_translt2 = this->translate2;
			this->contador_flash++;
			if (this->contador_flash >= 16) {
				this->flash = 1 - this->flash;
				this->contador_flash = 0;
			}
			this->tstados_counter = 0;
		} else {
			this->tstados_counter++;
		}
	}
}


/* PAINT_PIXELS paints one byte with INK color for 1 bits and PAPER color
for 0 bits, and increment acordingly the pointer PIXEL */

void Screen::paint_pixels (uint8_t octet, uint8_t ink, uint8_t paper) {

	static int bucle,valor;
	static unsigned int *p;
	static unsigned char mask;

	if (((this->currpix >= (this->last_column))) || (this->currline < this->first_line) || (this->pixel>=this->max_pixel)) {
		return;
	}

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
	case MODE_48K: // 48K
		this->pixancho = 224;
		this->pixalto = 312;
		this->pixborde_top = 64;
		this->tstate_contention = 14335;
		this->offset = 2;
		this->offset2 = -4;
		this->offset_p3 = 0;
		this->hoffset = 0;
		break;
	case MODE_128K:
	case MODE_P2:
	case MODE_128K_SPA:
		this->pixborde_top = 63;
		this->pixancho = 228;
		this->pixalto = 311;
		this->tstate_contention = 14361;
		this->offset = 0;
		this->offset2 = 0;
		this->offset_p3 = 0;
		this->hoffset = 4;
		break;
	case MODE_P3:
		this->pixborde_top = 63;
		this->pixancho = 228;
		this->pixalto = 311;
		this->tstate_contention = 14359;
		this->offset = 2;
		this->offset2 = 0;
		this->offset_p3 = 6;
		this->hoffset = 4;
		break;
	}
	printf("Reset\n");
	this->tstados_counter = 0;
	ordenador->cicles_counter = 0;
	this->pixel = this->base_pixel + this->init_line;
	this->p_translt = this->translate;
	this->p_translt2 = this->translate2;
	this->currpix = 0;
	this->currline = 0;

	this->tstate_contention2 = this->tstate_contention + 192 * this->pixancho;
	this->tstates_screen = this->pixancho * this->pixalto - 1;
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
