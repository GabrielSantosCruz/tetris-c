CC = gcc
CFLAGS = -std=c99
SRC = tetris.c vga.c button.c
TRG = main
TAGS = lintelfpgaup

all: $(TRG)

$(TRG): tetris.o vga.o button.o
	$(CC) $(CFLAGS) -o $(TRG) tetris.o vga.o button.o -$(TAGS)

tetris.o: tetris.c vga.h
	$(CC) $(CFLAGS) -c tetris.c -$(TAGS)

vga.o: vga.c vga.h
	$(CC) $(CFLAGS) -c vga.c -$(TAGS)

button.o: button.c button.h
	$(CC) $(CFLAGS) -c button.c -$(TAGS)

run: $(TRG)
	./$(TRG)

clean:
	rm -f $(TRG) *.o
