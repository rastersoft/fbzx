/*
 * Copyright (C) 2012 Fabio Olimpieri
 * Copyright 2003-2009 (C) Raster Software Vigo (Sergio Costas)
 * This file is part of FBZX and FBZX Wii
 *
 * FBZX (Wii) is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * FBZX (Wii) is distributed in the hope that it will be useful,
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

#include <stdlib.h>
#include "emulator.hh"
#include "computer.hh"
#include "llsound.hh"
#include "spk_ay.hh"

class SPK_AY *spk_ay;

SPK_AY::SPK_AY() {

	for (int bucle = 0; bucle < 16; bucle++)
		this->ay_registers[bucle] = 0;

	this->ay_emul = 0;
	this->aych_a = 0;
	this->aych_b = 0;
	this->aych_c = 0;
	this->aych_n = 0;
	this->aych_envel = 0;
	this->vol_a = 0;
	this->vol_b = 0;
	this->vol_c = 0;
	this->tst_ay = 0;
	this->tst_ay2 = 0;

	this->ayval_a = 0;
	this->ayval_b = 0;
	this->ayval_c = 0;
	this->ayval_n = 0;
	this->ay_envel_value = 0;
	this->ay_envel_way = 0;

	this->tstados_counter_sound = 0;
	this->num_buff = 0;	// first buffer
	this->sound_cuantity = 0;
	this->sound_current_value = 0;
	this->ay_latch = 0;

}

/* emulates the AY-3-8912 during TSTADOS tstates */

