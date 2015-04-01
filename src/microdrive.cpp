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

#include "microdrive.hh"
#include "computer.hh"
#include "emulator.hh"
#include "osd.hh"

#include <stdio.h>

class Microdrive *microdrive;

Microdrive::Microdrive() {

	int bucle;
	
	this->trash = 0;
	
	this->mdr_active = 0;
	this->mdr_paged = 0;
	
	for(bucle=0;bucle<137922;bucle++)
		this->mdr_cartridge[bucle]=0xFF; // cartridge erased
	this->mdr_cartridge[137922]=0; // but not write-protected

	this->mdr_tapehead=0;
	this->mdr_drive = 0; // no motor on
	this->mdr_old_STATUS=0x00; // default -> no down edge
	this->mdr_modified=0; // not modified
	this->mdr_current_mdr[0]=0; // no cartridge
	this->mdr_gap = 15;
	this->mdr_nogap = 15;
	this->mdr_bytes = 0;
	this->mdr_maxbytes = 0;

}

void Microdrive::reset() {
	
	this->mdr_gap = 15;
	this->mdr_nogap = 15;
	this->mdr_tapehead = 0; // head is at start position
	
}

void Microdrive::emulate(int tstados) {
	// still nothing to do here
	
}

byte Microdrive::in(word Port) {

	byte retorno;

	/* allow access to the port only if motor 1 is ON and there's a file open */
	
	if(((Port|0xFFE7) == 0xFFE7)&&(this->mdr_drive==0x01)&&(this->mdr_current_mdr[0])) {
		if(this->mdr_bytes<this->mdr_maxbytes) {
			retorno=this->mdr_cartridge[this->mdr_tapehead];
			this->trash = retorno;
			this->increment_head();
		} else {
			retorno = this->trash;
		}
		this->mdr_bytes++;
		return (retorno);
	}
	
	if((Port|0xFFE7) == 0xFFEF) {
		if((this->mdr_drive==0x01)&&(this->mdr_current_mdr[0])) { // motor 1 ON and file selected
			if(this->mdr_gap) {
				retorno=0xFE; // GAP and SYNC high
				this->mdr_gap--;
			} else {
				retorno=0xF8; // GAP and SYNC low
				if(this->mdr_nogap)
					this->mdr_nogap--;
				else {
					this->mdr_gap=15;
					this->mdr_nogap=15;
				}
			}
			if(!this->mdr_cartridge[137922]) // if write protected
				retorno|=0x01; // active bit
		} else // motor 1 OFF
			retorno=0xFF;

		this->restart();
		return (retorno);
	}

	if ((Port|0xFFE7)==0xFFF7) {		
		this->restart();
		return (0xFF);
	}
	
	return(0xFF);
	
}

void Microdrive::out(word Port,byte Value) {

	/* allow access to the port only if motor 1 is ON and there's a file open */
	
	if(((Port|0xFFE7)==0xFFE7)&&(this->mdr_drive==0x01)&&(this->mdr_current_mdr[0])) {

		if((this->mdr_bytes>11)&&(this->mdr_bytes<(this->mdr_maxbytes+12))) {
			this->mdr_cartridge[this->mdr_tapehead]=(unsigned int) Value;
			this->increment_head();
			this->mdr_modified=1;
		}
		this->mdr_bytes++;
		return;
	}
	
	if((Port|0xFFE7)==0xFFEF) {
		if(((Value&0x02)==0)&&((this->mdr_old_STATUS&0x02)==2)) { // edge down-> new bit for motor ON
			this->mdr_drive = ((this->mdr_drive<<1)&0xFE); // rotate one drive
			if(!(Value&0x01)) // if COM DATA is 0, we add a 1 bit to mdr_drive
				this->mdr_drive |= 0x01;
			
			if(this->mdr_modified) { // if the cartridge has been modified, we store it in hard disk
				this->save_cartridge();
			}
		}
		this->mdr_old_STATUS=Value;
		this->restart();
		return;
	}

	if ((Port|0xFFE7)==0xFFF7) {
		this->restart();
		return;
	}
}

bool Microdrive::save_cartridge() {

	FILE *mdr_file;

	mdr_file=fopen(this->mdr_current_mdr,"wb"); // create for write
	if(mdr_file==NULL) {
		osd->set_message("Can't store the cartridge",3000);
		return true;
	}
	fwrite(this->mdr_cartridge,137923,1,mdr_file); // save cartridge
	fclose(mdr_file);
	mdr_file=NULL;
	this->mdr_modified=0;
	return false;
}

void Microdrive::increment_head() { // gets the tape head to the next byte
	
	this->mdr_tapehead++;
	if(this->mdr_tapehead>137921)
		this->mdr_tapehead=0;
}

void Microdrive::restart() { // there's an access to a port. Reset counters and relocate the head

	//printf("Inicializado\n");
	
	while(((this->mdr_tapehead%543)!=0)&&((this->mdr_tapehead%543)!=15)) {
		this->increment_head(); // put head in the start of a block
	}
	
	this->mdr_bytes = 0; // reset current number of bytes written
	if((this->mdr_tapehead%543)==0)
		this->mdr_maxbytes = 15; // up to 15 bytes for header blocks
	else
		this->mdr_maxbytes = 528; // up to 528 bytes for data blocks
	
}

int Microdrive::select_mdrfile(char *filename) {

	int retorno;
	FILE *mdr_file;

	mdr_file=fopen(filename,"rb"); // read
	if(mdr_file==NULL)
		retorno=-1;
	else {
		retorno=0;
		fread(this->mdr_cartridge,137923,1,mdr_file); // read the cartridge in memory
		this->mdr_modified=0; // not modified
		fclose(mdr_file);
		this->mdr_tapehead=0;
	}

	strcpy(this->mdr_current_mdr,filename);
	return retorno;
}

int Microdrive::new_mdrfile(char *filename) {

	int retorno;
	int bucle;
	FILE *mdr_file;

	mdr_file=fopen(filename,"r"); // test if it exists
	if(mdr_file != NULL) {
		fclose(mdr_file);
		return -1;
	}


	for(bucle=0;bucle<137921;bucle++) {
		this->mdr_cartridge[bucle]=0xFF; // erase cartridge
	}
	strcpy(this->mdr_current_mdr,filename);
	this->mdr_cartridge[137922]=0;
	if (this->save_cartridge()) {
		retorno = -2;
		this->mdr_current_mdr[0] = 0;
	} else {
		retorno = 0;
	}

	return retorno;
}

bool Microdrive::get_protected() {

	if (this->mdr_cartridge[137922] == 0) {
		return false;
	} else {
		return true;
	}
}

void Microdrive::set_protected(bool prot) {

	if (prot) {
		this->mdr_cartridge[137922] = 1;
	} else {
		this->mdr_cartridge[137922] = 0;
	}
	this->save_cartridge();
}
