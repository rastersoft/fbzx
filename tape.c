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
#include "menus.h"
#include "tape.h"

int elcontador=0;
int eltstado=0;
char elbit=0;

/* reads a tape file and updates the readed bit */

inline void tape_read(FILE *fichero, int tstados) {

	if(ordenador.pause)
		return;

	if(ordenador.tape_file_type == TAP_TAP)
		tape_read_tap(fichero,tstados);
	else
		tape_read_tzx(fichero,tstados);
	
}

// manages TAP files in REAL_MODE

inline void tape_read_tap (FILE * fichero, int tstados) {

	static unsigned char value, value2;
	int retval;
		
	if (fichero == NULL)
		return;

	if (!ordenador.pause) {
		if (ordenador.tape_current_mode == TAP_TRASH) {		// initialize a new block						
			retval=fread (&value, 1, 1, fichero);
			retval=fread (&value2, 1, 1, fichero);	// read block longitude
			if (feof (fichero)) {
				rewind_tape(fichero,1);
				ordenador.tape_current_mode = TAP_TRASH;
				return;
			}
			ordenador.tape_byte_counter = ((unsigned int) value) + 256 * ((unsigned int) value2);
			retval=fread (&(ordenador.tape_byte), 1, 1, fichero);
			ordenador.tape_bit = 0x80;
			ordenador.tape_current_mode = TAP_GUIDE;
			ordenador.tape_counter0 = 2168;
			ordenador.tape_counter1 = 2168;
			if (!(0x80 & ordenador.tape_byte))
				ordenador.tape_counter_rep = 3228;	// 4 seconds
			else
				ordenador.tape_counter_rep = 1614;	// 2 seconds
		}

		// if there's a pulse still being reproduce, just reproduce it

		if (ordenador.tape_counter0) 	{
			if (ordenador.tape_counter0 > tstados) {
				ordenador.tape_readed = 0;	// return 0
				ordenador.tape_counter0 -= tstados;	// decrement counter;
				return;
			} else {
				tstados -= ordenador.tape_counter0;
				ordenador.tape_counter0 = 0;
			}
		}

		ordenador.tape_readed = 1;	// return 1
		if (ordenador.tape_counter1) {
			if (ordenador.tape_counter1 > tstados) {
				ordenador.tape_counter1 -= tstados;	// decrement counter;
				return;
			} else {
				tstados -= ordenador.tape_counter1;
				ordenador.tape_counter1 = 0;
				ordenador.tape_readed = 0;	// return 0
			}
		}

		// pulse ended
		
		switch (ordenador.tape_current_mode) {
		case TAP_GUIDE:	// guide tone
			if (ordenador.tape_counter_rep) {	// still into guide tone
				ordenador.tape_counter_rep--;
				ordenador.tape_counter0 = 2168 - tstados;
				ordenador.tape_counter1 = 2168;	// new pulse
				return;
			} else {	// guide tone ended. send sync tone
				ordenador.tape_counter0 = 667 - tstados;
				ordenador.tape_counter1 = 735;	// new pulse
				ordenador.tape_current_mode = TAP_DATA;	// data mode
				ordenador.tape_bit = 0x80;	// from bit0 to bit7
				return;
			}
			break;
		case TAP_DATA:	// data
			if (ordenador.tape_byte & ordenador.tape_bit) {	// next bit is 1
				ordenador.tape_counter0 = 1710 - tstados;
				ordenador.tape_counter1 = 1710;
			} else {
				ordenador.tape_counter0 = 851 - tstados;
				ordenador.tape_counter1 = 852;
			}
			ordenador.tape_bit = ((ordenador.tape_bit >> 1) & 0x7F);	// from bit0 to bit7
			if (!ordenador.tape_bit) {
				ordenador.tape_byte_counter--;
				if (!ordenador.tape_byte_counter) {	// ended the block
					ordenador.tape_current_mode = TAP_PAUSE;	// pause
					ordenador.tape_readed = 0;
					ordenador.tape_counter_rep = 3500000;	// 1 seconds
					return;
				}
				ordenador.tape_bit = 0x80;
				retval=fread (&(ordenador.tape_byte), 1, 1, fichero);	// read next byte
				if (feof (fichero)) {
					rewind_tape(fichero,0);
					ordenador.tape_current_mode = TAP_STOP;	// stop tape
					return;
				}
			}
			break;
		case TAP_PAUSE:	// pause
			ordenador.tape_readed = 0;
			if (ordenador.tape_counter_rep > tstados) {
				ordenador.tape_counter_rep -= tstados;
			} else {
				ordenador.tape_counter_rep = 0;
				ordenador.tape_current_mode = TAP_TRASH;	// read new block
			}
			return;
			break;
		case TAP_STOP:
			ordenador.tape_current_mode = TAP_TRASH;	// initialize
			ordenador.pause = 1;	// pause it
			break;
		default:
			break;
		}
	}
}