void SPK_AY::play_ay (int tstados) {

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

	if (!this->ay_emul)
		return;

	this->tst_ay += tstados;
	this->tst_ay2 += tstados;

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

	if (this->tst_ay2 > 127) {
		this->tst_ay2 -= 128;

		env_period=2*(((unsigned int) this->ay_registers[11]) + 256 * ((unsigned int) (this->ay_registers[12])));
		if (!env_period) {
			env_period = 1;
		}

		if (this->aych_envel<env_period) { // to check
			this->aych_envel++;
		} else {
			this->aych_envel = 0;
			if (this->ay_envel_way & 0x02)	// start cycle?
				switch ((this->
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
					this->ay_envel_way = 4;	// cycle started and decrementing
					this->ay_envel_value = 16;
					break;
				default:
					this->ay_envel_way = 5;	// cycle started and incrementing
					this->ay_envel_value = -1;
				}
			if (this->ay_envel_way & 0x04) { // cycle started?
				switch ((this->ay_registers[13]) & 0x0F) {
				case 0:
				case 1:
				case 2:
				case 3:
				case 9:
					this->ay_envel_value--;
					if (this->ay_envel_value == 0) {
						this->ay_envel_way = 0;	// end
					}
					break;

				case 4:
				case 5:
				case 6:
				case 7:
				case 15:
					this->ay_envel_value++;
					if (this->ay_envel_value == 16) {
						this->ay_envel_value = 0;
						this->ay_envel_way = 0;	// end
					}
					break;
				case 8:
					this->ay_envel_value--;
					if (this->ay_envel_value == -1) {
						this->ay_envel_value = 15;	// repeat
					}
					break;

				case 10:
				case 14:
					if (this->ay_envel_way & 0x01) { //incrementing?
						this->ay_envel_value++;
					} else {
						this->ay_envel_value--;
					}
					if (this->ay_envel_value == 16) {
						this->ay_envel_value = 15;
						this->ay_envel_way =4;
					}
					if (this->ay_envel_value == -1) {
						this->ay_envel_value = 0;
						this->ay_envel_way = 5;
					}
					break;

				case 11:
					this->ay_envel_value--;
					if (this->ay_envel_value == -1) {
						this->ay_envel_value = 15;
						this->ay_envel_way = 0;	// end
					}
					break;

				case 12:
					this->ay_envel_value++;
					if (this->ay_envel_value == 16) {
						this->ay_envel_value = 0;
					}
					break;

				case 13:
					this->ay_envel_value++;
					if (this->ay_envel_value == 15) {
						this->ay_envel_way = 0;	// end
					}
					break;
				}
			}
		}
	}

	//Tone and noise
	//Tone frequency is 1/(16*tone_period) of AY-3-8912 frequency
	//Noise frequency is 1/(16*noise_period) of AY-3-8912 frequency

	while (this->tst_ay > 15) {
		this->tst_ay -= 16;

		tone_period_a= ((unsigned int) this->ay_registers[0]) + 256 * ((unsigned int) ((this->ay_registers[1]) & 0x0F));
		tone_period_b= ((unsigned int) this->ay_registers[2]) + 256 * ((unsigned int) ((this->ay_registers[3]) & 0x0F));
		tone_period_c= ((unsigned int) this->ay_registers[4]) + 256 * ((unsigned int) ((this->ay_registers[5]) & 0x0F));
		noise_period= ((unsigned int) this->ay_registers[6]) & 0x1F;

		if (!noise_period)
			noise_period = 1;

		if (tone_period_a * llsound->freq < 110841) { //Freq_camp > cpufreq/(2*16*tone_period)
			this->aych_a = 1;
		} else {
			if (this->aych_a<tone_period_a) {
				this->aych_a++;
			} else {
				this->ayval_a = !this->ayval_a;
				this->aych_a = 0;
			}
		}

		if (tone_period_b * llsound->freq < 110841) { //Freq_camp > cpufreq/(2*16*tone_period)
			this->aych_b = 1;
		} else {
			if (this->aych_b<tone_period_b) {
				this->aych_b++;
			} else {
				this->ayval_b = !this->ayval_b;
				this->aych_b =0;
			}
		}

		if (tone_period_c * llsound->freq < 110841) { //Freq_camp > cpufreq/(2*16*tone_period)
			this->aych_c = 1;
		} else {
			if (this->aych_c<tone_period_c) {
				this->aych_c++;
			} else {
				this->ayval_c = !this->ayval_c;
				this->aych_c =0;
			}
		}

		if (this->aych_n<noise_period) {
			this->aych_n++;
		} else {

			//from FUSE

			//rng is 17-bit shift reg, bit 0 is output.
			//input is bit 0 xor bit 2.

			/*
			if( ( noise & 1 ) ^ ( ( noise & 2 ) ? 1 : 0 ) ) this->ayval_n = !this->ayval_n;

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

			if ((noise+1)&2) {
				this->ayval_n = !this->ayval_n; //xor bit 1 and 2
			}

			if( noise & 1 ) {
				noise ^= 0x24000 ;
			}
			noise >>= 1 ;
			this->aych_n =0;
		}

		// Volume
		//Each channel max 51
		if (this->ay_registers[8] & 0x10) {
			this->vol_a = (unsigned char) (levels[this->ay_envel_value]*(unsigned int) llsound->volume/80);
		} else {
			this->vol_a = (unsigned char) (levels[this->ay_registers[8] &0x0F]*(unsigned int) llsound->volume/80);
		}

		if (this->ay_registers[9] & 0x10) {
			this->vol_b = (unsigned char) (levels[this->ay_envel_value] *(unsigned int) llsound->volume/80);
		} else {
			this->vol_b = (unsigned char)(levels[this->ay_registers[9] &0x0F] *(unsigned int) llsound->volume/80);
		}

		if (this->ay_registers[10] & 0x10) {
			this->vol_c = (unsigned char) (levels[this->ay_envel_value] *(unsigned int) llsound->volume/80);
		} else {
			this->vol_c = (unsigned char) (levels[this->ay_registers[10] & 0x0F] *(unsigned int) llsound->volume/80);
		}
	}
}


void SPK_AY::reset() {

	// reset the AY-3-8912

	for (int bucle = 0; bucle < 16; bucle++)
		this->ay_registers[bucle] = 0;

	this->aych_a = 0;
	this->aych_b = 0;
	this->aych_c = 0;
	this->aych_n = 0;
	this->aych_envel = 0;
	this->vol_a = 0;
	this->vol_b = 0;
	this->vol_c = 0;
}

void SPK_AY::set_latch(byte Value) {

	this->ay_latch = (Value & 0x0F);
}

void SPK_AY::set_value(byte Value) {

	this->ay_registers[this->ay_latch] = (unsigned char) Value;
	if (this->ay_latch == 13) {
		this->ay_envel_way = 2;	// start cycle
	}
}

byte SPK_AY::get_value() {

	return (this->ay_registers[this->ay_latch]);
}

byte SPK_AY::get_value(int v) {

	return (this->ay_registers[v]);
}

byte SPK_AY::get_latch() {

	return (this->ay_latch);
}

/* Creates the sound buffer during the TSTADOS tstate that the Z80 used to
   execute last instruction */

void SPK_AY::play_sound (int tstados) {

	int bucle;
	int value;
	uint8_t sample_v;

	this->tstados_counter_sound += tstados;

	while (this->tstados_counter_sound >= llsound->tst_sample)	{

		this->tstados_counter_sound -= llsound->tst_sample;
		if (llsound->sound_type!=SOUND_NO) {
			for (bucle = 0; bucle < llsound->increment; bucle++) {
				if (ordenador->sound_bit)
					//Sound bit volume max 96
					this->sound_current_value = llsound->volume * 6;
				else
					this->sound_current_value=0;
				value = this->sound_current_value;

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

				if (this->ay_emul) {	// if emulation is ON, emulate it
				//this->ayval_n = 1;
					if (((this->ayval_a || (this->ay_registers[7] & 0x01))&&(this->ayval_n || (this->ay_registers[7] & 0x08))))
						value += (int) this->vol_a;
					if (((this->ayval_b || (this->ay_registers[7] & 0x02))&&(this->ayval_n || (this->ay_registers[7] & 0x10))))
						value += (int) this->vol_b;
					if (((this->ayval_c || (this->ay_registers[7] & 0x04))&&(this->ayval_n || (this->ay_registers[7] & 0x20))))
						value += (int) this->vol_c;

				}
				if (value > 255)
					value = 255;
				sample_v = (unsigned char)(value - (unsigned int)llsound->sign);
				*(llsound->current_buffer) = sample_v;
				llsound->current_buffer++;
			}
		}
		this->sound_cuantity++;

		if (this->sound_cuantity == llsound->buffer_len) {		// buffer filled
			llsound->play();
			this->sound_cuantity = 0;
		}
	}
}
