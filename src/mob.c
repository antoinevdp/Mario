#include <stdio.h>
#include <ncurses.h>
#include "main.h"
#include "mob.h"



void gravity();
int canGoLeft();
int canGoRight();
int checkPossibleMove(int y,int x);

void moveMob(){
    while(canGoLeft())
    mvprintw()
}

int canGoRight(){
    int somme = 0;
    for (int i = 0; i < MOB_DIMENSION; ++i) somme += checkPossibleMove(player_y - i, player_x+1);
    if(somme != 0) return 0;
    else return 1;
}
int canGoLeft(){
    int somme = 0;
    for (int i = 0; i < MOB_DIMENSION; ++i)  somme += checkPossibleMove(player_y - i, player_x-3);
    if(somme != 0) return 0;
    else return 1;
}