// manages TZX files

inline void tape_read_tzx (FILE * fichero, int tstados) {

	static unsigned char value, value2,value3,value4,done;
	static unsigned int bucle,bucle2;
	int retval;
	
	if ((fichero == NULL)||(ordenador.pause))
		return;

	if (ordenador.tape_current_mode == TAP_TRASH) {		// initialize a new block
		done = 0;
		do {
			retval=fread(&value,1,1,fichero); // read block ID
			//printf("ID: %X en %d\n",value,ftell(fichero));
			if(feof(fichero))
				done = 1;
			else
				switch(value) {
				case 0x10: // classic tape block
					done = 1;
					bucle = 0;
					ordenador.tape_current_bit = 0;
					ordenador.tape_bit0_level = 852;
					ordenador.tape_bit1_level = 1710;
					ordenador.tape_bits_at_end = 8;
					ordenador.tape_block_level = 2168;
					ordenador.tape_sync_level0 = 667;
					ordenador.tape_sync_level1 = 735;
					
					retval=fread(&value2,1,1,fichero);
					retval=fread(&value3,1,1,fichero); // pause length
					ordenador.tape_pause_at_end = ((unsigned int) value2) + 256 * ((unsigned int) value3);
					if(ordenador.tape_pause_at_end==0)
						ordenador.tape_pause_at_end=10; // to avoid problems
					ordenador.tape_pause_at_end *= 3500;
					retval=fread (&value, 1, 1, fichero);
					
					retval=fread (&value2, 1, 1, fichero);	// read block longitude
					if (feof (fichero)) {
						rewind_tape(fichero,1);
						ordenador.tape_current_bit = 0;
						ordenador.tape_current_mode = TAP_TRASH;						
						for(bucle=0;bucle<10;bucle++)
							retval=fread(&value3,1,1,fichero); // jump over the header
						return;
					}
					ordenador.tape_byte_counter = ((unsigned int) value) + 256 * ((unsigned int) value2);
					retval=fread (&(ordenador.tape_byte), 1, 1, fichero);
					ordenador.tape_bit = 0x80;
					ordenador.tape_current_mode = TAP_GUIDE;
					ordenador.tape_counter0 = 2168;
					ordenador.tape_counter1 = 2168;
					if (!(0x80 & ordenador.tape_byte))
						ordenador.tape_counter_rep = 3228;	// 4 seconds
					else
						ordenador.tape_counter_rep = 1614;	// 2 seconds					
					break;

				case 0x11: // turbo tape block					
					done = 1;
					bucle = 0;
					
					retval=fread(&value2,1,1,fichero);
					retval=fread(&value3,1,1,fichero);
					ordenador.tape_block_level = ((unsigned int) value2) + 256 * ((unsigned int) value3);
					retval=fread(&value2,1,1,fichero);
					retval=fread(&value3,1,1,fichero);
					ordenador.tape_sync_level0 = ((unsigned int) value2) + 256 * ((unsigned int) value3);
					retval=fread(&value2,1,1,fichero);
					retval=fread(&value3,1,1,fichero);
					ordenador.tape_sync_level1 = ((unsigned int) value2) + 256 * ((unsigned int) value3);
					retval=fread(&value2,1,1,fichero);
					retval=fread(&value3,1,1,fichero);
					ordenador.tape_bit0_level = ((unsigned int) value2) + 256 * ((unsigned int) value3);
					retval=fread(&value2,1,1,fichero);
					retval=fread(&value3,1,1,fichero);
					ordenador.tape_bit1_level = ((unsigned int) value2) + 256 * ((unsigned int) value3);
					retval=fread(&value2,1,1,fichero);
					retval=fread(&value3,1,1,fichero);
					ordenador.tape_counter_rep = ((unsigned int) value2) + 256 * ((unsigned int) value3);
					ordenador.tape_counter_rep /=2;
					retval=fread(&value2,1,1,fichero);
					ordenador.tape_bits_at_end = value2;					
					
					retval=fread(&value2,1,1,fichero);
					retval=fread(&value3,1,1,fichero); // pause length
					ordenador.tape_pause_at_end = ((unsigned int) value2) + 256 * ((unsigned int) value3);
					
			
					if(ordenador.tape_pause_at_end==0)
						ordenador.tape_pause_at_end=10; // to avoid problems
					ordenador.tape_pause_at_end *= 3500;
					
					retval=fread (&value, 1, 1, fichero);
					retval=fread (&value2, 1, 1, fichero);
					retval=fread (&value3, 1, 1, fichero);	// read block longitude
					ordenador.tape_byte_counter = ((unsigned int) value) + 256 * ((unsigned int) value2) + 65536* ((unsigned int) value3);
					
					if (feof (fichero)) {
						rewind_tape(fichero,1);
						ordenador.tape_current_bit = 0;
						ordenador.tape_current_mode = TAP_TRASH;
						return;
					}
					
					retval=fread (&(ordenador.tape_byte), 1, 1, fichero);
					ordenador.tape_bit = 0x80;
					ordenador.tape_current_mode = TAP_GUIDE;
					ordenador.tape_counter0 = ordenador.tape_block_level;
					ordenador.tape_counter1 = ordenador.tape_block_level;
					ordenador.tape_current_bit = 0;
					break;
					
				case 0x12: // pure tone
					done = 1;
					retval=fread(&value2,1,1,fichero);
					retval=fread(&value3,1,1,fichero); // length of pulse in T-states
					ordenador.tape_block_level = ((unsigned int) value2) + 256 * ((unsigned int) value3);
					ordenador.tape_counter0 = ordenador.tape_block_level;
					ordenador.tape_counter1 = 0;
					retval=fread(&value2,1,1,fichero);
					retval=fread(&value3,1,1,fichero); // number of pulses
					ordenador.tape_counter_rep = ((unsigned int) value2) + 256 * ((unsigned int) value3);
					if(ordenador.tape_counter_rep == 0)
						done = 0;
					ordenador.tape_current_mode = TZX_PURE_TONE;
					break;

				case 0x13: // multiple pulses
					done=1;
					retval=fread(&value2,1,1,fichero); // number of pulses
					ordenador.tape_counter_rep = ((unsigned int) value2);
					if(ordenador.tape_counter_rep == 0)
						done = 0;
					else {
						retval=fread(&value2,1,1,fichero);
						retval=fread(&value3,1,1,fichero); // length of pulse in T-states
						ordenador.tape_counter0 = ((unsigned int) value2) + 256 * ((unsigned int) value3);
						ordenador.tape_counter1 = 0;
						ordenador.tape_current_mode = TZX_SEQ_PULSES;
					}
					break;
				
				case 0x14: // turbo tape block					
					done = 1;
					bucle = 0;
					ordenador.tape_current_bit = 0;
					retval=fread(&value2,1,1,fichero);
					retval=fread(&value3,1,1,fichero);
					ordenador.tape_bit0_level = ((unsigned int) value2) + 256 * ((unsigned int) value3);
					retval=fread(&value2,1,1,fichero);
					retval=fread(&value3,1,1,fichero);
					ordenador.tape_bit1_level = ((unsigned int) value2) + 256 * ((unsigned int) value3);
					retval=fread(&value2,1,1,fichero);
					ordenador.tape_bits_at_end = value2;
					retval=fread(&value2,1,1,fichero);
					retval=fread(&value3,1,1,fichero); // pause length
					ordenador.tape_pause_at_end = ((unsigned int) value2) + 256 * ((unsigned int) value3);
					/*if(ordenador.tape_pause_at_end==0)
						ordenador.tape_pause_at_end=10;*/ // to avoid problems
					ordenador.tape_pause_at_end *= 3500;
					retval=fread (&value, 1, 1, fichero);					
					retval=fread (&value2, 1, 1, fichero);
					retval=fread (&value3,1,1,fichero);	// read block longitude
					if (feof (fichero)) {
						rewind_tape(fichero,1);
						ordenador.tape_current_bit = 0;
						ordenador.tape_current_mode = TAP_TRASH;						
						return;
					}
					ordenador.tape_byte_counter = ((unsigned int) value) + 256 * ((unsigned int) value2) + 65536*((unsigned int)value3);					
					ordenador.tape_bit = 0x80;
					retval=fread (&(ordenador.tape_byte), 1, 1, fichero);	// read next byte
					if((ordenador.tape_byte_counter==1)&&(ordenador.tape_bits_at_end!=8)) { // last byte
						for(bucle=ordenador.tape_bits_at_end;bucle<8;bucle++) {
							ordenador.tape_byte=((ordenador.tape_byte>>1)&0x7F);
							ordenador.tape_bit = ((ordenador.tape_bit>>1)&0x7F);
						}
					}
					ordenador.tape_current_mode = TAP_DATA;
					ordenador.tape_counter0 = 0;
					ordenador.tape_counter1 = 0;
					ordenador.tape_counter_rep = 0;					
					break;

				case 0x20: // pause
					done = 1;
					retval=fread(&value2,1,1,fichero);
					retval=fread(&value3,1,1,fichero); // pause length
					ordenador.tape_counter_rep = ((unsigned int) value2) + 256 * ((unsigned int) value3);
					ordenador.tape_readed = 0;
					ordenador.tape_counter0 = 0;				
					ordenador.tape_counter1 = 0; // 1ms of inversed pulse					
					if(ordenador.tape_counter_rep == 0) {
						ordenador.tape_current_mode = TAP_PAUSE2;	// initialize
						ordenador.tape_byte_counter = 31500;
						break;
					}
					ordenador.tape_counter_rep *= 3500;
					ordenador.tape_current_mode = TAP_PAUSE;									
					break;
					
				case 0x21: // group start
					retval=fread(&value2,1,1,fichero);
					bucle2=(unsigned int) value2;
					for(bucle=0;bucle<bucle2;bucle++)
						retval=fread(&value2,1,1,fichero);
					break;
					
				case 0x22: // group end
					break;
				
				case 0x24: // loop start
					retval=fread(&value2,1,1,fichero);
					retval=fread(&value3,1,1,fichero);
					ordenador.tape_loop_counter = ((unsigned int) value2) + 256 * ((unsigned int) value3);
					ordenador.tape_loop_pos = ftell(fichero);
					break;
				
				case 0x25: // loop end
					if(ordenador.tape_loop_counter) {
						ordenador.tape_loop_counter--;
						fseek(fichero,ordenador.tape_loop_pos,SEEK_SET);
					}
					break;
				
				case 0x2A: // pause if 48K
					if(ordenador.mode128k==0) {
						ordenador.pause = 1;
						return;
					}
					break;
					
				case 0x30: // text description
					retval=fread(&value2,1,1,fichero); // length
					for(bucle=0;bucle<((unsigned int)value2);bucle++)
						retval=fread(&value3,1,1,fichero);
					break;
					
				case 0x31: // show text
					retval=fread(&value2,1,1,fichero);
					retval=fread(&value2,1,1,fichero); // length
					for(bucle=0;bucle<((unsigned int)value2);bucle++)
						retval=fread(&value3,1,1,fichero);
					break;
					
				case 0x32: // archive info
					retval=fread(&value2,1,1,fichero);
					retval=fread(&value3,1,1,fichero); // pause length
					bucle2 = ((unsigned int) value2) + 256 * ((unsigned int) value3);
					for(bucle=0;bucle<bucle2;bucle++)
						retval=fread(&value3,1,1,fichero);
					break;
				
				case 0x33: // hardware info
					retval=fread(&value2,1,1,fichero);					
					bucle2 = ((unsigned int) value2) *3;
					for(bucle=0;bucle<bucle2;bucle++)
						retval=fread(&value3,1,1,fichero);
					break;
					
				case 0x34: // emulation info					
					for(bucle=0;bucle<8;bucle++)
						retval=fread(&value3,1,1,fichero);
					break;
					
				case 0x35: // custon info					
					for(bucle=0;bucle<10;bucle++)
						retval=fread(&value3,1,1,fichero);
					retval=fread(&value,1,1,fichero);
					retval=fread(&value2,1,1,fichero);
					retval=fread(&value3,1,1,fichero);
					retval=fread(&value4,1,1,fichero);
					bucle2 = ((unsigned int) value) + 256 * ((unsigned int) value2) + 65536*((unsigned int) value3) + 16777216*((unsigned int) value4);
					for(bucle=0;bucle<bucle2;bucle++)
						retval=fread(&value3,1,1,fichero);
					break;
					
				default: // not supported
					sprintf(ordenador.osd_text,"Unsuported TZX. Contact FBZX autor. %X",value);
					ordenador.osd_time=200;
					rewind_tape(fichero,1); // rewind and stop
					ordenador.tape_current_mode = TAP_TRASH;
					return;					
					break;
				}
		} while (!done);			
	}

	if (feof (fichero)) {
		rewind_tape(fichero,1);
		ordenador.tape_current_bit = 0;
		ordenador.tape_current_mode = TAP_TRASH;		
		return;
	}
	
	// if there's a pulse still being reproduced, just play it

	if (ordenador.tape_counter0) 	{
		if (ordenador.tape_counter0 > tstados) {
			ordenador.tape_readed = ordenador.tape_current_bit;	// return current
			ordenador.tape_counter0 -= tstados;	// decrement counter;
			return;
		} else {
			tstados -= ordenador.tape_counter0;
			ordenador.tape_counter0 = 0;
		}
	}
	
	ordenador.tape_readed = 1 - ordenador.tape_current_bit;	// return oposite current
	if (ordenador.tape_counter1) {		
		if (ordenador.tape_counter1 > tstados) {
			ordenador.tape_counter1 -= tstados;	// decrement counter;
			return;
		} else {
			tstados -= ordenador.tape_counter1;
			ordenador.tape_counter1 = 0;
			ordenador.tape_readed = ordenador.tape_current_bit;	// return current
		}
	}

	// pulse ended

	switch (ordenador.tape_current_mode) {
	case TAP_FINAL_BIT:
		ordenador.tape_current_mode = TAP_TRASH;
		break;
	case TAP_GUIDE:	// guide tone
		if (ordenador.tape_counter_rep) {	// still into guide tone
			ordenador.tape_counter_rep--;
			ordenador.tape_counter0 = ordenador.tape_block_level - tstados;
			ordenador.tape_counter1 = ordenador.tape_block_level;	// new pulse
			return;
		} else {	// guide tone ended. send sync tone			
			ordenador.tape_counter0 = ordenador.tape_sync_level0 - tstados;
			ordenador.tape_counter1 = ordenador.tape_sync_level0;	// new pulse
			ordenador.tape_current_mode = TAP_DATA;	// data mode
			ordenador.tape_bit = 0x80;	// from bit0 to bit7
			if((ordenador.tape_byte_counter==1)&&(ordenador.tape_bits_at_end!=8)) { // last byte
				for(bucle=ordenador.tape_bits_at_end;bucle<8;bucle++) {
					ordenador.tape_byte=((ordenador.tape_byte>>1)&0x7F);
					ordenador.tape_bit = ((ordenador.tape_bit>>1)&0x7F);
				}
			}
			return;
		}
		break;
	case TAP_DATA:	// data
		if (ordenador.tape_byte & ordenador.tape_bit) {	// next bit is 1
			ordenador.tape_counter0 = ordenador.tape_bit1_level - tstados;
			ordenador.tape_counter1 = ordenador.tape_bit1_level;
		} else {
			ordenador.tape_counter0 = ordenador.tape_bit0_level - tstados;
			ordenador.tape_counter1 = ordenador.tape_bit0_level;
		}
		ordenador.tape_bit = ((ordenador.tape_bit >> 1) & 0x7F);	// from bit0 to bit7
		if (!ordenador.tape_bit) {			
			ordenador.tape_byte_counter--;
			if (!ordenador.tape_byte_counter) {	// ended the block
				if(ordenador.tape_pause_at_end) {					
					ordenador.tape_current_mode = TAP_PAUSE3;	// pause					
					ordenador.tape_counter_rep = ordenador.tape_pause_at_end;
				} else
					ordenador.tape_current_mode = TAP_FINAL_BIT;					
				return;
			}
			ordenador.tape_bit = 0x80;
			retval=fread (&(ordenador.tape_byte), 1, 1, fichero);	// read next byte
			if (feof (fichero)) {
				rewind_tape (fichero,0);
				ordenador.tape_current_bit = 0;				
				ordenador.tape_current_mode = TAP_STOP;	// stop tape
				return;
			}
			if((ordenador.tape_byte_counter==1)&&(ordenador.tape_bits_at_end!=8)) { // last byte
				for(bucle=ordenador.tape_bits_at_end;bucle<8;bucle++) {
					ordenador.tape_byte=((ordenador.tape_byte>>1)&0x7F);
					ordenador.tape_bit = ((ordenador.tape_bit>>1)&0x7F);
				}
			}
		}
		break;
		
	case TAP_PAUSE3: // one pulse of 1 ms for ending the bit
		ordenador.tape_counter0 = 3500; // 1 ms
		ordenador.tape_counter1 = 0;		
		ordenador.tape_current_mode = TAP_PAUSE;
		break;
		
	case TAP_PAUSE:	// pause
		ordenador.tape_readed = 0;
		ordenador.tape_current_bit = 0;
		if (ordenador.tape_counter_rep > tstados) {
			ordenador.tape_counter_rep -= tstados;
		} else {
			ordenador.tape_counter_rep = 0;
			ordenador.tape_current_mode = TAP_TRASH;	// read new block
		}
		break;
	case TAP_PAUSE2:	// pause and stop
		ordenador.tape_readed = 0;
		ordenador.tape_current_bit = 0;
		if (ordenador.tape_counter_rep > tstados) {
			ordenador.tape_counter_rep -= tstados;
		} else {
			ordenador.tape_counter_rep = 0;
			ordenador.tape_current_mode = TAP_TRASH;	// read new block
			ordenador.pause = 1;
		}
		break;
	case TZX_PURE_TONE:
		ordenador.tape_counter_rep--;
		ordenador.tape_current_bit = 1 - ordenador.tape_current_bit; // invert current bit
		if (ordenador.tape_counter_rep) {	// still into guide tone			
			ordenador.tape_counter0 = ordenador.tape_block_level - tstados;
			ordenador.tape_counter1 = 0; // new pulse			
		} else
			ordenador.tape_current_mode = TAP_TRASH;	// next ID
		break;
	case TZX_SEQ_PULSES:
		ordenador.tape_current_bit = 1 - ordenador.tape_current_bit; // invert current bit
		ordenador.tape_counter_rep--;
		if(ordenador.tape_counter_rep) {
			retval=fread(&value2,1,1,fichero);
			retval=fread(&value3,1,1,fichero); // length of pulse in T-states
			ordenador.tape_counter0 = ((unsigned int) value2) + 256 * ((unsigned int) value3);
			ordenador.tape_counter1 = 0;
		} else
			ordenador.tape_current_mode = TAP_TRASH;	// next ID		
		break;
			
	case TAP_STOP:
		ordenador.tape_current_bit = 0;
		ordenador.tape_current_mode = TAP_TRASH;	// initialize
		ordenador.pause = 1;	// pause it
		break;
	default:
		break;
	}
}

