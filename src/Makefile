CC=g++ -c
CPP=g++ -c
LN=g++

CFLAGS += `pkg-config --cflags sdl libpulse-simple alsa` -O2 -D D_SOUND_PULSE -D D_SOUND_ALSA -D D_SOUND_OSS
CPPFLAGS += `pkg-config --cflags sdl libpulse-simple alsa` -O2 -D D_SOUND_PULSE -D D_SOUND_ALSA -D D_SOUND_OSS
LDFLAGS += `pkg-config --libs sdl libpulse-simple alsa`


fbzx: cargador.o characters.o computer.o emulator.o menus.o microdrive.o sound.o spk_ay.o tape.o Z80free.o Z80free_codes.o Z80free_codesCB.o Z80free_codesED.o Z80free_codesDD.o Z80free_codesFD.o Z80free_codesDDCB.o Z80free_codesFDCB.o 
	$(LN) -o fbzx cargador.o characters.o computer.o emulator.o menus.o microdrive.o sound.o spk_ay.o tape.o Z80free.o Z80free_codes.o Z80free_codesCB.o Z80free_codesED.o Z80free_codesDD.o Z80free_codesFD.o Z80free_codesDDCB.o Z80free_codesFDCB.o  $(LDFLAGS)

Z80free.o: z80free/Z80free.c z80free/Z80free.h
	$(CC) -o Z80free.o z80free/Z80free.c

Z80free_codes.o: z80free/Z80free_codes.c z80free/Z80free.h
	$(CC) -o Z80free_codes.o z80free/Z80free_codes.c
	
Z80free_codesCB.o: z80free/Z80free_codesCB.c z80free/Z80free.h
	$(CC) -o Z80free_codesCB.o z80free/Z80free_codesCB.c

Z80free_codesED.o: z80free/Z80free_codesED.c z80free/Z80free.h
	$(CC) -o Z80free_codesED.o z80free/Z80free_codesED.c
	
Z80free_codesDD.o: z80free/Z80free_codesDD.c z80free/Z80free.h
	$(CC) -o Z80free_codesDD.o z80free/Z80free_codesDD.c
	
Z80free_codesFD.o: z80free/Z80free_codesFD.c z80free/Z80free.h
	$(CC) -o Z80free_codesFD.o z80free/Z80free_codesFD.c

Z80free_codesDDCB.o: z80free/Z80free_codesDDCB.c z80free/Z80free.h
	$(CC) -o Z80free_codesDDCB.o z80free/Z80free_codesDDCB.c
	
Z80free_codesFDCB.o: z80free/Z80free_codesFDCB.c z80free/Z80free.h
	$(CC) -o Z80free_codesFDCB.o z80free/Z80free_codesFDCB.c

cargador.o: cargador.cpp emulator.hh z80free/Z80free.h computer.hh tape.hh cargador.hh
	$(CPP) -o cargador.o cargador.cpp

characters.o: characters.cpp emulator.hh z80free/Z80free.h computer.hh tape.hh characters.hh
	$(CPP) -o characters.o characters.cpp

computer.o: computer.cpp tape.hh emulator.hh z80free/Z80free.h computer.hh spk_ay.hh sound.hh microdrive.hh menus.hh characters.hh
	$(CPP) -o computer.o computer.cpp

emulator.o: emulator.cpp sound.hh microdrive.hh z80free/Z80free.h menus.hh computer.hh tape.hh emulator.hh characters.hh cargador.hh
	$(CPP) -o emulator.o emulator.cpp

menus.o: menus.cpp menus.hh tape.hh emulator.hh z80free/Z80free.h computer.hh characters.hh cargador.hh
	$(CPP) -o menus.o menus.cpp

microdrive.o: microdrive.cpp emulator.hh z80free/Z80free.h computer.hh tape.hh microdrive.hh
	$(CPP) -o microdrive.o microdrive.cpp

sound.o: sound.cpp sound.hh emulator.hh z80free/Z80free.h computer.hh tape.hh
	$(CPP) -o sound.o sound.cpp

spk_ay.o: spk_ay.cpp sound.hh emulator.hh z80free/Z80free.h computer.hh tape.hh
	$(CPP) -o spk_ay.o spk_ay.cpp

tape.o: tape.cpp z80free/Z80free.h tape.hh emulator.hh computer.hh
	$(CPP) -o tape.o tape.cpp

