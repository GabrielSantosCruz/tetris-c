#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include <ncurses.h>
#include <unistd.h> // Para usleep()
#include "vga.h"
#include <stdbool.h>
#include "button.h"
#include "ADXL345.h"
#include <pthread.h>

#define WIDTH 10
#define HEIGHT 20
#define TETROMINOES_QUANT 13

/* #define PUSHBUTTONS ((volatile long *) 0xFF200050)
#define EDGE_CAPTURE ((volatile long *) 0xFF20005C) */

#define RLEDs ((volatile long *) 0xFF200000)

// Matriz de tetrominos
int tetrominoes[TETROMINOES_QUANT][4][4] = {
    // falta colocar/mudar as peças 
    { 
        {1, 1},
        {0, 0}
    },{ 
        {1, 0},
        {1, 0}  
    },{ 
        {0, 1},
        {1, 1}
    }, { 
        {1, 0},
        {1, 1}
    },{ 
        {1, 1},
        {1, 0}
    },{ 
        {1, 1},
        {0, 1}
    },{ 
        {1, 1},
        {1, 1}
    },
    { 
        {0, 0, 0},
        {1, 1, 1},
        {0, 0, 0}
    },{ 
        {0, 1, 0},
        {0, 1, 0},
        {0, 1, 0}
    },{
        {0, 1, 0},
        {1, 1, 1},
        {0, 0, 0}
    },{
        {0, 1, 0},
        {0, 1, 1},
        {0, 1, 0}
    },{
        {0, 1, 0},
        {1, 1, 0},
        {0, 1, 0}
    },{
        {0, 0, 0},
        {1, 1, 1},
        {0, 1, 0}
    }
};

// Campo de jogo
int board[HEIGHT][WIDTH] = {0};


int currentX, currentY; // Posição atual da peça
int currentPiece;   // Índice da peça atual
int nextPiece;      // Índice da próxima peça
bool isPaused = false; // estado de pausa do jogo
bool isStarted = false;
int score = 0;
pthread_mutex_t pauseMutex;

int botao;

void* monitorButton(void* arg) {
    while (1) { // Use 1 para loop infinito em C
        int buttonState = buttonPressed(); // Obter estado do botão

        // printf("botao state = %d\n", buttonState);

        pthread_mutex_lock(&pauseMutex); // Bloquear mutex para sincronização

        if (buttonState == 1) {
            isPaused = !isPaused; // Alterna o estado de pausa
            printf("Botão 0 pressionado\n");
            // gamePaused(); // Descomente caso seja necessário
        } else if (buttonState == 2) {
            isStarted = !isStarted;
            printf("Botão 1 pressionado\n");
        } 
        
        pthread_mutex_unlock(&pauseMutex); // Desbloquear o mutex
        usleep(10000); // Pausar a thread por 10 ms
    }
    return NULL;
}


// Função para escolher um índice de peça aleatório
int getRandomPiece() {
    return rand() % TETROMINOES_QUANT; // Escolhe aleatoriamente um índice entre 0 e 6
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

/* Função para verificar colisão  
Rebebe: a posicao x e y da peca
*/
bool checkCollision(int x, int y) {
    int pieceHeight = (currentPiece == 1) ? 2 : 4; // Peça O tem altura 2
    for (int i = 0; i < pieceHeight; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetrominoes[currentPiece][i][j] && 
                (x + j < 0 || x + j >= WIDTH || y + i >= HEIGHT || board[y + i][x + j])) {
                return true;
            }
        }
    }
    return false;
}

/* Função que remove as linhas que estão completas da matriz do jogo e move tudo que está em cima para baixo */
void removeFullLines() {
    for (int i = HEIGHT - 1; i >= 0; i--) {
        bool fullLine = true;
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == 0) {
                fullLine = false;
                break;
            }
        }
        if (fullLine) {
            // Remove a linha e atualiza a pontuação
            score += 100; // Adiciona 100 pontos por linha completa
            // Move todas as linhas acima uma linha para baixo
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < WIDTH; j++) {
                    board[k][j] = board[k - 1][j];
                }
            }
            // Limpa a linha superior
            for (int j = 0; j < WIDTH; j++) {
                board[0][j] = 0;
            }
            i++; // Verifica a linha novamente
        }
    }
}