void rewind_tape(FILE *fichero,unsigned char pause) {

	int thebucle;
	unsigned char value;
	int retval;
	
	rewind (ordenador.tap_file);
	if(ordenador.tape_file_type==TAP_TZX)
		for(thebucle=0;thebucle<10;thebucle++)
			retval=fread(&value,1,1,ordenador.tap_file); // jump over the header
	ordenador.pause=pause;
}

unsigned char file_empty(FILE *fichero) {
	
	long position,position2;
	
	position=ftell(fichero);
	fseek(fichero,0,SEEK_END); // set the pointer at end
	position2=ftell(fichero);
	fseek(fichero,position,SEEK_SET); // return the pointer to the original place
	if(position2==0)
		return 1; // empty file
	else
		return 0;	
}

void save_file(FILE *fichero) {

	long position;
	unsigned char xor,salir;
	byte dato;
	int longitud;
			
	position=ftell(fichero); // store current position
	fseek(fichero,0,SEEK_END); // put position at end
	xor=0;
	
	longitud=(int)(procesador.Rm.wr.DE);
	longitud+=2;
	
	dato=(byte)(longitud%256);
	fprintf(fichero,"%c",dato);
	dato=(byte)(longitud/256);
	fprintf(fichero,"%c",dato); // file length

	fprintf(fichero,"%c",procesador.Rm.br.A); // flag

	xor^=procesador.Rm.br.A;

	salir = 0;
	do {
		if (procesador.Rm.wr.DE == 0)
			salir = 2;
		if (!salir) {
			dato=Z80free_Rd(procesador.Rm.wr.IX); // read data
			fprintf(fichero,"%c",dato);
			xor^=dato;
			procesador.Rm.wr.IX++;			
			procesador.Rm.wr.DE--;			
		}
	} while (!salir);
	fprintf(fichero,"%c",xor);
	procesador.Rm.wr.IX++;
	procesador.Rm.wr.IX++;
	fseek(fichero,position,SEEK_SET); // put position at end
	
	if(ordenador.tape_fast_load==1) //if we want fast load, we assume we want fast save too
		ordenador.other_ret = 1;	// next instruction must be RET
	
	return;
}

