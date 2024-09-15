#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <ncurses.h>
//#include "vga.h"

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

void editPart(Part *part, int posX, int posY){
    part->posX = posX;
    part->posY = posY;
};

void drawGame();
void clearTerminal();
volatile sig_atomic_t flag = 0;
void handle_int(int sig); 
void fallTest();
void delay(int delayTime);
void insertPart(int numPart, int posX, int posY);
void eraseParts(int numPart, int posX, int posY);
void draw();
void controlPiece(Part *part, int *blockNum);
int checkColisions(Part *part);

void main(){
    signal(SIGINT, handle_int); // intercepta SIGINT
    fallTest();
}

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

void clearTerminal(){
    system("clear");
}

void handle_int(int sig){
    flag = 1;
}

void fallTest(){
    int blockNum = 4;
    Part part = createPart(1, 5);
    int colision = 1;
    while (!flag) {
        insertPart(blockNum, part.posX, part.posY);
        drawGame(); 
        eraseParts(blockNum, part.posX, part.posY);
        
        colision = checkColisions(&part);
        if (colision == 0) {
            insertPart(blockNum, part.posX, part.posY);  // Fixa a peça no lugar
            break;
        }
        delay(300);
    }
}

void delay(int delayTime){
    clock_t startTime = clock();
    while(clock() < startTime + delayTime * CLOCKS_PER_SEC / 1000);
}

int checkColisions(Part *part){
    if(part->posX + 2 < WIDTH && displayGame[part->posX + 2][part->posY] == 0){
        editPart(part, (part->posX + 1), part->posY);
        return 1;
    } else {
        return 0;
    }
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
