//
// Marios Bros Game All rights reserved
//
//region INCLUDE
#include "main.h"
#include "map.h"
#include "map.c"
#include "player.h"
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <string.h>
#include <locale.h>

//endregion INCLUDE

//region VARIABLES
char CHAR_PLAYER = 'o';
int player_x = SPAWN_POINT_X;
int player_y = SPAWN_POINT_Y;
int player_life = 1;
int next_pos_x = 0;
int next_pos_y = 0;
int animationFPS = 40;

int mapListInt[MAPLISTSIZE];

int power_y = 0;
int power_x = 0;

int direction = 1;

int rightWall = 0;
int leftWall = 0;


int nextTypeBlockUp;
int nextTypeBlockDown;
int nextTypeBlockRight;
int nextTypeBlockLeft;

char previousInputList[1];

time_t mytime_start;
char * time_str_start;
time_t mytime_end;
char * time_str_end;
//endregion VARIABLES


//region main
int main ( int argc, char **argv) {
    setlocale(LC_ALL, "");
    //region variables
    WINDOW *win;
    int input; // entree clavier
    int screen_width, screen_height; //taille de l'écran
    memcpy(intMapList, mapListInt, MAPLISTSIZE);
    //endregion variables

    Setup(); //Start ncurses
    getmaxyx(stdscr, screen_height, screen_width); //recupere la taille du terminal
    // si en plein ecran
    if (!(screen_height<SCREEN_HEIGHT || screen_width<SCREEN_WIDTH)){
        // Tant que l'entrée clavier n'est pas 'q'
        startPlayerColor();

        while ((input = getch()) != 'x'){
            if(player_y >= SCREEN_HEIGHT) { Shutdown(); exit(1); }
            if(player_life <= 0) { Shutdown(); exit(1); }
            resetCharacterDisplay();
            //play
            getNextTypeBlocks();

            if(nextTypeBlockUp == 3 || nextTypeBlockDown == 3 || nextTypeBlockRight == 3 || nextTypeBlockLeft == 3){
                player_life--;
                if(previousInputList[0] == 'q') player_x+=4;
                if(previousInputList[0] == 'd') player_x-=4;
                startPlayerColor();
            }

            if(input == 'd'){
                previousInputList[0] = input;
                mytime_start = time(NULL);
                movePlayer(1);
            }else if(input == 'q'){
                previousInputList[0] = input;
                mytime_start = time(NULL);
                movePlayer(-1);
            } else if(input == ' '){
                mytime_end = time(NULL);
                double time_diff = difftime(mytime_end, mytime_start);
                if (time_diff >= 0.5) previousInputList[0] = input;
                if (canJump(player_y, player_x, CHAR_DIMENSION)){
                    jump(previousInputList[0]); // Sinon, on saute en ligne droite
                }
            } else{
                gravity();
                powerBoxMovements();
            }
            resetCharacterDisplay();
            displayCharacter();
            // On actualise l'écran
            UpdateScreen();
            flushinp();
        }
        Shutdown();
        exit(1);

    }else{ //si l'écran n'est pas en plein écran
        Shutdown(); //on ferme
        printf("Mettre la fenetre en plein ecran svp !\n");
        return 0; // On quitte le programme
    }

}
//endregion main

//region Functions

//region CHARACTER MOVEMENT
void movePlayer(int m_direction){
    resetCharacterDisplay();
    if(m_direction == 1 && canGoRight(player_y, player_x, CHAR_DIMENSION) == 1){
        player_x++;
    }else if(m_direction == -1 && canGoLeft(player_y, player_x, CHAR_DIMENSION) == 1){
        player_x--;
    }
}

