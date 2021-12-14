//
// Created by lucas on 18/11/2021.
//

#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include "map.h"

#ifndef INNOVATION_MAIN_H
#define INNOVATION_MAIN_H
#define SCREEN_WIDTH 209
#define SCREEN_HEIGHT 49
#define FPS 40

#define new_max(x,y) (((x) >= (y)) ? (x) : (y))

//region prototypes
//void drawMap(char *filename);
void startPlayerColor();
void startPlayerStarColor();
void displayCharacter();
void resetCharacterDisplay();
int checkPossibleMove(int y, int x);
void getNextTypeBlocks();
void checkMobCollsion();
int canGoRight(int y, int x, int dimension);
int canGoLeft(int y, int x, int dimension);
int canJump(int y, int x, int dimension);
int canDrop(int y, int x, int dimension);
void movePlayer(int m_direction);
void powerBoxMovements();
void jump();
void blockMystereDisplay();
void gravity();
void powerGravity();
void spawnPower();
//endregion prototypes


void Setup(){
    initscr(); //initialisation de la fenetre ncurses
    curs_set(0); //on enleve le curseur
    noecho();
    keypad(stdscr, TRUE); //initialisation de la recuperation des inputs
    timeout(65);
    drawMap("../bin/ascii-art.txt"); //On dessine la map sur l'écran
}

void Shutdown(){
    endwin();			/* Stop */
}

int GetKey(){
    return getchar(); //recupere une entree clavier
}

void UpdateScreen(){
    refresh(); //rafraichi la fenetre ncurses
}

#endif //INNOVATION_MAIN_H
