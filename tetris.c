#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

// Tamanho do campo
#define WIDTH 13
#define HEIGHT 20
#define QUANT_TETRIMONIO 7

/* Definição das peças do jogo (Tetrimonios) */
int tetrominos[QUANT_TETRIMONIO][4][4] = {
    {
        {1, 1, 1, 1}, 
        {0, 0, 0, 0}, 
        {0, 0, 0, 0}, 
        {0, 0, 0, 0}},
    {
        {1, 0, 0, 0}, 
        {1, 1, 1, 0}, 
        {0, 0, 0, 0}, 
        {0, 0, 0, 0}}, 
    {
        {0, 0, 1, 0}, 
        {1, 1, 1, 0}, 
        {0, 0, 0, 0}, 
        {0, 0, 0, 0}}, 
    {
        {1, 1, 0, 0}, 
        {1, 1, 0, 0}, 
        {0, 0, 0, 0}, 
        {0, 0, 0, 0}}, 
    {
        {0, 1, 1, 0}, 
        {1, 1, 0, 0}, 
        {0, 0, 0, 0}, 
        {0, 0, 0, 0}}, 
    {
        {0, 1, 0, 0}, 
        {1, 1, 1, 0}, 
        {0, 0, 0, 0}, 
        {0, 0, 0, 0}}, 
    {
        {1, 1, 0, 0}, 
        {0, 1, 1, 0}, 
        {0, 0, 0, 0}, 
        {0, 0, 0, 0}}, 
};

// Estado do jogo
int field[HEIGHT][WIDTH];  // deixar uma matriz já prédefinida
int currentPiece, currentRotation, currentX, currentY; // dados das peças
int score = 0;  // Pontuação do jogador

/* Inicia a matriz do jogo colocando todos os seus valores com 0 */
void initGame() { // ou colocar outros valores para as bordas do campo aqui
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            field[y][x] = 0;
        }
    }
}

/* Desenha o campo do jogo e a pontuação no terminal */
void drawField() {
    clear();
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            // adicionar switch case aqui
            if (field[y][x] == 1) {
                mvprintw(y, x, "@");
            } else {
                mvprintw(y, x, ". ");
            }
        }
    }
    // Exibe a pontuação no canto da tela
    mvprintw(0, WIDTH + 2, "Score: %d", score);
}

/* Desenha as peças, 
Recebe: as informações da peça
*/
void drawPiece(int piece, int rotation, int posX, int posY) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (tetrominos[piece][rotation][y * 4 + x] == 1) {
                mvprintw(posY + y, posX + x, "@");
            }
        }
    }
}

/* 
Função para checar as colisões com outras peças e as bordas do jogo
Recebe: as informações da peça a ser checada
Retorna: 1 caso haja colisão e 0 caso contrário
*/
int checkCollision(int piece, int rotation, int posX, int posY) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            int block = tetrominos[piece][rotation][y * 4 + x];
            if (block) {
                int newX = posX + x;
                int newY = posY + y;
                if (newX < 0 || newX >= WIDTH || newY >= HEIGHT || (newY >= 0 && field[newY][newX] != 0)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* 
Adiciona o valor 1 na matriz do jogo para que a função de desenhar o tabuleiro também desenhe a peça
Recebe: as informações da peça como argumentos
*/
void placePiece(int piece, int rotation, int posX, int posY) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (tetrominos[piece][rotation][y * 4 + x] == 1) {
                field[posY + y][posX + x] = 1;
            }
        }
    }
}

/* 
Gera randomicamente uma nova peça após a primeira ser fixada, e também checa se essa peça já não entra gerando uma colisão, causando assim o fim do jogo
*/
void spawnPiece() {
    currentPiece = rand() % QUANT_TETRIMONIO;
    currentRotation = 0;
    currentX = WIDTH / 2 - 2;
    currentY = 0;

    // Se a nova peça já colide ao ser gerada, o jogo termina
    if (checkCollision(currentPiece, currentRotation, currentX, currentY)) {
        endwin();
        printf("Game Over!\n");
        printf("Your Score: %d\n", score);
        exit(0);
    }
}

/* 
Faz a verificação na matriz, para fazer a limpeza das linhas completas e também faz a contagem de pontos
*/
void clearLines() {
    int linesCleared = 0;
    for (int y = 0; y < HEIGHT; y++) {
        int fullLine = 1;
        for (int x = 0; x < WIDTH; x++) {
            if (field[y][x] == 0) {
                fullLine = 0;
                break;
            }
        }
        if (fullLine) {
            linesCleared++;
            for (int ny = y; ny > 0; ny--) {
                for (int x = 0; x < WIDTH; x++) {
                    field[ny][x] = field[ny - 1][x];
                }
            }
            for (int x = 0; x < WIDTH; x++) {
                field[0][x] = 0;
            }
        }
    }
    // Adiciona pontos dependendo de quantas linhas foram limpas
    if (linesCleared == 1) score += 100;
    else if (linesCleared == 2) score += 300;
    else if (linesCleared == 3) score += 500;
    else if (linesCleared == 4) score += 800;
}

int main() {
    // Inicializa ncurses e configurações
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    timeout(100);  // Tempo de espera para input
    keypad(stdscr, TRUE);

    srand(time(0));
    initGame();
    spawnPiece();

    clock_t lastFall = clock();  // Tempo da última queda
    int fallInterval = 2;  // Intervalo de queda em milissegundos

    while (1) {
        clock_t now = clock();
        int ch = getch(); // para receber as entradas do teclado
        int newX = currentX;
        int newY = currentY;
        int newRotation = currentRotation;

        // Movimentação do jogador
        switch (ch) {
            case KEY_LEFT:
                newX--;
                break;
            case KEY_RIGHT:
                newX++;
                break;
            case KEY_DOWN:
                newY++;
                break;
            case 'q':  // Sair do jogo
                endwin();
                return 0;
        }

        // Verifica colisão antes de mover a peça
        if (!checkCollision(currentPiece, currentRotation, newX, newY)) {
            currentX = newX;
            currentY = newY;
        }

        // queda automática da peça
        if ((now - lastFall) * 1000 / CLOCKS_PER_SEC >= fallInterval) {
            if (!checkCollision(currentPiece, currentRotation, currentX, currentY + 1)) {
                currentY++;
            } else {
                // Coloca a peça no campo e gera uma nova
                placePiece(currentPiece, currentRotation, currentX, currentY);
                clearLines();
                spawnPiece();
            }
            lastFall = now;
        }

        // Desenha o campo e a peça
        drawField();
        drawPiece(currentPiece, currentRotation, currentX, currentY);
        refresh(); // para atualizar o terminal do computador   
    }

    endwin();
    return 0;
}