// Fonction pour faire sauter le personnage en ligne droite
void jump(int previousInput){
    // BAS vers HAUT -> Pour i de 1 jusqu'a la hauteur de saut du personnage
    for (int i = 1; i <= JUMP_STRENGHT; ++i) {
        getNextTypeBlocks();
        resetCharacterDisplay();
        if (canJump(player_y, player_x, CHAR_DIMENSION) == 1){// Si le bloc est un bloc vide, on monte
            player_y-=0.01*i;
            if(previousInput == 'd' && canJump(player_y, player_x, CHAR_DIMENSION) == 1 && canGoRight(player_y, player_x, CHAR_DIMENSION)){
                player_x++;
            } else if(previousInput == 'q' && canJump(player_y, player_x, CHAR_DIMENSION) == 1 && canGoLeft(player_y, player_x, CHAR_DIMENSION)){
                player_x--;
            }
        }
        if(nextTypeBlockUp == POWER_BOX_VALUE){
            blockMystereDisplay(); // Si c'est un bloc mystere, on display et on arrête de monter
            break;
        }

        displayCharacter();
        napms(animationFPS); // on laisse un delai pour l'animation
        UpdateScreen(); // On update le visuel
    }

    // HAUT vers BAS -> Tant que i est plus grand que 1
    //Tant que le bloc en dessous est un bloc vide
    int hasTouched = 0;
    int counter = 0;
    while (canDrop(player_y, player_x, CHAR_DIMENSION) == 1){
        counter += 1;
        resetCharacterDisplay();
        player_y++;
        if (counter >= 5) hasTouched = 1;

        if (canGoRight(player_y, player_x, CHAR_DIMENSION) != 1 || canGoLeft(player_y, player_x, CHAR_DIMENSION) != 1) hasTouched = 1;
        if (previousInput == 'd' && canDrop(player_y, player_x, CHAR_DIMENSION) == canGoRight(player_y, player_x, CHAR_DIMENSION) == 1 && hasTouched == 0)  player_x++;
        else if (previousInput == 'q' && canDrop(player_y, player_x, CHAR_DIMENSION) == canGoLeft(player_y, player_x, CHAR_DIMENSION) == 1 && hasTouched == 0) player_x--;

        displayCharacter();
        napms(animationFPS);
        UpdateScreen();
    }
    getNextTypeBlocks();
    flushinp();
}

void powerBoxMovements(){
    if(power_x != 0 && (canDrop(power_y, power_x, 1) == 1 || (canGoRight(power_y, power_x, 1) == 1) || (canGoLeft(power_y, power_x, 1) == 1))){
        mvprintw(power_y, power_x, " ");
        if(canDrop(power_y, power_x, 1) == 1) power_y++;

        if((canGoRight(power_y, power_x, 1) == 1) && (direction == 1)) direction=1;
        else{
            rightWall = power_x+1;
            direction = -1;
        }
        if((canGoLeft(power_y, power_x, 1) == 1) && (direction == -1)) direction=-1;
        else{
            leftWall = power_x-1;
            direction = 1;
        }
        power_x += direction;
        mvprintw(power_y, power_x, "o");
        napms(animationFPS);
    }

}

void gravity(){
    if (canDrop(player_y, player_x, CHAR_DIMENSION) == 1){
        resetCharacterDisplay();
        player_y++;
        displayCharacter();
        UpdateScreen();
    }
    flushinp();
}
//endregion CHARACTER MOVEMENT

//region CHARACTER DISPLAY
void displayCharacter(){
    mvaddch(player_y+Y_OFFSET_HEAD,player_x+X_OFFSET_HEAD,CHAR_HEAD);
    mvaddch(player_y+Y_OFFSET_BODY,player_x+X_OFFSET_BODY,CHAR_BODY);
    mvaddch(player_y+Y_OFFSET_ARM_L,player_x+X_OFFSET_ARM_L,CHAR_ARM_L);
    mvaddch(player_y+Y_OFFSET_ARM_R,player_x+X_OFFSET_ARM_R,CHAR_ARM_R);
    mvaddch(player_y+Y_OFFSET_FOOT_L,player_x+X_OFFSET_FOOT_L,CHAR_FOOT_L);
    mvaddch(player_y+Y_OFFSET_FOOT_L,player_x+X_OFFSET_FOOT_L+1, ' ');
    mvaddch(player_y+Y_OFFSET_FOOT_R,player_x+X_OFFSET_FOOT_R,CHAR_FOOT_R);
    if(((player_x == power_x) && (player_y == power_y)) || ((player_x-1 == power_x) && (player_y == power_y)) || ((player_x-2 == power_x) && (player_y == power_y))){
        startPlayerStarColor();
        player_life++;
    }
}

