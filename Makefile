ifdef PREFIX
        PREFIX2=$(PREFIX)/usr
else
        PREFIX2=/usr/local
endif

CFLAGS += `pkg-config --cflags sdl libpulse-simple alsa` -O2 -Wno-pointer-sign -Wall -D D_SOUND_PULSE -D D_SOUND_ALSA -D D_SOUND_OSS
LDFLAGS += `pkg-config --libs sdl libpulse-simple alsa`

fbzx: computer.o Z80free.o Z80free_codes.o Z80free_codesCB.o Z80free_codesED.o Z80free_codesDD.o Z80free_codesFD.o Z80free_codesDDCB.o Z80free_codesFDCB.o emulator.o cargador.o characters.o menus.o sound.o tape.o spk_ay.o microdrive.o

	$(CC) -o fbzx computer.o Z80free.o Z80free_codes.o Z80free_codesCB.o Z80free_codesED.o Z80free_codesDD.o Z80free_codesFD.o Z80free_codesDDCB.o Z80free_codesFDCB.o emulator.o cargador.o characters.o menus.o sound.o tape.o spk_ay.o microdrive.o $(LDFLAGS)

clean:
	rm fbzx *.o *~

install:
	rm -f $(PREFIX2)/bin/fbzx
	cp fbzx $(PREFIX2)/bin
	mkdir -p $(PREFIX2)/share/fbzx
	mkdir -p $(PREFIX2)/share/spectrum-roms
	mkdir -p $(PREFIX2)/share/applications
	mkdir -p $(PREFIX2)/share/pixmaps
	mkdir -p $(PREFIX2)/share/doc/fbzx
	cp spectrum-roms/* $(PREFIX2)/share/spectrum-roms
	cp keymap.bmp $(PREFIX2)/share/fbzx
	cp fbzx.desktop $(PREFIX2)/share/applications
	cp fbzx.svg $(PREFIX2)/share/pixmaps
	cp AMSTRAD CAPABILITIES COPYING FAQ README README.TZX VERSIONS $(PREFIX2)/share/doc/fbzx/
	chmod 644 $(PREFIX2)/share/fbzx/*
	chmod 644 $(PREFIX2)/share/spectrum-roms/*
	chmod 644 $(PREFIX2)/share/doc/fbzx/*

uninstall:
	rm -f $(PREFIX2)/bin/fbzx
	rm -rf $(PREFIX2)/share/fbzx
	rm -f $(PREFIX2)/share/applications/fbzx.desktop
	rm -f $(PREFIX2)/share/pixmaps/fbzx.svg
	rm -rf $(PREFIX2)/share/doc/fbzx
	rm -rf $(PREFIX2)/share/spectrum-roms

spk_ay.o: spk_ay.c spk_ay.h emulator.h sound.h computer.h z80free/Z80free.h
	$(CC) $(CFLAGS) -c -o spk_ay.o spk_ay.c

microdrive.o: microdrive.c microdrive.h z80free/Z80free.h computer.h emulator.h z80free/Z80free.h
	$(CC) $(CFLAGS) -c -o microdrive.o microdrive.c

sound.o: sound.c sound.h computer.h emulator.h z80free/Z80free.h
	$(CC) $(CFLAGS) -c -o sound.o sound.c

emulator.o: emulator.c z80free/Z80free.h computer.h emulator.h characters.h menus.h cargador.h sound.h tape.h microdrive.h
	$(CC) $(CFLAGS) -c -o emulator.o emulator.c

computer.o: computer.c z80free/Z80free.h computer.h emulator.h characters.h menus.h cargador.h sound.h tape.h spk_ay.h microdrive.h
	$(CC) $(CFLAGS) -c -o computer.o computer.c

tape.o: tape.c z80free/Z80free.h computer.h emulator.h menus.h tape.h computer.h
	$(CC) $(CFLAGS) -c -o tape.o tape.c

cargador.o: cargador.c cargador.h computer.h emulator.h z80free/Z80free.h characters.h menus.h computer.h
	$(CC) $(CFLAGS) -c -o cargador.o cargador.c

characters.o: characters.c characters.h emulator.h computer.h
	$(CC) $(CFLAGS) -c -o characters.o characters.c

menus.o: menus.c menus.h characters.h computer.h emulator.h z80free/Z80free.h cargador.h tape.h
	$(CC) $(CFLAGS) -c -o menus.o menus.c

Z80free.o: z80free/Z80free.c z80free/Z80free.h
	$(CC) $(CFLAGS) -c -o Z80free.o z80free/Z80free.c

Z80free_codes.o: z80free/Z80free_codes.c z80free/Z80free.h
	$(CC) $(CFLAGS) -c -o Z80free_codes.o z80free/Z80free_codes.c
	
Z80free_codesCB.o: z80free/Z80free_codesCB.c z80free/Z80free.h
	$(CC) $(CFLAGS) -c -o Z80free_codesCB.o z80free/Z80free_codesCB.c

Z80free_codesED.o: z80free/Z80free_codesED.c z80free/Z80free.h
	$(CC) $(CFLAGS) -c -o Z80free_codesED.o z80free/Z80free_codesED.c
	
Z80free_codesDD.o: z80free/Z80free_codesDD.c z80free/Z80free.h
	$(CC) $(CFLAGS) -c -o Z80free_codesDD.o z80free/Z80free_codesDD.c
	
Z80free_codesFD.o: z80free/Z80free_codesFD.c z80free/Z80free.h
	$(CC) $(CFLAGS) -c -o Z80free_codesFD.o z80free/Z80free_codesFD.c

Z80free_codesDDCB.o: z80free/Z80free_codesDDCB.c z80free/Z80free.h
	$(CC) $(CFLAGS) -c -o Z80free_codesDDCB.o z80free/Z80free_codesDDCB.c
	
Z80free_codesFDCB.o: z80free/Z80free_codesFDCB.c z80free/Z80free.h
	$(CC) $(CFLAGS) -c -o Z80free_codesFDCB.o z80free/Z80free_codesFDCB.c

