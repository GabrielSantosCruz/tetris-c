#include <stdio.h>
#include "vga.h"
#include <intelfpgaup/video.h>

#define video_GREEN 0x07E0
#define video_ORANGE 0xFC00
#define video_WHITE 0xFFFF
#define video_PINK 0xFC18

#define BLOCK_SIZE 11

void drawMonitor(int rows, int cows, int matriz[rows][cows], int rows2, int cows2, int currentPiece, int tetrominoes[currentPiece][rows2][cows2], int currentX, int currentY, int nextPiece){
    
    if(video_open() == 1){
        video_clear();
        video_erase();

        video_box(100, 0, 221, 239, video_WHITE); // desenha o retangulo branco lá



        // desenha a matriz principal
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cows; j++){
                switch(matriz[i][j]){
                   
                    case 1: // desenha a peça da matriz do jogo
                        video_box(
                            (j*BLOCK_SIZE+90+(j-1)), 
                            (i*BLOCK_SIZE+(i-1)), 
                            (j*BLOCK_SIZE+10+90+(j-1)), 
                            (i*BLOCK_SIZE+10+(i-1)), video_ORANGE); // cor da peca
                        continue;

                    /* case 0:
                        video_box((j*BLOCK_SIZE+90), (i*BLOCK_SIZE+10), (j*BLOCK_SIZE+10+90), (i*BLOCK_SIZE+10+10), video_WHITE); // cor do fundo */
                   
                }
            }
        }
        
        // O PROBLEMA ESTÁ AQUI!!!!!!
        // Desenha a peça temporariamente (antes de colidir)
        int pieceHeight = (currentPiece == 1) ? 2 : 4;
        for (int i = 0; i < pieceHeight; i++) {
            for (int j = 0; j < 4; j++) {
                if (tetrominoes[currentPiece][i][j]) {
                    video_box(
                            (j*BLOCK_SIZE+11*currentX+90+(j-1)+3), 
                            (i*BLOCK_SIZE+11*currentY+(i-1)+1), 
                            (j*BLOCK_SIZE+11*currentX+10+90+(j-1)+3), 
                            (i*BLOCK_SIZE+11*currentY+10+(i-1)+1), 
                            video_GREEN);
                }
            }
        }

        // OK
        // desenha a proxima peca no tabuleiro
        int nextPieceHeight = sizeof(tetrominoes[nextPiece]) / sizeof(tetrominoes[nextPiece][0]);
        video_text(60, 5, "PROXIMA PECA: ");
        for (int i = 0; i < nextPieceHeight; i++) {
            for (int j = 0; j < 4; j++) {
                if (tetrominoes[nextPiece][i][j]) {
                    video_box(
                    (j*BLOCK_SIZE+250+(j-1)), 
                    (i*BLOCK_SIZE+40+(i-1)), 
                    (j*BLOCK_SIZE+10+250+(j-1)), 
                    (i*BLOCK_SIZE+40+10+(i-1)), video_PINK);
                }
            }
        }

        video_show();                
        video_close();

        // video_show();

    } else {
    printf("Video nao conectado!\n");
    }
}

void clearVGA(){
    video_erase();
    video_clear();
    video_show();
    
    video_erase();
    video_clear();
    video_show();
}