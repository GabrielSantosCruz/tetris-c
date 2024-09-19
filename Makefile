CC = gcc
CFLAGS = -std=c99
SRC = tetris.c vga.c
TRG = main
HEADERS = vga.h
OBJ = tetris.o vga.o

# Alvo padrão
all: $(TRG)

# Regra para criar o executável
$(TRG): $(OBJ)
	$(CC) $(CFLAGS) tetris.o vga.o -lintelfpgaup -o $(TRG)

# Regras para compilar os arquivos .c em .o
tetris.o: tetris.c $(HEADERS)
	$(CC) $(CFLAGS) -lintelfpgaup -c tetris.c

vga.o: vga.c $(HEADERS)
	$(CC) $(CFLAGS) -lintelfpgaup -c vga.c

# Regra para rodar o programa
run: $(TRG)
	./$(TRG)

# Limpar arquivos compilados
clean:
	rm -f $(TRG) $(OBJ)
