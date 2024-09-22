#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h> // Para usleep()
#include "vga.h"

#define WIDTH 10
#define HEIGHT 20
#define TETROMINOES_QUANT 7

// Matriz de tetrominos
int tetrominoes[TETROMINOES_QUANT][4][4] = {
    { // I
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    { // O
        {1, 1},
        {1, 1}
    },
    { // T
        {0, 1, 0},
        {1, 1, 1},
        {0, 0, 0}
    },
    { // J
        {1, 0, 0},
        {1, 1, 1},
        {0, 0, 0}
    },
    { // L
        {0, 0, 1},
        {1, 1, 1},
        {0, 0, 0}
    },
    { // S
        {0, 1, 1},
        {1, 1, 0},
        {0, 0, 0}
    },
    { // Z
        {1, 1, 0},
        {0, 1, 1},
        {0, 0, 0}
    }
};

// Campo de jogo
int board[HEIGHT][WIDTH] = {0};

// Posição atual da peça
int currentX, currentY;
int currentPiece;   // Índice da peça atual
int nextPiece;      // Índice da próxima peça

// Função para escolher um índice de peça aleatório
int getRandomPiece() {
    return rand() % TETROMINOES_QUANT; // Escolhe aleatoriamente um índice entre 0 e 6
}

// Inicializa a próxima peça
void initializeNextPiece() {
    nextPiece = getRandomPiece();
    printw("%d", nextPiece);
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

/* Função para desenhar o campo de jogo (com a peça temporária)
*/
void drawBoard() {
    clear(); // Usa ncurses para limpar a tela
    
    // Desenha o tabuleiro fixo
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == 0) {
                printw("."); // Ponto vazio
            } else {
                printw("#"); // Bloco preenchido
            }
        }
        printw("\n");
    }
    
    // Desenha a peça temporariamente (antes de colidir)
    int pieceHeight = (currentPiece == 1) ? 2 : 4;
    for (int i = 0; i < pieceHeight; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetrominoes[currentPiece][i][j]) {
                mvprintw(currentY + i, currentX + j, "#");
            }
        }
    }

    // Desenha a próxima peça
    mvprintw(1, 11, "PROXIMA PECA:");
    int nextPieceHeight = sizeof(tetrominoes[nextPiece]) / sizeof(tetrominoes[nextPiece][0]);
    for (int i = 0; i < nextPieceHeight; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetrominoes[nextPiece][i][j]) {
                mvprintw(2 + i, 11 + j, "@"); // Ajuste as coordenadas (30, 12) conforme necessário
            }
        }
    }


    refresh(); // Atualiza a tela ncurses
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
        currentX = WIDTH / 2 - 2;
        currentY = 0;
    }
}

// Função para inicializar o jogo, definindo as posições das peças e o valor randomico delas
void initGame() {
    srand(time(NULL));
    initializeCurrentPiece();
    currentX = WIDTH / 2 - 2;
    currentY = 0;
}

// Função para verificar se o jogo acabou
bool isGameOver() {
    return checkCollision(currentX, currentY); // Se houver colisão no topo, o jogo acaba
}

// Função principal do jogo
void gameLoop() {
    int ch;
    initGame();
    
    // Controle de tempo para a animação
    int speed = 500; // Velocidade de queda (500 milissegundos)
    clock_t lastMoveTime = clock();
    
    nodelay(stdscr, TRUE); // Faz com que getch() não bloqueie o loop (para entradas com o teclado)
    while (1) {
        if (isGameOver()) { // condicao para mostrar a tela de fim de jogo
            clear();
            mvprintw(HEIGHT / 2, WIDTH / 2 - 5, "GAME OVER");
            refresh();
            usleep(3000000); // Pausa de 3 segundos para exibir o Game Over
            endwin(); // Fecha a janela ncurses
            exit(0); // Encerra o programa
        }
        drawBoard(); // Desenha o tabuleiro
        
        ch = getch(); // Captura a tecla pressionada (não bloqueia o programa)
        
        // Verifica se o usuário pressionou alguma tecla
        if (ch != ERR) {
            switch (ch) {
                case 'a': // Mover para a esquerda
                    movePiece(-1, 0);
                    break;
                case 'd': // Mover para a direita
                    movePiece(1, 0);
                    break;
                case 's': // Mover mais rápido para baixo
                    movePiece(0, 1);
                    break;
                case 'q': // Sair do jogo
                    endwin(); // Fecha a janela ncurses
                    exit(0);
            }
        }
        
        // Controle de tempo para a peça descer automaticamente
        clock_t currentTime = clock();
        if ((currentTime - lastMoveTime) * 100000 / CLOCKS_PER_SEC > speed) {
            movePiece(0, 1); // Move a peça para baixo automaticamente
            lastMoveTime = currentTime; // Atualiza o tempo da última queda
        }
        
        usleep(50000); // Pequeno atraso para evitar uso excessivo de CPU
    }
}

int main() {
    initscr(); // Inicializa a tela ncurses
    noecho(); // Não exibe as teclas digitadas
    cbreak(); // Desativa o buffer de linha
    keypad(stdscr, TRUE); // Habilita teclas especiais
    curs_set(0); // Oculta o cursor
    initializeNextPiece();
    gameLoop(); // Inicia o loop do jogo
    endwin(); // Finaliza a janela ncurses
    return 0;
}