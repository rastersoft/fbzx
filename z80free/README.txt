Z80Free
A free Z80 emulator


DISCLAIMER

Z80free is distributed under the GPL license, version three or later, which
means that is distributed "as is", without warraty of any kind. To know more
details, read the file COPYING.


WHAT IS Z80free?

Z80free is a Z80 emulator written in C. It's designed to be as accurate as
possible, mainly in the flags from the F register, the instruction set
(emulating all the oficial and unoficial instructions like SET n,(IX+10h),r
the phantom IN, incorrect FD and DD sequences...) and the R register. It also
aims to allow a very accurate emulation of the computer hardware by dividing
the instructions as finely as possible; that way the prefixes, when
available, are executed in a first round, and only then the instruction
itself is executed. This allows a much more precise emulation of incorrect
sequences of prefixes like FDFDFDFDxx... and others.


WORKING WITH Z80FREE

The heart is the typedef struct Z80FREE, which contains all the registers and
some variables needed to maintain the current state.

typedef struct {
	Z80FRegs Rm;		/** Main register set (R) */
	Z80FRegs Ra;		/** Alternate register set (R') */
	word	PC;			/** Program counter */
	byte	R;			/** Refresh */
	byte	R2;			/** Upper bit for Refresh */
	byte	I;
	byte	IFF1;		/** Interrupt Flipflop 1. If it's 2
							decrement it and don't allow INT */
	byte	IFF2;		/** Interrupt Flipflop 2 */
	byte	IM;			/** Interrupt mode */
	byte	HALT;		/** HALT status */
	byte	INT_P;		/** INT pending */
	byte	NMI_P;		/** NMI pending */
	byte	empty_bus;	/** value for empty bus when procesing
							a maskable int */
	word	IAddr;		/** address with offset for IX+d and IY+d */
	byte	IAddr_done;	/** if 1, IAddr contains a valid data */
	enum {Z80XX, Z80CB, Z80DD, Z80ED, Z80FD, Z80INT} Status;
} Z80FREE;

The first two elements, Rm and Ra, contains the main and alternate register
sets. The typedef is the following:

typedef union {
	/** Word registers. */
	struct
	{
		word AF, BC, DE, HL, IX, IY, SP;
	} wr;
	
#if __BYTE_ORDER == __LITTLE_ENDIAN
	struct
	{
		byte F,A, C,B, E,D, L,H, IXl,IXh, IYl,IYh, P,S;
	} br;
#else
	struct
	{
		byte A,F, B,C, D,E, H,L, IXh,IXl, IYh,IYl, S,P;
	} br;
#endif

} Z80FRegs;

This means that accessing a register (like HL) is done with

	processor.Rm.wr.HL

while accessing a register like H in 8bit mode is done with

	processor.Rm.br.H

Since Z80FRegs is an union, both structs are solaped. The conditional
#if __BYTE_ORDER == __LITTLE_ENDIAN allows to set the right register
order depending on the endiannes of the machine.

The register R is stored twice: as R and R2, but only R is incremented after
each instruction or prefix. When reading it (with LD A,R), the value is
composed with (R&0x7F)|(R2&0x80). That way the true value is returned,
since the real Z80 only increment the lower 7 bits of R, but stores all the
8 bits.

IFF1 and IFF2 are modified by DI and EI. But IFF1 can be 0, 1 or 2. When its
2, it means that the interrups have to be allowed after the next instruction.
This is because, when doing

	...
	EI
	XXX
	YYY
	...

if an interrupt arrives during EI, it won't be accepted between EI and XXX,
but between XXX and YYY. When IFF1 is 2, Z80FREE won't still accept
interrupts, but will decrement it, so at the next instruction it will be 1,
accepting the pending interrupt.

IM can be 0, 1 or 2, but currently modes 0 and 1 are the same (they both jump
to 38h).

When HALT is 1, the processor is running a HALT instruction, returning 4
tstates consumed and maintaining the PC counter until an interrupt is
accepted (or a reset is done).

All the other info is internal and should not be needed when creating an
emulator.

There are four functions offered by Z80FREE to interact with it:

	void Z80free_reset(Z80FREE *);

RESET function resets the processor.

	void Z80free_INT(Z80FREE *,byte);

INT triggers a maskable interrupt. The BYTE value contains the value
currently present in the bus (useful when, in a future, the true IM0
mode will be implemented).

	int Z80free_step(Z80FREE *);