void startPlayerColor(){
    start_color();
    //initialisation des couleurs pour chaque objet
    init_pair(50, COLOR_RED, COLOR_BLUE);
    attron(COLOR_PAIR(50));
}
void startPlayerStarColor(){
    power_x = 0;
    power_y = 0;
    start_color();
    //initialisation des couleurs pour chaque objet
    init_pair(60, COLOR_YELLOW, COLOR_BLUE);
    attron(COLOR_PAIR(60));
}

void resetCharacterDisplay(){
    mvaddch(player_y+Y_OFFSET_HEAD,player_x+X_OFFSET_HEAD,' ');
    mvaddch(player_y+Y_OFFSET_BODY,player_x+X_OFFSET_BODY,' ');
    mvaddch(player_y+Y_OFFSET_ARM_L,player_x+X_OFFSET_ARM_L,' ');
    mvaddch(player_y+Y_OFFSET_ARM_R,player_x+X_OFFSET_ARM_R,' ');
    mvaddch(player_y+Y_OFFSET_FOOT_L,player_x+X_OFFSET_FOOT_L,' ');
    mvaddch(player_y+Y_OFFSET_FOOT_R,player_x+X_OFFSET_FOOT_R,' ');
}

//endregion CHARACTER DISPLAY

//region CAN
int canGoRight(int y, int x, int dimension){
    int somme = 0;
    for (int i = 0; i < dimension; ++i) somme += checkPossibleMove(y - i, x + 1);
    if(somme != 0) return 0;
    else return 1;
}
int canGoLeft(int y, int x, int dimension){
    int somme = 0;
    for (int i = 0; i < dimension; ++i)  somme += checkPossibleMove(y - i, x - 3);
    if(somme != 0) return 0;
    else return 1;
}
int canJump(int y, int x, int dimension){
    int somme = 0;
    for (int i = 0; i < dimension; ++i) somme += checkPossibleMove(y - 3, x - i);
    if(somme != 0) return 0;
    else return 1;
}
int canDrop(int y, int x, int dimension){
    int somme = 0;
    for (int i = 0; i < dimension; ++i) somme += checkPossibleMove(y + 1, x - i);
    if(somme != 0) return 0;
    else return 1;
}
//endregion CAN

//region CALCULATE MOVEMENTS
void getNextTypeBlocks(){
    nextTypeBlockUp = checkPossibleMove(player_y+Y_OFFSET_HEAD-1, player_x+X_OFFSET_HEAD);
    nextTypeBlockDown = new_max(checkPossibleMove(player_y+1, player_x+X_OFFSET_FOOT_L), checkPossibleMove(player_y+1, player_x+X_OFFSET_FOOT_R));
    nextTypeBlockRight = checkPossibleMove(player_y, player_x+X_OFFSET_FOOT_R+1);
    nextTypeBlockLeft = checkPossibleMove(player_y, player_x+X_OFFSET_FOOT_L-1);
}
// Fonction pour connaitre savoir si un bloc est un bloc mystère, vide ou plein
int checkPossibleMove(int y, int x){
    int blockType = fromCoordToElementInMapList(y,x); // on recupere le int en focntion des coordonnées
    if(blockType == CLOUDS_PAIR || blockType == SKY_PAIR) return 0;
    else if(blockType == POWER_BOX_PAIR) return 2;
    else if(blockType == MOB_PAIR) return 3;
    else return 1;
}
//endregion CALCULATE MOVEMENTS

