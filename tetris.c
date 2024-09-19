#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "vga.h"

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
    {0, 0, 0, 0},
    {7, 7, 7, 7},
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


void drawGame(); // desenha a matriz do jogo na tela do terminal
void clearTerminal(); // for linux
volatile sig_atomic_t flag = 0;
void handle_int(int sig); 
void fallTest();
void delay(int delayTime); // adiciona um delay na execução do codigo
void insertPart(int numPart, int posX, int posY); // adiciona as peças na matriz do jogo
void eraseParts(int numPart, int posX, int posY); // remove as peças da matriz do jogo
void draw(); // executa as funções de adicionar a peça, desenhar e remover peça
int isValidMove(int numPart, int posX, int posY); // verifica se o movimento horizontal é válido
int isValidHorizontalMove(int numPart, int posX, int posY, int deltaX); // verifica se o movimento horizontal é válido
void horizonMove(int numPart, Part *part, int deltaX);

void main(){
    signal(SIGINT, handle_int); // Intercepta SIGINT
    srand(time(NULL));          // Inicializa o gerador de números aleatórios
    initializeNextPiece();     // Inicializa a primeira peça
    fallTest();
}

int currentPiece;   // Índice da peça atual
int nextPiece;      // Índice da próxima peça

// Função para escolher um índice de peça aleatório
int getRandomPiece() {
    return rand() % 7; // Escolhe aleatoriamente um índice entre 0 e 6
}

// Inicializa a próxima peça
void initializeNextPiece() {
    nextPiece = getRandomPiece();
}

// Inicializa a peça atual
void initializeCurrentPiece() {
    currentPiece = nextPiece; // A peça atual é a próxima peça
    initializeNextPiece();    // Define a próxima peça para a próxima vez
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
void fallTest() {
    // Inicializa a primeira peça
    initializeCurrentPiece();

    Part part = createPart(1, 5); // Começa na parte superior central
    int canMove = 1;
    
    while (canMove) {
        insertPart(currentPiece, part.posX, part.posY);
        drawGame();
        
        drawMonitor(WIDTH, HEIGHT, displayGame, nextPiece, 4, 4, tetrimonios);

        eraseParts(currentPiece, part.posX, part.posY);

        // Tenta mover a peça para baixo
        if (isValidMove(currentPiece, part.posX + 1, part.posY)) {
            editPart(&part, part.posX + 1, part.posY);
        } else {
            // Não é possível mover para baixo, fixa a peça e inicia uma nova
            insertPart(currentPiece, part.posX, part.posY);
            // Verifica se a nova peça pode ser colocada
            if (!isValidMove(nextPiece, 1, 5)) {
                printf("Game Over\n");
                break; // Fim do jogo
            }
            // Inicializa a próxima peça
            initializeCurrentPiece();
            part = createPart(1, 5); // Reseta a posição da nova peça
        }

        delay(300);

        // Simulação de entrada para movimento horizontal
        // Para teste, vamos mover a peça para a direita ou esquerda
        /* if (/* condição para mover para a direita ) {
            moveHorizontal(blockNum, &part, 1); // Move para a direita
        } else if (/* condição para mover para a esquerda ) {
            moveHorizontal(blockNum, &part, -1); // Move para a esquerda
        } */
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

int isValidMove(int numPart, int posX, int posY) {
    // Verifica se as coordenadas estão dentro dos limites da matriz
    if (posX < 0 || posY < 0 || posX + 4 > WIDTH +1|| posY + 4 > HEIGHT) {
        return 0; // Fora dos limites
    }

    // Verifica se a posição da peça colide com outra peça já colocada
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetrimonios[numPart][i][j] != 0) {
                int newX = posX + i;
                int newY = posY + j;

                // Se a nova posição estiver ocupada por uma peça existente
                if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT) {
                    if (displayGame[newX][newY] != 0) {
                        return 0; // Colidiu com uma peça existente
                    }
                } else {
                    return 0; // Fora dos limites
                }
            }
        }
    }
    return 1; // Movimento válido
}

int isValidHorizontalMove(int numPart, int posX, int posY, int deltaX) {
    // deltaX é o deslocamento horizontal desejado (positivo para direita, negativo para esquerda)

    // Verifica se as coordenadas estão dentro dos limites da matriz
    if (posX + deltaX < 0 || posX + deltaX + 4 > WIDTH) {
        return 0; // Fora dos limites horizontais
    }

    // Verifica se a posição da peça colide com outra peça já colocada
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetrimonios[numPart][i][j] != 0) {
                int newX = posX + deltaX + i;
                int newY = posY + j;

                // Se a nova posição estiver ocupada por uma peça existente
                if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT) {
                    if (displayGame[newX][newY] != 0 && displayGame[newX][newY] != 7) {
                        return 0; // Colidiu com uma peça existente
                    }
                } else {
                    return 0; // Fora dos limites horizontais
                }
            }
        }
    }
    return 1; // Movimento horizontal válido
}

void horizonMove(int numPart, Part *part, int deltaX) {
    // Tenta mover a peça horizontalmente
    if (isValidHorizontalMove(numPart, part->posX, part->posY, deltaX)) {
        eraseParts(numPart, part->posX, part->posY); // Remove a peça da posição atual
        part->posX += deltaX; // Atualiza a posição horizontal
        insertPart(numPart, part->posX, part->posY); // Insere a peça na nova posição
    }
}
