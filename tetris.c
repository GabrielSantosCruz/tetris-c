#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
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

void drawGame(); // desenha a matriz do jogo na tela do terminal
void clearTerminal(); // for linux
volatile sig_atomic_t flag = 0;
void handle_int(int sig); 
void gravity(); // testar a queda de peças
void delay(int delayTime); // adiciona um delay na execução do codigo
void insertPart(int numPart, int posX, int posY); // adiciona as peças na matriz do jogo
void eraseParts(int numPart, int posX, int posY); // remove as peças da matriz do jogo
void draw(); // executa as funções de adicionar a peça, desenhar e remover peça
int isValidMove(int numPart, int posX, int posY); // verifica se o movimento horizontal é válido
int isValidHorizontalMove(int numPart, int posX, int posY, int deltaX); // verifica se o movimento horizontal é válido
void horizonMove(int numPart, Part *part, int deltaX);
void initializeNextPiece();
Part createPart(int posX, int posY); // cria a struct com as posições da peça no tabuleiro do jogo
void editPart(Part *part, int posX, int posY); // edita os valores da peça
void setBufferedInput(int enable);


// fução para criar a struct com as posições da peça
Part createPart(int posX, int posY){
    Part part;
    part.posX = posX;
    part.posY = posY;
    return part;
}

// função para editar a struct por referência
void editPart(Part *part, int posX, int posY){
    part->posX = posX;
    part->posY = posY;
};

// Função para configurar o terminal e desativar o modo de buffer de entrada
void setBufferedInput(int enable) {
    static struct termios oldt, newt;
    if (!enable) {
        tcgetattr(STDIN_FILENO, &oldt); // Salva os atributos atuais
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO); // Desativa o buffer de entrada e eco
        tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Aplica as mudanças imediatamente
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restaura os atributos antigos
    }
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


// Desenha a matriz do jogo no terminal
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
    printf("\033[H\033[J");
}

// função para auxiliar no cancelamento da execução
void handle_int(int sig){
  flag = 1;
}

// teste de queda do jogo
void gravity() {
    
    initializeCurrentPiece(); // Inicializa a peça atual

    Part part = createPart(1, 5); // Começa na parte superior central
    int canMove = 1;
    char input;

    setBufferedInput(0); // Desativa o buffer de entrada

    while (canMove) {
        insertPart(currentPiece, part.posX, part.posY); // Insere a peça
        drawGame(); // Desenha o estado do jogo
        
        // Função personalizada para desenhar o jogo no monitor VGA
        //drawMonitor(WIDTH, HEIGHT, displayGame, nextPiece, 4, 4, tetrimonios);

        eraseParts(currentPiece, part.posX, part.posY); // Remove a peça anterior

        // Verifica se pode cair
        if (isValidMove(currentPiece, part.posX + 1, part.posY)) {
            editPart(&part, part.posX + 1, part.posY); // Move para baixo
        } else {
            insertPart(currentPiece, part.posX, part.posY); // Fixa a peça
            if (!isValidMove(nextPiece, 1, 5)) {
                printf("Game Over\n");
                break; // Fim de jogo
            }
            initializeCurrentPiece(); // Inicializa a nova peça
            part = createPart(1, 5); // Reposiciona a nova peça
        }

        // Lê a entrada do jogador para mover a peça horizontalmente
        if (read(STDIN_FILENO, &input, 1) == 1) {
            if (input == 'a') {
                horizonMove(currentPiece, &part, -1); // Move para a esquerda
            } else if (input == 'd') {
                horizonMove(currentPiece, &part, 1); // Move para a direita
            } else if (input == 'q') {
                flag = 1; // Sai do jogo se o jogador pressionar 'q'
            }
        }

        delay(300); // Delay para controlar a queda da peça
    }

    setBufferedInput(1); // Restaura o buffer de entrada ao 
}


/* Função para adicionar delay no jogo
Recebe: o tempo em segundos
 */
void delay(int delayTime){
    usleep(delayTime * 1000); // Converte para microsegundos
}

/* Função para inserir as peças na matriz do jogo
Recebe: o número da peça e a posição X e Y
 */
void insertPart(int numPart, int posX, int posY){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(tetrimonios[numPart][i][j] != 0){
                displayGame[posX + i][posY + j] = 7;
            }
        }
    }
}
/* Função para remover a peça da matriz do jogo 
Recebe: o número da peça e a posição X e Y
*/
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

/* Função para verificar se o movimento da peça não excede o limite da matriz do jogo
Recebe: o número da peça e a posição X e Y
 */
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

/* Função para verificar se o movimento horizontal da peça é válido
Recebe: o número da peça e a posição X e Y
 */
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

/* Função para mover a peça horizontalmente
Recebe: o número da peça e a posição X e Y
 */
void horizonMove(int numPart, Part *part, int deltaX) {
    // Tenta mover a peça horizontalmente
    if (isValidHorizontalMove(numPart, part->posX, part->posY, deltaX)) {
        eraseParts(numPart, part->posX, part->posY); // Remove a peça da posição atual
        part->posY += deltaX; // Atualiza a posição horizontal
        insertPart(numPart, part->posX, part->posY); // Insere a peça na nova posição
    }
}

void main(){
    signal(SIGINT, handle_int); // Intercepta SIGINT
    srand(time(NULL));          // Inicializa o gerador de números aleatórios
    initializeNextPiece();     // Inicializa a primeira peça
    gravity();
}