// Função para adicionar a peça ao campo (ajustada para lidar com tamanhos de peças menores)
void placePiece() {
    int pieceHeight = (currentPiece == 1) ? 2 : 4; // Peça O tem altura 2
    for (int i = 0; i < pieceHeight; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetrominoes[currentPiece][i][j]) {
                board[currentY + i][currentX + j] = 1;
            }
        }
    }
    removeFullLines();
}

/* Função para mover a peça 
Recebe: o valor da variação de x e/ou y para movimentar a peça
*/
void movePiece(int dx, int dy) {
    if (!checkCollision(currentX + dx, currentY + dy)) {
        currentX += dx;
        currentY += dy;
    } else if (dy != 0) {
        placePiece();
        initializeCurrentPiece();
        currentX = 0;//WIDTH / 2 - 2;
        currentY = 0;
    }
}

// Função para inicializar o jogo, definindo as posições das peças e o valor randomico delas
void initGame() {
    // srand(time(NULL));
    initializeCurrentPiece();
    currentX = 0;//WIDTH / 2 - 2;
    currentY = 0;
}

// Função para verificar se o jogo acabou
bool isGameOver() {
    return checkCollision(currentX, currentY); // Se houver colisão no topo, o jogo acaba
}


// Função principal do jogo
void gameLoop() {
    
    int ch;
    uint8_t idAccel; 
    int16_t XYZ[3];
    int16_t direction;

    idAccel = configureAccel(); 
    //printf("%p\n",idAccel); 


    initGame();
    // Controle de tempo para a animação
    int speed = 500; // Velocidade de queda (500 milissegundos)
    clock_t lastMoveTime = clock();

    char resultado[40];

    if (idAccel == 0xE5){
        while (1) {
            // buttonPressed();
            if (isGameOver()) { // Verifica se o jogo acabou
                // clear();
                // mvprintw(HEIGHT / 2, WIDTH / 2 - 5, "GAME OVER");
                
                // refresh();
                gameOver();
                usleep(3000000); // Pausa de 3 segundos para exibir o Game Over
                // endwin(); // Fecha a janela ncurses
                exit(0); // Encerra o programa
            }


            if (!isPaused) {
                sprintf(resultado, "pontuacao: %d", score);
                // drawBoard(); // Desenha o tabuleiro enquanto não está pausado
                drawMonitor(HEIGHT, WIDTH, board, 4, 4, currentPiece, tetrominoes, currentX, currentY, nextPiece);
                // Controle do tempo para a peça descer automaticamente
                clock_t currentTime = clock();
                if ((currentTime - lastMoveTime) * 10000 / CLOCKS_PER_SEC > speed) {
                    movePiece(0, 1); // Move a peça para baixo automaticamente
                    lastMoveTime = currentTime; // Atualiza o tempo da última queda
                }
            } else {
                gamePaused();
            }
            
            // Verifica se o usuário pressionou alguma tecla
            if (ADXL345_WasActivityUpdated()){
                ADXL345_XYZ_Read(XYZ);
                direction = moviment((XYZ[0]*4)); //4 = mg_per_lsb
                switch (direction) { //moviment in ch 
                    case 1: // Mover para a direita
                        if (!isPaused) movePiece(1, 0);
                        break;
                    case 2: // Mover para a esquerda
                        if (!isPaused) movePiece(-1, 0);
                        break;
                    }
                }       
                usleep(50000); // Pequeno atraso para evitar uso excessivo de CPU
        }
    }
    
}

int main() {
    
    pthread_t buttonThread; // Declara a thread
    pthread_mutex_init(&pauseMutex, NULL);

    // Cria a thread que irá monitorar o botão
    if (pthread_create(&buttonThread, NULL, monitorButton, NULL) != 0) {
        perror("Erro ao criar a thread do botão");
        return 1; // Erro ao criar a thread
    }
    while(1){
        
        if(!isStarted){
            mainWindow();
            
        } else {
            initializeNextPiece();
            gameLoop(); // Inicia o loop do jogo
        }
    }
    return 0;
}
