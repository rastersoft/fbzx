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

#include "emulator.h"
#include "sound.h"
#include <stdlib.h>

/* emulates the AY-3-8912 during TSTADOS tstates */

inline void play_ay (unsigned int tstados) {

	if (!ordenador.ay_emul)
		return;

	ordenador.tst_ay += tstados;
	ordenador.tst_ay2 += tstados;

	if (ordenador.tst_ay2 > 255) {
		ordenador.tst_ay2 -= 256;
		if ((ordenador.ay_registers[11])
		    || (ordenador.ay_registers[12])) {
			if (ordenador.aych_envel)
				ordenador.aych_envel--;
			else {
				ordenador.aych_envel = (((unsigned int) ordenador.ay_registers[11]) + 256 * ((unsigned int) (ordenador.ay_registers[12])));
				if (ordenador.ay_envel_way & 0x02)	// start cycle?
					switch ((ordenador.
						 ay_registers[13]) & 0x0F)
					{
					case 0:
					case 1:
					case 2:
					case 3:
					case 8:
					case 9:
					case 10:
					case 11:
						ordenador.ay_envel_way = 4;	// cycle started and decrementing
						ordenador.ay_envel_value = 16;
						break;
					default:
						ordenador.ay_envel_way = 5;	// cycle started and incrementing
						ordenador.ay_envel_value = -1;
					}
				if (ordenador.ay_envel_way & 0x04)
				{	// cycle started?
					switch ((ordenador.
						 ay_registers[13]) & 0x0F)
					{
					case 0:
					case 1:
					case 2:
					case 3:
					case 9:
						ordenador.ay_envel_value--;
						if (ordenador.
						    ay_envel_value == 0)
							ordenador.ay_envel_way = 0;	// end
						break;

					case 4:
					case 5:
					case 6:
					case 7:
					case 15:
						ordenador.ay_envel_value++;
						if (ordenador.
						    ay_envel_value == 16)
						{
							ordenador.
								ay_envel_value
								= 0;
							ordenador.ay_envel_way = 0;	// end
						}
						break;

					case 8:
						ordenador.ay_envel_value--;
						if (ordenador.ay_envel_value == -1)
							ordenador.ay_envel_value = 15;	// repeat
						break;

					case 10:
					case 14:
						if (ordenador.ay_envel_way & 0x01)
							ordenador.ay_envel_value++;
						else
							ordenador.ay_envel_value--;
						if (ordenador.ay_envel_value == 16) {
							ordenador.ay_envel_value = 14;
							ordenador.ay_envel_way =4;
						}
						if (ordenador.ay_envel_value == -1) {
							ordenador.ay_envel_value = 1;
							ordenador.ay_envel_way = 5;
						}
						break;

					case 11:
						ordenador.ay_envel_value--;
						if (ordenador.ay_envel_value == -1) {
							ordenador.ay_envel_value = 15;
							ordenador.ay_envel_way = 0;	// end
						}
						break;

					case 12:
						ordenador.ay_envel_value++;
						if (ordenador.ay_envel_value == 16)
							ordenador.ay_envel_value = 0;
						break;

					case 13:
						ordenador.ay_envel_value++;
						if (ordenador.ay_envel_value == 15)
							ordenador.ay_envel_way = 0;	// end
						break;
					}
				}
			}
		}
		else
			ordenador.ay_envel_value = 15;
	}

	while (ordenador.tst_ay >= 16)
	{
		ordenador.tst_ay -= 16;

		if ((ordenador.ay_registers[0])
		    || (ordenador.ay_registers[1]))
		{
			if (ordenador.aych_a)
				ordenador.aych_a--;
			else
			{
				ordenador.ayval_a = 1 - ordenador.ayval_a;
				ordenador.aych_a =
					(((unsigned int) ordenador.
					  ay_registers[0]) +
					 256 *
					 ((unsigned
					   int) ((ordenador.
						  ay_registers[1]) & 0x0F))) /
					2;
			}
		}
		else
			ordenador.ayval_a = 0;

		if ((ordenador.ay_registers[2])
		    || (ordenador.ay_registers[3]))
		{
			if (ordenador.aych_b)
				ordenador.aych_b--;
			else
			{
				ordenador.ayval_b = 1 - ordenador.ayval_b;
				ordenador.aych_b =
					(((unsigned int) ordenador.
					  ay_registers[2]) +
					 256 *
					 ((unsigned
					   int) ((ordenador.
						  ay_registers[3]) & 0x0F))) /
					2;
			}
		}
		else
			ordenador.ayval_b = 0;

		if ((ordenador.ay_registers[4])
		    || (ordenador.ay_registers[5]))
		{
			if (ordenador.aych_c)
				ordenador.aych_c--;
			else
			{
				ordenador.ayval_c = 1 - ordenador.ayval_c;
				ordenador.aych_c =
					(((unsigned int) ordenador.
					  ay_registers[4]) +
					 256 *
					 ((unsigned
					   int) ((ordenador.
						  ay_registers[5]) & 0x0F))) /
					2;
			}
		}
		else
			ordenador.ayval_c = 0;

		if (ordenador.ay_registers[6])
		{
			if (ordenador.aych_n)
				ordenador.aych_n--;
			else
			{
				ordenador.ayval_n = 1 - ordenador.ayval_n;
				ordenador.aych_n =
					((((unsigned int) ordenador.
					   ay_registers[6]) & 0x1F) +
					 (rand () % 3)) / 2;
				if (ordenador.aych_n > 16)
					ordenador.aych_n = 16;
			}
		}
		else
			ordenador.ayval_n = 0;

		if (ordenador.ay_registers[8] & 0x10)
			ordenador.vol_a =
				(unsigned
				 char) ((((unsigned int) ordenador.
					  ay_envel_value)) *
					(unsigned int) ordenador.volume) / 15;
		else
			ordenador.vol_a =
				(unsigned
				 char) ((((unsigned int) (ordenador.
							  ay_registers[8] &
							  0x0F)) *
					 (unsigned int) ordenador.volume) /
					15);

		if (ordenador.ay_registers[10] & 0x10)
			ordenador.vol_c =
				(unsigned
				 char) ((((unsigned int) ordenador.
					  ay_envel_value)) *
					(unsigned int) ordenador.volume) / 15;
		else
			ordenador.vol_c =
				(unsigned
				 char) ((((unsigned int) (ordenador.
							  ay_registers[10] &
							  0x0F)) *
					 (unsigned int) ordenador.volume) /
					15);

		if (ordenador.ay_registers[9] & 0x10)
			ordenador.vol_b =
				(unsigned
				 char) ((((unsigned int) ordenador.
					  ay_envel_value)) *
					(unsigned int) ordenador.volume) / 15;
		else
			ordenador.vol_b =
				(unsigned
				 char) ((((unsigned int) (ordenador.
							  ay_registers[9] &
							  0x0F)) *
					 (unsigned int) ordenador.volume) /
					15);

	}
}


