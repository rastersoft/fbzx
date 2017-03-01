CC = gcc -c -O1 -Wall
CCo = gcc -O1 -Wall

z80free_tester: z80free_tester.o Z80free.o Z80free_codes.o Z80free_codesCB.o Z80free_codesED.o Z80free_codesDD.o Z80free_codesFD.o Z80free_codesDDCB.o Z80free_codesFDCB.o
	$(CCo) -o z80free_tester z80free_tester.o Z80free.o Z80free_codes.o Z80free_codesCB.o Z80free_codesED.o Z80free_codesDD.o Z80free_codesFD.o Z80free_codesDDCB.o Z80free_codesFDCB.o

z80free_tester.o: z80free_tester.c Z80free.h
	$(CC) -o z80free_tester.o z80free_tester.c

Z80free.o: Z80free.c Z80free.h
	$(CC) -o Z80free.o Z80free.c

Z80free_codes.o: Z80free_codes.c Z80free.h
	$(CC) -o Z80free_codes.o Z80free_codes.c

Z80free_codesCB.o: Z80free_codesCB.c Z80free.h
	$(CC) -o Z80free_codesCB.o Z80free_codesCB.c

Z80free_codesED.o: Z80free_codesED.c Z80free.h
	$(CC) -o Z80free_codesED.o Z80free_codesED.c

Z80free_codesDD.o: Z80free_codesDD.c Z80free.h
	$(CC) -o Z80free_codesDD.o Z80free_codesDD.c

Z80free_codesFD.o: Z80free_codesFD.c Z80free.h
	$(CC) -o Z80free_codesFD.o Z80free_codesFD.c

Z80free_codesDDCB.o: Z80free_codesDDCB.c Z80free.h
	$(CC) -o Z80free_codesDDCB.o Z80free_codesDDCB.c

Z80free_codesFDCB.o: Z80free_codesFDCB.c Z80free.h
	$(CC) -o Z80free_codesFDCB.o Z80free_codesFDCB.c
