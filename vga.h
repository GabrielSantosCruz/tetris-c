#include <intelfpgaup/video.h>

#ifndef VGA_H
#define VGA_H

void drawMonitor(int rows, int cows, int matriz[rows][cows], int nextPiece, int rows2, int cows2, int matriz2[nextPiece][rows2][cows2]); // desenha o jogo no monitor
void clearVGA(); // limpa os buffers do monitor

#endif