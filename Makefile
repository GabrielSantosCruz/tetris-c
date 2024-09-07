CC = gcc
SRC = main.c
TRG = main

$(TRG): $(SRC)
	$(CC) -o $(TRG) $(SRC)

run: $(TRG)
	clear
	./$(TRG)