#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "vga.h"

#define video_GREEN 0x07E0
#define video_ORANGE 0xFC00
#define video_WHITE 0xFFFF

#define HEIGHT 13
#define WIDTH 20

int displayGame[WIDTH][HEIGHT] = {
    {5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 6},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4}
};

int tetrimonios[7][4][4] = {
    {
    {7, 7, 7, 7},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    }, {
    {0, 7, 0, 0},
    {7, 7, 7, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    }, {
    {7, 7, 0, 0},
    {7, 7, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    }, {
    {7, 0, 0, 0},
    {7, 7, 7, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    }, {
    {0, 7, 7, 0},
    {7, 7, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    }, {
    {7, 7, 0, 0},
    {0, 7, 7, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    }, {
    {0, 0, 7, 0},
    {7, 7, 7, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    }, 
};

typedef struct{
    int posX, posY;
} Part;

Part createPart(int posX, int posY){
    Part part;
    part.posX = posX;
    part.posY = posY;
    return part;
}

Part editPart(Part part, int posX, int posY){
    part.posX = posX;
    part.posY = posY;
    return part;
};


void drawGame(); // desenha a matriz do jogo na tela do terminal
void clearTerminal(); // for linux
volatile sig_atomic_t flag = 0;
void handle_int(int sig); 
void fallTest();
void delay(int delayTime); // adiciona um delay na execução do codigo
void insertPart(int numPart, int posX, int posY); // adiciona as peças na matriz do jogo
void eraseParts(int numPart, int posX, int posY); // remove as peças da matriz do jogo
void draw(); // executa as funções de adicionar a peça, desenhar e remover peça

void main(){
    signal(SIGINT, handle_int); // intercept SIGINT
    // fallTest();
    int matrizTeste[4][4] = {
    {0, 0, 7, 0},
    {7, 7, 7, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    };
    
    
    fallTest();
}

// Desenha a matriz do jogo
void drawGame(){
    clearTerminal();
    printf("Pressione ctrl + z para parar!\n");
    for(int i = 0; i < WIDTH; i++){
        for(int j = 0; j < HEIGHT; j++){
            switch (displayGame[i][j]){
            case 0:
                printf("  ");
                break;
            
            case 7:
                printf("[]");
                break;
            
            default:
                printf("@ ");
                break;
            }
        }
        printf("\n");
    }
}

// limpa o terminal
void clearTerminal(){
    system("clear");
}

void handle_int(int sig){
  flag = 1;
}

// teste de queda do jogo
void fallTest(){
    int blockNum = 4;
    Part part = createPart(1, 5);
    for(int i = 1; i < WIDTH - 2; i++){
        insertPart(blockNum, part.posX, part.posY);
        //drawGame();
        drawMonitor(20, 13, displayGame);    
        eraseParts(blockNum, part.posX, part.posY);
        part = editPart(part, (part.posX + 1), 5);
        delay(300);
    }
}

void delay(int delayTime){
    int miliSec = 1000 * delayTime;
    clock_t startTime = clock();
    while(clock() < startTime + miliSec);
}

void insertPart(int numPart, int posX, int posY){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(tetrimonios[numPart][i][j] != 0){
                displayGame[posX + i][posY + j] = 7;
            }
        }
    }
}

void eraseParts(int numPart, int posX, int posY){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(tetrimonios[numPart][i][j] != 0){
                displayGame[posX + i][posY + j] = 0;
            }
        }
    }
}

void draw(){
    clearTerminal();
    insertPart(3, 1, 2);
    drawGame();
}
