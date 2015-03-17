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

byte basura;

void microdrive_init() {

	int bucle;
	
	basura = 0;
	
	ordenador.mdr_active = 0;
	ordenador.mdr_paged = 0;
	
	for(bucle=0;bucle<137922;bucle++)
		ordenador.mdr_cartridge[bucle]=0xFF; // cartridge erased
	ordenador.mdr_cartridge[137922]=0; // but not write-protected

	ordenador.mdr_tapehead=0;
	ordenador.mdr_drive=0; // no motor on
	ordenador.mdr_old_STATUS=0x00; // default -> no down edge
	ordenador.mdr_modified=0; // not modified
	ordenador.mdr_current_mdr[0]=0; // no cartridge

}

void microdrive_reset() {
	
	ordenador.mdr_gap = 15;
	ordenador.mdr_nogap = 15;
	ordenador.mdr_tapehead = 0; // head is at start position	
	
}

void microdrive_emulate(int tstados) {
	// still nothing to do here
	
}

byte microdrive_in(word Port) {

	byte retorno;

	/* allow access to the port only if motor 1 is ON and there's a file open */
	
	if(((Port|0xFFE7)==0xFFE7)&&(ordenador.mdr_drive==0x01)&&(ordenador.mdr_current_mdr[0])) {
		if(ordenador.mdr_bytes<ordenador.mdr_maxbytes) {
			retorno=ordenador.mdr_cartridge[ordenador.mdr_tapehead];
			basura=retorno;
			increment_head();
		} else {
			retorno = basura;			
		}
		ordenador.mdr_bytes++;
		return (retorno);
	}
	
	if((Port|0xFFE7)==0xFFEF) {
		if((ordenador.mdr_drive==0x01)&&(ordenador.mdr_current_mdr[0])) { // motor 1 ON and file selected
			if(ordenador.mdr_gap) {
				retorno=0xFE; // GAP and SYNC high
				ordenador.mdr_gap--;
			} else {
				retorno=0xF8; // GAP and SYNC low
				if(ordenador.mdr_nogap)
					ordenador.mdr_nogap--;
				else {
					ordenador.mdr_gap=15;
					ordenador.mdr_nogap=15;
				}
			}
			if(!ordenador.mdr_cartridge[137922]) // if write protected
				retorno|=0x01; // active bit
		} else // motor 1 OFF
			retorno=0xFF;

		microdrive_restart();
		return (retorno);
	}

	if ((Port|0xFFE7)==0xFFF7) {		
		microdrive_restart();		
		return (0xFF);
	}
	
	return(0xFF);
	
}

void microdrive_out(word Port,byte Value) {

	int retval;

	/* allow access to the port only if motor 1 is ON and there's a file open */
	
	if(((Port|0xFFE7)==0xFFE7)&&(ordenador.mdr_drive==0x01)&&(ordenador.mdr_current_mdr[0])) {
		if((ordenador.mdr_bytes>11)&&(ordenador.mdr_bytes<(ordenador.mdr_maxbytes+12))) {
			ordenador.mdr_cartridge[ordenador.mdr_tapehead]=(unsigned int) Value;			
			increment_head();
			ordenador.mdr_modified=1;
		}
		ordenador.mdr_bytes++;
		return;
	}
	
	if((Port|0xFFE7)==0xFFEF) {
		if(((Value&0x02)==0)&&((ordenador.mdr_old_STATUS&0x02)==2)) { // edge down-> new bit for motor ON
			ordenador.mdr_drive=((ordenador.mdr_drive<<1)&0xFE); // rotate one drive
			if(!(Value&0x01)) // if COM DATA is 0, we add a 1 bit to mdr_drive
				ordenador.mdr_drive|=0x01;
			
			if(ordenador.mdr_modified) { // if the cartridge has been modified, we store it in hard disk
				ordenador.mdr_file=fopen(ordenador.mdr_current_mdr,"wb"); // create for write				
				if(ordenador.mdr_file==NULL) {
					sprintf(ordenador.osd_text,"Can't store the cartridge");
					ordenador.osd_time=150;
				} else {
					retval=fwrite(ordenador.mdr_cartridge,137923,1,ordenador.mdr_file); // save cartridge
					fclose(ordenador.mdr_file);
					ordenador.mdr_file=NULL;
					ordenador.mdr_modified=0;
				}
			}
		}
		ordenador.mdr_old_STATUS=Value;
		microdrive_restart();
		return;
	}

	if ((Port|0xFFE7)==0xFFF7) {
		microdrive_restart();
		return;
	}
}

void increment_head() { // gets the tape head to the next byte
	
	ordenador.mdr_tapehead++;
	if(ordenador.mdr_tapehead>137921)
		ordenador.mdr_tapehead=0;	
}

void microdrive_restart() { // there's an access to a port. Reset counters and relocate the head

	//printf("Inicializado\n");
	
	while(((ordenador.mdr_tapehead%543)!=0)&&((ordenador.mdr_tapehead%543)!=15))
		increment_head(); // put head in the start of a block
	
	ordenador.mdr_bytes = 0; // reset current number of bytes written
	if((ordenador.mdr_tapehead%543)==0)
		ordenador.mdr_maxbytes = 15; // up to 15 bytes for header blocks
	else
		ordenador.mdr_maxbytes = 528; // up to 528 bytes for data blocks		
	
}
