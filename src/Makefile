CC=g++ -c
CPP=g++ -c -g
LN=g++

CFLAGS += `pkg-config --cflags sdl libpulse-simple alsa` -O2 -D D_SOUND_PULSE -D D_SOUND_ALSA -D D_SOUND_OSS
CPPFLAGS += `pkg-config --cflags sdl libpulse-simple alsa` -O2 -D D_SOUND_PULSE -D D_SOUND_ALSA -D D_SOUND_OSS
LDFLAGS += `pkg-config --libs sdl libpulse-simple alsa`


fbzx: cargador.o computer.o emulator.o llscreen.o menus.o microdrive.o osd.o signals.o sound.o spk_ay.o tape.o Z80free.o Z80free_codes.o Z80free_codesCB.o Z80free_codesED.o Z80free_codesDD.o Z80free_codesFD.o Z80free_codesDDCB.o Z80free_codesFDCB.o 
	$(LN) -o fbzx cargador.o computer.o emulator.o llscreen.o menus.o microdrive.o osd.o signals.o sound.o spk_ay.o tape.o Z80free.o Z80free_codes.o Z80free_codesCB.o Z80free_codesED.o Z80free_codesDD.o Z80free_codesFD.o Z80free_codesDDCB.o Z80free_codesFDCB.o  $(LDFLAGS)

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

cargador.o: cargador.cpp emulator.hh osd.hh z80free/Z80free.h computer.hh signals.hh llscreen.hh tape.hh cargador.hh
	$(CPP) $(CPPFLAGS) -o cargador.o cargador.cpp

computer.o: computer.cpp z80free/Z80free.h computer.hh signals.hh llscreen.hh emulator.hh osd.hh tape.hh spk_ay.hh sound.hh microdrive.hh menus.hh
	$(CPP) $(CPPFLAGS) -o computer.o computer.cpp

emulator.o: emulator.cpp sound.hh microdrive.hh z80free/Z80free.h menus.hh llscreen.hh emulator.hh osd.hh computer.hh signals.hh tape.hh cargador.hh
	$(CPP) $(CPPFLAGS) -o emulator.o emulator.cpp

llscreen.o: llscreen.cpp osd.hh font.h llscreen.hh emulator.hh z80free/Z80free.h computer.hh signals.hh tape.hh
	$(CPP) $(CPPFLAGS) -o llscreen.o llscreen.cpp

menus.o: menus.cpp llscreen.hh emulator.hh osd.hh z80free/Z80free.h computer.hh signals.hh tape.hh menus.hh cargador.hh
	$(CPP) $(CPPFLAGS) -o menus.o menus.cpp

microdrive.o: microdrive.cpp osd.hh emulator.hh z80free/Z80free.h computer.hh signals.hh llscreen.hh tape.hh microdrive.hh
	$(CPP) $(CPPFLAGS) -o microdrive.o microdrive.cpp

osd.o: osd.cpp osd.hh
	$(CPP) $(CPPFLAGS) -o osd.o osd.cpp

signals.o: signals.cpp signals.hh
	$(CPP) $(CPPFLAGS) -o signals.o signals.cpp

sound.o: sound.cpp sound.hh emulator.hh osd.hh z80free/Z80free.h computer.hh signals.hh llscreen.hh tape.hh
	$(CPP) $(CPPFLAGS) -o sound.o sound.cpp

spk_ay.o: spk_ay.cpp sound.hh emulator.hh osd.hh z80free/Z80free.h computer.hh signals.hh llscreen.hh tape.hh
	$(CPP) $(CPPFLAGS) -o spk_ay.o spk_ay.cpp

tape.o: tape.cpp z80free/Z80free.h tape.hh signals.hh emulator.hh osd.hh computer.hh llscreen.hh
	$(CPP) $(CPPFLAGS) -o tape.o tape.cpp