/* Creates the sound buffer during the TSTADOS tstate that the Z80 used to
   execute last instruction */

inline void play_sound (unsigned int tstados) {

	int bucle;
	int value;
	unsigned char sample_v;

	ordenador.tstados_counter_sound += tstados;

	while (ordenador.tstados_counter_sound >= ordenador.tst_sample)	{

		ordenador.tstados_counter_sound -= ordenador.tst_sample;
		if (sound_type!=1)
			for (bucle = 0; bucle < ordenador.increment; bucle++) {
				sample_v = ordenador.sample1b[bucle];
				if ((ordenador.sound_bit) && (sample_v)) {
					ordenador.sound_current_value+=(ordenador.tst_sample/8);
					if(ordenador.sound_current_value>ordenador.volume)
						ordenador.sound_current_value = ordenador.volume;
				} else {
					if(ordenador.sound_current_value>=(ordenador.tst_sample/8))
						ordenador.sound_current_value-=((ordenador.tst_sample)/8);
					else
						ordenador.sound_current_value = 0;
				}
				value = ordenador.sound_current_value;
				if (ordenador.ay_emul) {	// if emulation is ON, emulate it
					if ((ordenador.ayval_a) && (sample_v)
						&& (!(ordenador.ay_registers[7] & 0x01)))
						value += (int) ordenador.vol_a;
					if ((ordenador.ayval_b) && (sample_v)
						&& (!(ordenador.ay_registers[7] & 0x02)))
						value += (int) ordenador.vol_b;
					if ((ordenador.ayval_c) && (sample_v)
						&& (!(ordenador.ay_registers[7] & 0x04)))
						value += (int) ordenador.vol_c;
					if ((ordenador.ayval_n) && (sample_v)
						&& (!(ordenador.ay_registers[7] & 0x08)))
						value += (int) ordenador.vol_a;
					if ((ordenador.ayval_n) && (sample_v)
						&& (!(ordenador.ay_registers[7] & 0x10)))
						value += (int) ordenador.vol_b;
					if ((ordenador.ayval_n) && (sample_v)
						&& (!(ordenador.ay_registers[7] & 0x20)))
						value += (int) ordenador.vol_c;
				}
				if (value > 255)
					value = 255;
				sample_v = (char)(value - (unsigned int)ordenador.sign);
				*ordenador.current_buffer =	2*sample_v;			
				ordenador.current_buffer++;
			}
		ordenador.sound_cuantity++;

		if (ordenador.sound_cuantity == ordenador.buffer_len) {		// buffer filled
			sound_play();
			ordenador.sound_cuantity = 0;
		}
	}
}