Runs the processor for one complete instruction. It returns the number of
TSTates used by that instruction. After it, the PC points to the next
instruction, and is safe to do operations like creating snapshots or
similar.

	int Z80free_ustep(Z80FREE *);

Runs the processor for a microstep. That means that, if the current
instruction has a prefix, only the prefix will be recognized, and the
PC will point to the middle of the instruction; but if the instruction
doesn't have a prefix, it will run it complete. It returns the TSTates
used to run that piece of the current instruction. It's unsafe to create
an snapshot after running this function because the processor can be
in an intermediate state, but is safe to update the hardware (like the
screen or the sound).

To know when is safe to do things, a piece of code like this one can
be used:

while (1) {

	do {
		// Insecure zone. Only update the hardware
		tstados=Z80free_ustep(&procesador);
		EMULATE(tstados);
	} while(procesador.Status!=Z80XX);

	// Secure zone. Do snapshots and other things that
	// require the Z80 to be in a defined state.
	...
}

Here is assumed that EMULATE(int) emulates the screen, sound,... for
that number of TSTates. When STATUS is Z80XX, the processor is going
to read the first byte of an instruction, and is safe to do other things
like creating snapshots.

Of course, if you don't want to complicate things, just use Z80FREE_STEP.

Finally, you must implement the following functions in order to emulate the
hardware itself:

	byte Z80free_Rd (register word Addr);

When Z80Free needs to read a byte from memory, it will call this function.
It must return the value contained in the possition ADDR.

	void Z80free_Wr (register word Addr, register byte Value);

When Z80Free needs to write a byte to memory, it will call this funcion.
It must store the data VALUE at the position ADDR.

	byte Z80free_In (register word Port);

When Z80Free needs to access to an input port, it will call this function.
PORT contains the 16bit port being accessed.

	void Z80free_Out (register word Port, register byte Value);

When Z80Free needs to access to an output port, it will call this function.
PORT contains the 16bit port being accessed, and VALUE the data to be
written to the port.


HOW Z80FREE IS IMPLEMENTED

There are two main files: Z80FREE.C and Z80FREE.H. Z80FREE.C contains the main
loop and some help functions. Some of them were extracted from LibZ80 (from
Gabriel Gambetta), adapted to the new processor structure and, where needed,
fixed some little bugs.

But the true magic is in the files z80FREE_CODESXXXX.C. These files contains
big switch/case functions which plays each Z80 instruction and returns the
number of TSTATES used by them. But these files weren't created "by hand",
but parsing the files Z80FREE_CODESXXXX.TXT. These files contains a list of
all the Z80 instructions, with its opcode numbers and the number of TSTATES
used (when it's a conditional instruction, which can use more TSTATES when
true than when false, it contain both values), and are parsed by the program
Z80FREE_GENCODE.PY.

This program gets the opcode, the number of TSTATES and the instruction.
Then it splits the instruction in the instruction itself (like LD, or EX) and
parameters (like A or 5). With all that data it generates the code for that
instruction in the right CASE statement.

The code in Z80FREE_GENCODE.PY is divided in several classes, so is extremely
easy to adapt it to other processors. Just read the comments (you should use
the GENERIC_PARSER class "as is", and create a new class which inherits from
it and implements the methods GET_CODE_FROM_ONE_PARAM, CREATE_CODE,
WRITE_START_CODE and WRITE_END_CODE.

In order to simplify the parser, the syntax of some instructions have been
modified, like the I/O ones: instead of (C) we use [BC].

If an instruction is incorrectly implemented, you must fix it in
Z80FREE_GENCODE.PY, run it again to regenerate the .C files, and only then
recompile your emulator.

The big advantage of this method is that, if the implementation of one
instruction type is incorrect, fixing it once will fix it in all the
instructions with all the access modes used. An example: if the LD
instruction type has a bug, fixing it will fix all the LD instructions, like
LD A,n, LD (HL),A, LD B,C... The same when the bug is in the parameter code:
if there's an error in the code that implements (HL) as writing, fixing it
will fix all the instructions that uses it, like LD (HL),A, or CP (HL).


CONTACTING THE AUTHOR

This program is
(C) 2008 Sergio Costas Rodriguez
(Raster Software Vigo)

raster@rastersoft.com
http://www.rastersoft.com

