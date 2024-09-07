#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#define HEIGHT 10
#define WIDTH 20

int displayGame[WIDTH][HEIGHT] = {
    {5, 2, 2, 2, 2, 2, 2, 2, 2, 6},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {3, 2, 2, 2, 2, 2, 2, 2, 2, 4}};

void drawGame();
void clearTerminal(); // for linux
volatile sig_atomic_t flag = 0;
void handle_int(int sig);
void fallTest();
void delay(int delayTime);

void main(){
    signal(SIGINT, handle_int); // intercept SIGINT
    fallTest();
    /* while(!flag){ // ctrl z for stop
        clearTerminal();
        drawGame();
    } */
}

void drawGame(){
    for(int i = 0; i < WIDTH; i++){
        for(int j = 0; j < HEIGHT; j++){
            switch (displayGame[i][j])
            {
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
    for(int i = 0; i < 18; i++){
        clearTerminal();
        displayGame[i+1][5] = 7;
        drawGame();
        displayGame[i+1][5] = 0;
        delay(200);
    }
}

void delay(int delayTime){
    int miliSec = 1000 * delayTime;
    clock_t startTime = clock();
    while(clock() < startTime + miliSec);
}