//region DIVERS

void spawnPower(){
    power_y = player_y+Y_OFFSET_HEAD-3;
    power_x = player_x+X_OFFSET_HEAD;
    mvprintw(power_y, power_x, "o");
}

// Fonction pour changer la couleur du bloc mystere
void blockMystereDisplay(){
    char *ch_t = "!";
    //initialisation des couleurs
    init_pair(10, COLOR_YELLOW, COLOR_BLACK);
    attron(COLOR_PAIR(10));
    //déplace à la position où le joueur tape + power box
    if(checkPossibleMove(player_y+Y_OFFSET_HEAD-1, player_x+X_OFFSET_HEAD-1) == POWER_BOX_VALUE) mvprintw(player_y+Y_OFFSET_HEAD-1, player_x+X_OFFSET_HEAD-1, ch_t);
    if(checkPossibleMove(player_y+Y_OFFSET_HEAD-1, player_x+X_OFFSET_HEAD-2) == POWER_BOX_VALUE) mvprintw(player_y+Y_OFFSET_HEAD-1, player_x+X_OFFSET_HEAD-2, ch_t);
    if(checkPossibleMove(player_y+Y_OFFSET_HEAD-1, player_x+X_OFFSET_HEAD+1) == POWER_BOX_VALUE) mvprintw(player_y+Y_OFFSET_HEAD-1, player_x+X_OFFSET_HEAD+1, ch_t);
    if(checkPossibleMove(player_y+Y_OFFSET_HEAD-1, player_x+X_OFFSET_HEAD+2) == POWER_BOX_VALUE) mvprintw(player_y+Y_OFFSET_HEAD-1, player_x+X_OFFSET_HEAD+2, ch_t);
    if(checkPossibleMove(player_y+Y_OFFSET_HEAD-2, player_x+X_OFFSET_HEAD-1) == POWER_BOX_VALUE) mvprintw(player_y+Y_OFFSET_HEAD-2, player_x+X_OFFSET_HEAD-1, ch_t);
    if(checkPossibleMove(player_y+Y_OFFSET_HEAD-2, player_x+X_OFFSET_HEAD-2) == POWER_BOX_VALUE) mvprintw(player_y+Y_OFFSET_HEAD-2, player_x+X_OFFSET_HEAD-2, ch_t);
    if(checkPossibleMove(player_y+Y_OFFSET_HEAD-2, player_x+X_OFFSET_HEAD+1) == POWER_BOX_VALUE) mvprintw(player_y+Y_OFFSET_HEAD-2, player_x+X_OFFSET_HEAD+1, ch_t);
    if(checkPossibleMove(player_y+Y_OFFSET_HEAD-2, player_x+X_OFFSET_HEAD+2) == POWER_BOX_VALUE) mvprintw(player_y+Y_OFFSET_HEAD-2, player_x+X_OFFSET_HEAD+2, ch_t);
    mvprintw(player_y+Y_OFFSET_HEAD-1, player_x+X_OFFSET_HEAD, ch_t);
    mvprintw(player_y+Y_OFFSET_HEAD-2, player_x+X_OFFSET_HEAD, ch_t);
    int index = fromCoordToIndexInMapList(player_y+Y_OFFSET_HEAD-1, player_x+X_OFFSET_HEAD);
    attroff(COLOR_PAIR(10));
    init_pair(20, COLOR_CYAN, COLOR_BLUE);
    attron(COLOR_PAIR(20));
    startPlayerColor();
    if(mapListInt[index] == 0) spawnPower();
    mapListInt[index] = 1;
    mapListInt[index-1] = 1;
    mapListInt[index+1] = 1;
    mapListInt[index-2] = 1;
    mapListInt[index+2] = 1;
}
//endregion DIVERS
//endregion Functions
