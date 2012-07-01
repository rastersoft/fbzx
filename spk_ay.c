/* 
 * Copyright (C) 2012 Fabio Olimpieri
 * Copyright 2003-2009 (C) Raster Software Vigo (Sergio Costas)
 * This file is part of FBZX Wii
 *
 * FBZX Wii is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * FBZX Wii is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * The AY white noise RNG algorithm is based on info from MAME's ay8910.c -
 * MAME's licence explicitly permits free use of info (even encourages it).
 */

#include "emulator.h"
#include "sound.h"
#include <stdlib.h>

/* emulates the AY-3-8912 during TSTADOS tstates */

inline void play_ay (unsigned int tstados) {
	
	static unsigned int noise = 1; // init value assigned on AY chip reset

	static unsigned int env_period, noise_period, tone_period_a,tone_period_b,tone_period_c;

/* AY output doesn't match the claimed levels; these levels are based
 * on the measurements posted to comp.sys.sinclair in Dec 2001 by
 * Matthew Westcott, adjusted as Philip Kendall described in a followup to his post,
 * then scaled to 0..0xff. Taken from FUSE.
 */
 
	static const unsigned int levels[16] = {
    0x00, 0x03, 0x05, 0x07,
    0x0A, 0x0F, 0x15, 0x23,
    0x2B, 0x43, 0x5A, 0x73,
    0x92, 0xAF, 0xD9, 0xFF
  };
  
	if (!ordenador.ay_emul)
		return;

	ordenador.tst_ay += tstados;
	ordenador.tst_ay2 += tstados;
	
	// A note about the period of tones, noise and envelope: careful studies of the chip
    // output prove that it counts up from 0 until the counter becomes
    // greater or equal to the period. This is an important when the
    // program is rapidly changing the period to modulate the sound.
    // Also, note that period = 0 is the same as period = 1. This is mentioned
    // in the YM2203 data sheets. However, this does NOT apply to the Envelope
    // period. In that case, period = 0 is half as period = 1.
	
	//The frequency of AY-3-8912 is half the ZX Spectrum frequency
	
	//Envelope
	//Envelope frequency is 1/(256*envelop_period) of AY-3-8912 frequency
	
	if (ordenador.tst_ay2 > 127) {
		ordenador.tst_ay2 -= 128;
		
		env_period=2*((unsigned int) ordenador.ay_registers[11]) + 256 * ((unsigned int) (ordenador.ay_registers[12]));
		if (!env_period) env_period = 1;
		
			if (ordenador.aych_envel<env_period) // to check
				ordenador.aych_envel++;
			else {
				ordenador.aych_envel = 0;
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
				if (ordenador.ay_envel_way & 0x04) // cycle started?
				{	
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
						if (ordenador.ay_envel_way & 0x01) //incrementing?
							ordenador.ay_envel_value++;
						else
							ordenador.ay_envel_value--;
						if (ordenador.ay_envel_value == 16) {
							ordenador.ay_envel_value = 15;
							ordenador.ay_envel_way =4;
						}
						if (ordenador.ay_envel_value == -1) {
							ordenador.ay_envel_value = 0;
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

	//Tone and noise
	//Tone frequency is 1/(16*tone_period) of AY-3-8912 frequency
	//Noise frequency is 1/(16*noise_period) of AY-3-8912 frequency
	
	while (ordenador.tst_ay > 15)
	{
		ordenador.tst_ay -= 16;
		
		tone_period_a= ((unsigned int) ordenador.ay_registers[0]) + 256 * ((unsigned int) ((ordenador.ay_registers[1]) & 0x0F));
		tone_period_b= ((unsigned int) ordenador.ay_registers[2]) + 256 * ((unsigned int) ((ordenador.ay_registers[3]) & 0x0F));
		tone_period_c= ((unsigned int) ordenador.ay_registers[4]) + 256 * ((unsigned int) ((ordenador.ay_registers[5]) & 0x0F));
		noise_period= ((unsigned int) ordenador.ay_registers[6]) & 0x1F;
		
		if (!noise_period) noise_period = 1;
	
		if (tone_period_a*ordenador.freq<110841)  //Freq_camp > cpufreq/(2*16*tone_period)
			ordenador.aych_a =1;
		else
		{
			if (ordenador.aych_a<tone_period_a)
				ordenador.aych_a++;
			else
			{
				ordenador.ayval_a = !ordenador.ayval_a;
				ordenador.aych_a =0;		
			}
		}

		if (tone_period_b*ordenador.freq<110841)  //Freq_camp > cpufreq/(2*16*tone_period)
			ordenador.aych_b =1;
		else
		{
			if (ordenador.aych_b<tone_period_b)
				ordenador.aych_b++;
			else
			{
				ordenador.ayval_b = !ordenador.ayval_b;
				ordenador.aych_b =0;			
			}
		}
		
		if (tone_period_c*ordenador.freq<110841)  //Freq_camp > cpufreq/(2*16*tone_period)
			ordenador.aych_c =1;
		else
		{
		if (ordenador.aych_c<tone_period_c)
			ordenador.aych_c++;
		else
		{
			ordenador.ayval_c = !ordenador.ayval_c;
			ordenador.aych_c =0;			
		}
		}
	
		if (ordenador.aych_n<noise_period)
			ordenador.aych_n++;
		else
		{
			
			//from FUSE
			
			//rng is 17-bit shift reg, bit 0 is output.
			//input is bit 0 xor bit 2.
			
			/*
			if( ( noise & 1 ) ^ ( ( noise & 2 ) ? 1 : 0 ) ) ordenador.ayval_n = !ordenador.ayval_n;
	
			noise |= ( ( noise & 1 ) ^ ( ( noise & 4 ) ? 1 : 0 ) ) ? 0x20000 : 0;
			noise >>= 1;
			*/
			
			//From MAME AY
			/* The Random Number Generator of the 8910 is a 17-bit shift */
			/* register. The input to the shift register is bit0 XOR bit3 */
			/* (bit0 is the output). This was verified on AY-3-8910 and YM2149 chips. */

			/* The following is a fast way to compute bit17 = bit0^bit3. */
			/* Instead of doing all the logic operations, we only check */
			/* bit0, relying on the fact that after three shifts of the */
			/* register, what now is bit3 will become bit0, and will */
			/* invert, if necessary, bit14, which previously was bit17. */
			
			if ((noise+1)&2) ordenador.ayval_n = !ordenador.ayval_n; //xor bit 1 and 2
			
			if( noise & 1 ) {          
			noise ^= 0x24000 ;
			}
			noise >>= 1 ; 
			
			ordenador.aych_n =0;
		}	

		// Volume
		//Each channel max 51
		
		if (ordenador.ay_registers[8] & 0x10)
			ordenador.vol_a =
				(unsigned char) (levels[ordenador.ay_envel_value]*(unsigned int) ordenador.volume/80);
		else
			ordenador.vol_a =
				(unsigned char) (levels[ordenador.ay_registers[8] &0x0F]*(unsigned int) ordenador.volume/80);

		if (ordenador.ay_registers[10] & 0x10)
			ordenador.vol_c =
				(unsigned char) (levels[ordenador.ay_envel_value] *(unsigned int) ordenador.volume/80);
		else
			ordenador.vol_c =
				(unsigned char) (levels[ordenador.ay_registers[10] & 0x0F] *(unsigned int) ordenador.volume/80);		

		if (ordenador.ay_registers[9] & 0x10)
			ordenador.vol_b =
				(unsigned char) (levels[ordenador.ay_envel_value] *(unsigned int) ordenador.volume/80);
		else
			ordenador.vol_b =
				(unsigned char)(levels[ordenador.ay_registers[9] &0x0F] *(unsigned int) ordenador.volume/80);
					

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
		if (sound_type!=1) //!SOUND_OSS
			for (bucle = 0; bucle < ordenador.increment; bucle++) {
				sample_v = ordenador.sample1b[bucle];
				if (ordenador.sound_bit && sample_v) 
					//Sound bit volume max 96
					ordenador.sound_current_value=ordenador.volume*6;
					else ordenador.sound_current_value=0;
				value = ordenador.sound_current_value;
				
				//Mixer
				
				// The 8912 has three outputs, each output is the mix of one of the three
				// tone generators and of the (single) noise generator. The two are mixed
				// BEFORE going into the DAC. The formula to mix each channel is:
				// (ToneOn | ToneDisable) & (NoiseOn | NoiseDisable).
				// Note that this means that if both tone and noise are disabled, the output
				// is 1, not 0, and can be modulated changing the volume.

				// If the channels are disabled, set their output to 1, and increase the
				// counter, if necessary, so they will not be inverted during this update.
				// Setting the output to 1 is necessary because a disabled channel is locked
				// into the ON state (see above); and it has no effect if the volume is 0.
				// If the volume is 0, increase the counter, but don't touch the output.
				
				if (ordenador.ay_emul) {	// if emulation is ON, emulate it
				//ordenador.ayval_n = 1;
					if (sample_v &&((ordenador.ayval_a || (ordenador.ay_registers[7] & 0x01))&&(ordenador.ayval_n || (ordenador.ay_registers[7] & 0x08))))
						value += (int) ordenador.vol_a;
					if (sample_v &&((ordenador.ayval_b || (ordenador.ay_registers[7] & 0x02))&&(ordenador.ayval_n || (ordenador.ay_registers[7] & 0x10))))
						value += (int) ordenador.vol_b;
					if (sample_v &&((ordenador.ayval_c || (ordenador.ay_registers[7] & 0x04))&&(ordenador.ayval_n || (ordenador.ay_registers[7] & 0x20))))
						value += (int) ordenador.vol_c;

				}
				if (value > 255)
					value = 255;
				sample_v = (unsigned char)(value - (unsigned int)ordenador.sign);
				*ordenador.current_buffer =	sample_v;	
				ordenador.current_buffer++;
			}
		ordenador.sound_cuantity++;

		if (ordenador.sound_cuantity == ordenador.buffer_len) {		// buffer filled
			sound_play();
			ordenador.sound_cuantity = 0;
		}
	}
}
