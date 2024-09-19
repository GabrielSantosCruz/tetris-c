#include <stdio.h>
#include "vga.h"
#include <intelfpgaup/video.h>

#define video_GREEN 0x07E0
#define video_ORANGE 0xFC00
#define video_WHITE 0xFFFF

void drawMonitor(int rows, int cows, int matriz[rows][cows], int nextPiece, int rows2, int cows2, int matriz2[nextPiece][rows2][cows2]) {
    

    if(video_open() == 1){

        video_clear();
        video_erase();
        video_text(60, 5, "Pontuacao: ");

        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cows; j++){
                switch(matriz[i][j]){
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                        video_box((j*11+90), (i*11+10), (j*11+10+90), (i*11+10+10), video_GREEN); // cor do tabuleiro
                        continue;
                    case 7:
                        video_box((j*11+90), (i*11+10), (j*11+10+90), (i*11+10+10), video_ORANGE);
                        continue;
                    case 0:
                        video_box((j*11+90), (i*11+10), (j*11+10+90), (i*11+10+10), video_WHITE);
                        continue;
                }
            }
        }
        
        video_text(60, 9, "Proxima: ");

        for(int i = 0; i < rows2; i++){
            for(int j = 0; j < cows2; j++){
                switch(matriz2[nextPiece][i][j]){
                    case 0:
                        continue;
                    case 7:
                        video_box((j*11+240), (i*11+50), (j*11+10+240), (i*11+50+10), video_ORANGE);
                        continue;
                }
            }
        }


        video_show();                
        video_close();
    } else {
        printf("Video nao conectado!\n");
    }
}

void clearVGA(){
    video_clear();
    video_show();
    video_clear();
    video_show();
}