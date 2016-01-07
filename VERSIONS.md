* Version 3.1.0
   * Added Kempston Mouse emulation
   * Added block 30 support for TZX files
* Version 3.0.0
   * Now allows to run in TURBO mode when the tape is playing, and return to NORMAL mode when the tape is paused
   * Sorts the files and folders alphabetically
   * Allows to use fast-load with some parts of TZX files
   * Allows to save data both in TAP and TZX files
   * Allows to do FAST-SAVE in +3 mode
   * Allows to create TZX files
   * New fonts for menus
   * Fixed an stupid bug in the FAST LD_BYTES code that avoided it to work as expected
   * Supports new TZX blocks, like "pause if 48K mode"
   * Refactorized in C++
* Version 2.11.0
   * Allows to play and pause the tape even with the FAST LOAD mode enabled  
   * Adds some noise in the tape bit to ensure that the user knows when a program is waiting to load from tape
* Version 2.10.0
   * Fixed CPIR, CPDR, INI, INIR, IND, INDR, OUTI, OTIR, OUTD and OTDR instructions
   * Fixed RETI and RETN instructions to better mimic the original behaviour
   * Big rewrote of the AY-3-8912 code, greatly improving the emulation
(all changes thanks to Fabio Olimpieri)
* Version 2.9.0
   * Fixed CPIR and CPDR, so now Ant Attack works fine (thanks to Fabio Olimpieri)
   * Now CPIR and CPDR sets the N bit
   * Access to the ULAPLUS registers now produces memory contention, as it should be (thanks to Fabio Olimpieri)
   * Now initializates some registers when loading a .sna snapshot (thanks to Fabio Olimpieri)
* Version 2.8.0
   * Fixed OUTI, OTIR, OUTD and OTDR: now decrements B before doing OUT (thanks to Fabio Olimpieri)
   * Now the snapshot management functions doesn't use so many stack memory (thanks to Fabio Olimpieri)
   * Fixed a bug in screen painting with big endian machines (thanks to Fabio Olimpieri)
   * Fixed key order for Sinclair Joystick #2 (thanks to Fabio Olimpieri)
   * Fixed the Z80 emulator tests for big endian machines (thanks to Fabio Olimpieri)
   * Removed anoying sound when reseting the emulator
* Version 2.7.0
   * Fixed a bug that prevented FBZX to store the current mode
   * Now supports 320x240 mode
* Version 2.6.0
   * ULAPlus support
   * Fixed a little bug in the ROM loading
   * Allows to go to TAP/Snapshots/etc menus from HELP menu
* Version 2.5.0
   * Allows to set POKEs
   * Emulates B&W sets
   * Allows to load and save SCR snapshots
* Version 2.4.3
   * Fixed a bug with the double-scan flag
* Version 2.4.2
   * Fixed a bug with alsa sound
* Version 2.4.1
   * Now will work fine under all 64bit systems
* Version 2.4.0
   * Fixed a bug in SNA loader (thanks to Jose Luis)
   * Added a DC filter in PulseAudio sound, to avoid losing the sound when
mixing with other audio sources (thanks to Paul N)
   * Added a help message at startup
* Version 2.3.0
   * Added support for PulseAudio
   * Allows to selectively compile the audio backend (useful for Gentoo users)
* Version 2.2.0
   * Several bugfixes in the Z80 emulator, giving a much more accurate
emulation of the Z80 instruction set (thanks to the Z80 tests from
FUSE). Anyway, still not perfect.
* Version 2.1b
   * Directory ROMS in TAR.BZ2 package renamed to SPECTRUM-ROMS to allow FBZX
to run without global installation.
* Version 2.1
   * Changed the ROM format for compatibility with files in
Debian package SPECTRUM-ROMS
   * Added new icon (thanks to Paulo Silva)
* Version 2.0
   * 100% Free code (under GPL3) due to a rewriten Z80 emulator
   * Emulates the Spanish 128K (thanks to J. Baltasar)
   * Can use ALSA or OSS to sound output
   * Stores the volume settings
   * Fixed a bug in the sound with some soundcards
   * Can be run from the local directory, without installing in /usr
* Version 1.10
   * Allows to switch between full screen and window mode
   * Added Turbo mode, to load TZX files faster (both thanks to a patch
from Marcin Bukat)
   * Shows FBZX as window name
* Version 1.9
   * Stores the current mode before exiting, so if you was playing in 128K
mode, it will return in 128K mode.
   * Allows to show the screen with or without scanline emulation
   * Shows the OSD in the bottom of the screen
   * Clicking on the X button in the window bar closes the emulator
* Version 1.8
   * Fixed a bug when loading .Z80 snapshots
   * Added support for .SNA snapshots
   * Added support for Joysticks
   * Now creates an entry in the GNOME/KDE menus
   * Can load snapshots or tapes from the command line
   * Allows to asociate to it the file extensions .SNA, .Z80, .TAP and TZX so
you can load games just clicking over the file.
* Version 1.7
   * Fixed a bug in the creation of 128K snapshots
   * Fixed the memory access ports in 128K mode
* Version 1.6
   * Fixed an stupid bug that can crash FBZX
   * Added support for rotated, 480x640 screens (like PDAs)
   * Direct support for 8, 16, 24 and 32 bpp (more performance)
   * Added contended memory emulation
   * Added support for ',' and '.' keys
   * Added FullScreen mode in XWindows
   * Added a picture with the keyboard layout
   * Added support for computers without function keys
   * Asks for confirmation before exiting
   * Fixed the version number
* Version 1.5
   * Direct compilation under BIG ENDIAN arquitectures (eg. PowerPC).
* Version 1.4
   * Added the ability to create .TAP files and save to them.
   * Added Interface I and Microdrive emulation (read and write).
   * Added emulation of DELETE key.
   * Added emulation of SNOW effect.
   * Fixed a little bug in the fast-TAP loader.
   * Fixed a speed problem when used without sound.
* Version 1.3
   * Added support for more TZX tape files (thanks to Daley Thompson :)
* Version 1.2
   * Added support for TZX tape files.
   * Fixed a bug in the file selector.
   * Added right emulation of register R.
   * Added a digital low-pass filter to enhace the 48K sound.
* Version 1.1
   * Changed the sound system. Now it uses OSS instead the SDL API. This fixes a
problem with the temporization and some glitches in the sound.
   * Now can be used without sound card (run it with '-nosound' parameter).
* Version 1.0.1
   * Very little fixes to remove some innocent warnings during compilation. Thanks
to Philip Kendall.
* Version 1.0
   * First public release.
