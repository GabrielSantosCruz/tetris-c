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

// grava a posição do primeiro bloco da peça
typedef struct{
    int posX, posY;
} Part;

// cria a strucr
Part createPart(int posX, int posY){
    Part part;
    part.posX = posX;
    part.posY = posY;
    return part;
}

// edita a struct
void editPart(Part *part, int posX, int posY){
    part->posX = posX;
    part->posY = posY;
};


void drawGame(); // desenha a matriz do jogo na tela do terminal
void clearTerminal(); // for linux
volatile sig_atomic_t flag = 0;
void handle_int(int sig); 
void fallTest(); // testando a "gravidade"
void delay(int delayTime); // adiciona um delay na execução do codigo
void insertPart(int numPart, int posX, int posY); // adiciona as peças na matriz do jogo
void eraseParts(int numPart, int posX, int posY); // remove as peças da matriz do jogo
void draw(); // executa as funções de adicionar a peça, desenhar e remover peça
void controlPiece(Part *part, int *blockNum); // move as peças a partir de alguma entrada

void main(){
    signal(SIGINT, handle_int); // intercept SIGINT
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
    while (!flag) {
        insertPart(blockNum, part.posX, part.posY);
        drawGame();  // Desenha o jogo no terminal
        eraseParts(blockNum, part.posX, part.posY);
        
        // Checa colisão com o fundo
        if (part.posX + 2 < WIDTH - 1) { // aparentemente essa verificação pode dar erro posteriormente
            editPart(&part, (part.posX + 1), part.posY);
        } else {
            insertPart(blockNum, part.posX, part.posY);  // Fixa a peça no lugar
            break;
        }
        delay(300);
    }
}

void delay(int delayTime){
    int miliSec = 1000 * delayTime;
    clock_t startTime = clock();
    while(clock() < startTime + miliSec);
}

// adiciona o valor 7 a matriz do jogo no formato da peça selecionada
void insertPart(int numPart, int posX, int posY){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(tetrimonios[numPart][i][j] != 0){
                displayGame[posX + i][posY + j] = 7;
            }
        }
    }
}

// apaga a peça da posição, colocando o 0 de volta ao local
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

void controlPiece(Part *part, int *blockNum) {
    int ch = getch();
    eraseParts(*blockNum, part->posX, part->posY);

    switch(ch) {
        case KEY_LEFT:
            if (part->posY > 1) part->posY--;
            break;
        case KEY_RIGHT:
            if (part->posY + 4 < HEIGHT - 1) part->posY++;
            break;
        case KEY_DOWN:
            if (part->posX + 4 < WIDTH - 1) part->posX++;
            break;
        case 'q':  // Tecla 'q' para sair do jogo
            flag = 1;
            break;
        default:
            break;
    }
}