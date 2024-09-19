CC = gcc
CFLAGS =-std=c99
#SRC = tetris.c
TRG = main
TAGS = lintelfpgaup
#VGA = vga.c

all: main

main: tetris.o vga.o

tetris.o: tetris.c vga.h
	$(CC) $(CFLAGS) -c tetris.c

vga.o: vga.c vga.h
	$(CC) $(CFLAGS) -c vga.c

run: $(TRG)
	./$(TRG)