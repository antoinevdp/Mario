//
// Created by lucas on 18/11/2021.
//

#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include "map.h"

#ifndef INNOVATION_MAIN_H
#define INNOVATION_MAIN_H

//region prototypes
//void drawMap(char *filename);
void startPlayerColor();
void startPlayerStarColor();
void displayCharacter();
void resetCharacterDisplay();
int checkPossibleMove(int y, int x);
void getNextTypeBlockUpAndDown();
int canGoRight();
int canGoLeft();
int canJump();
int canDrop();
void movePlayer(int m_direction);
void jump();
void blockMystereDisplay();
void gravity();
void spawnPower();
//endregion prototypes


void Setup(){
    initscr(); //initialisation de la fenetre ncurses
    raw();
    curs_set(0); //on enleve le curseur
    keypad(stdscr, TRUE); //initialisation de la recuperation des inputs
    noecho();
    drawMap("../bin/ascii-art.txt"); //On dessine la map sur l'écran
}

void Shutdown(){
    endwin();			/* Stop */
}

int GetKey(){
    return getch(); //recupere une entree clavier
}

void UpdateScreen(){
    refresh(); //rafraichi la fenetre ncurses
}

#endif //INNOVATION_MAIN_H
