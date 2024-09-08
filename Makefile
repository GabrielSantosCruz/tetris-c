CC = gcc
SRC = tetris.c
TRG = tetris

$(TRG): $(SRC)
	$(CC) -o $(TRG) $(SRC)

run: $(TRG)
	clear
	./$(TRG)