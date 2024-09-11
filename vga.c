#include <stdio.h>
#include "vga.h"
#include <intelfpgaup/video.h>

#define video_GREEN 0x07E0
#define video_ORANGE 0xFC00
#define video_WHITE 0xFFFF

void drawMonitor(int rows, int cows, int matriz[rows][cows]){

    if(video_open() == 1){
        // clearVGA();
        // delay(300);
        video_clear();
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
                    // printf("x1 = %d, y1 = %d, x2 = %d, y2 = %d\n", (j*10), (i*10), (j*10+10), (i*10+10));
                    // delay(500);
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
    video_clear();
    video_show();
    video_clear();
    video_show();
}