void fastload_block (FILE * fichero) {

	unsigned int longitud;
	unsigned char value[65536], salir,empty,flag_found;	
	unsigned int veces;
	int retval;

	ordenador.other_ret = 1;	// next instruction must be RET

	if (!(procesador.Rm.br.F & F_C)) { // if Carry=0, is VERIFY, so return OK
		procesador.Rm.br.F |= F_C;	 // verify OK
		procesador.Rm.wr.IX += procesador.Rm.wr.DE;
		procesador.Rm.wr.DE = 0;
		return;
	}

	empty=file_empty(fichero);

	
	if ((fichero == NULL)||(empty)) {
		procesador.Rm.br.F &= (~F_C);	// Load error
		procesador.Rm.wr.IX += procesador.Rm.wr.DE;
		procesador.Rm.wr.DE = 0;
		if(empty)
			sprintf (ordenador.osd_text, "Tape file empty");
		else
			sprintf (ordenador.osd_text, "No tape selected");
		ordenador.osd_time = 100;
		return;
	}

	veces=0;

	flag_found=0;
	do {
		retval=fread (value, 2, 1, fichero);	// read length of current block
		if (feof (fichero))	{			// end of file?
			veces++; // one more time rewinded
			sprintf (ordenador.osd_text, "Tape rewind");			
			ordenador.osd_time = 100;
			rewind (fichero);	// again
			retval=fread (value, 2, 1, fichero);	// read length of current block
		}
		longitud = ((unsigned int) value[0]) + 256 * ((unsigned int) value[1]);
		retval=fread (value, 1, 1, fichero);	// read flag byte
		longitud--;
		if (value[0] != procesador.Rm.br.A) { // different flag
			retval=fread (value, longitud, 1, fichero);	// jump to the next block
			if (veces==3) { // tape rewinded three times? Block with that flag not found.
				sprintf(ordenador.osd_text,"Block with right flag not found");
				ordenador.osd_time = 100;
				procesador.Rm.br.F &= (~F_C);	// Load error
				return;
			}
		} else
			flag_found = 1;
	} while(flag_found == 0);

	salir = 0;
	// avoid contention during fast load
	unsigned char old_contended = ordenador.contended_zone;
	ordenador.contended_zone=0;
	do {
		if (longitud == 0)
			salir = 1;
		if (procesador.Rm.wr.DE == 0)
			salir = 2;
		if (!salir) {
			retval=fread (value, 1, 1, fichero);	// read byte
			Z80free_Wr (procesador.Rm.wr.IX, (byte) value[0]);	// store the byte
			procesador.Rm.wr.IX++;
			procesador.Rm.wr.DE--;
			longitud--;
		}
	}
	while (!salir);
	ordenador.contended_zone = old_contended;

	clean_screen ();

	if (salir == 1) { // system wants to load more bytes that the existent
		procesador.Rm.br.F &= (~F_C);	// Load error
		return;
	}

	if ((salir == 2) && (longitud != 1)) { // there are more bytes to load
		procesador.Rm.br.F &= (~F_C);	// Load error
		if (longitud > 1)
			retval=fread (value, longitud, 1, fichero);	// jump to the next block
		return;
	}

	retval=fread (value, 1, 1, fichero);	// jump over the checksum
	procesador.Rm.br.F |= F_C;	// Load OK
	return;

}
