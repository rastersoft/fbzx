ifdef PREFIX
	PREFIX2B=$(PREFIX)
else
	PREFIX2B=/usr/local
endif

ifdef DESTDIR
	PREFIX2=$(DESTDIR)/$(PREFIX2B)
else
	PREFIX2=$(PREFIX2B)
endif


all:
	cd src && $(MAKE)

clean:
	rm -f src/fbzx src/*.o src/*~

install:
	rm -f $(PREFIX2)/bin/fbzx
	mkdir -p $(PREFIX2)/bin
	mkdir -p $(PREFIX2)/share/fbzx
	mkdir -p $(PREFIX2)/share/applications
	mkdir -p $(PREFIX2)/share/pixmaps
	mkdir -p $(PREFIX2)/share/doc/fbzx
	mkdir -p $(PREFIX2)/share/appdata
	cp src/fbzx $(PREFIX2)/bin
	cp data/keymap.bmp $(PREFIX2)/share/fbzx
	cp data/fbzx.desktop $(PREFIX2)/share/applications
	cp data/fbzx.svg $(PREFIX2)/share/pixmaps
	cp AMSTRAD CAPABILITIES COPYING FAQ README README.TZX VERSIONS.md $(PREFIX2)/share/doc/fbzx/
	cp fbzx.appdata.xml $(PREFIX2)/share/appdata/
	chmod 644 $(PREFIX2)/share/fbzx/*
	chmod 644 $(PREFIX2)/share/doc/fbzx/*
ifndef NOROMS
	mkdir -p $(PREFIX2)/share/spectrum-roms
	cp data/spectrum-roms/* $(PREFIX2)/share/spectrum-roms
	chmod 644 $(PREFIX2)/share/spectrum-roms/*
endif

uninstall:
	rm -f $(PREFIX2)/bin/fbzx
	rm -rf $(PREFIX2)/share/fbzx
	rm -f $(PREFIX2)/share/applications/fbzx.desktop
	rm -f $(PREFIX2)/share/pixmaps/fbzx.svg
	rm -rf $(PREFIX2)/share/doc/fbzx
	rm -rf $(PREFIX2)/share/spectrum-roms
	rm -f $(PREFIX2)/share/appdata/fbzx.appdata.xml
