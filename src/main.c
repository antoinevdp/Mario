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
int animationFPS = 50;

int power_y = 0;
int power_x = 0;

int nextTypeBlockUp;
int nextTypeBlockDown;
int nextTypeBlockForward;
int nextTypeBlockBackward;

int previousInputList[1];

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
    //endregion variables

    Setup(); //Start ncurses
    getmaxyx(stdscr, screen_height, screen_width); //recupere la taille du terminal
    // si en plein ecran
    if (!(screen_height<49 || screen_width<209)){
        // Tant que l'entrée clavier n'est pas 'q'
        startPlayerColor();

        while ((input = GetKey()) != 'q'){
            //play
            getNextTypeBlockUpAndDown();
            //region gravité
            gravity();
            //endregion gravité

            //region INPUT SWITCHER
            switch (input) {
                case KEY_DOWN:
                    printf("down");
                    break;
                case KEY_LEFT:
                    previousInputList[0] = input;
                    mytime_start = time(NULL);
                    movePlayer(-1);
                    //movePlayer(nextTypeBlockBackward, nextTypeBlockBackwardHead, -1);
                    break;
                case KEY_RIGHT:
                    previousInputList[0] = input;
                    mytime_start = time(NULL);
                    movePlayer(1);
                    //movePlayer(nextTypeBlockForward, nextTypeBlockForwardHead, 1);
                    break;
                default:
                    // Si touche espace appuyé
                    if (input == ' ')
                    {
                        mytime_end = time(NULL);
                        double time_diff = difftime(mytime_end, mytime_start);
                        if (time_diff >= 0.5) previousInputList[0] = input;
                        if (canJump()){
                            jump(previousInputList[0]); // Sinon, on saute en ligne droite
                        }

                        break;
                    }
                    else break;
            }
            //endregion INPUT SWITCHER
            gravity();
            //Apres avoir effectué les calculs, on bouge le joueur
            displayCharacter();
            // On actualise l'écran
            UpdateScreen();
        }
        //Si la touche 'q' est appuyé, on ferme
        Shutdown();

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
    if(m_direction == 1 && canGoRight() == 1){
        player_x++;
    }else if(m_direction == -1 && canGoLeft() == 1){
        player_x--;
    }
    else return;
}

// Fonction pour faire sauter le personnage en ligne droite
void jump(int previousInput){
    // BAS vers HAUT -> Pour i de 1 jusqu'a la hauteur de saut du personnage
    for (int i = 1; i <= JUMP_STRENGHT; ++i) {
        getNextTypeBlockUpAndDown();
        resetCharacterDisplay();
        if (canJump() == 1){// Si le bloc est un bloc vide, on monte
            player_y-=0.01*i;
            if(previousInput == KEY_RIGHT && canJump() == 1 && canGoRight()){
                player_x++;
            } else if(previousInput == KEY_LEFT && canJump() == 1 && canGoLeft()){
                player_x--;
            }
        }
        if(nextTypeBlockUp == POWER_BOX_VALUE) blockMystereDisplay(); // Si c'est un bloc mystere, on display et on arrête de monter

        displayCharacter();
        napms(animationFPS); // on laisse un delai pour l'animation
        UpdateScreen(); // On update le visuel
    }

    // HAUT vers BAS -> Tant que i est plus grand que 1
    //Tant que le bloc en dessous est un bloc vide
    int hasTouched = 0;
    int counter = 0;
    while (canDrop() == 1){
        counter += 1;
        resetCharacterDisplay();
        player_y++;
        if (counter >= 5) hasTouched == 1;

        if (canGoRight() != 1 || canGoLeft() != 1) hasTouched = 1;
        if (previousInput == KEY_RIGHT && canDrop() == canGoRight() == 1 && hasTouched == 0)  player_x++;
        else if (previousInput == KEY_LEFT && canDrop() == canGoLeft() == 1 && hasTouched == 0) player_x--;

        displayCharacter();
        napms(animationFPS);
        UpdateScreen();
    }
    getNextTypeBlockUpAndDown();
    flushinp();
    return;
}

void gravity(){
    while (canDrop() == 1){
        resetCharacterDisplay();
        player_y++;
        displayCharacter();
        napms(animationFPS);
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
int canGoRight(){
    int somme = 0;
    for (int i = 0; i < CHAR_DIMENSION; ++i) somme += checkPossibleMove(player_y - i, player_x+1);
    if(somme != 0) return 0;
    else return 1;
}
int canGoLeft(){
    int somme = 0;
    for (int i = 0; i < CHAR_DIMENSION; ++i)  somme += checkPossibleMove(player_y - i, player_x-3);
    if(somme != 0) return 0;
    else return 1;
}
int canJump(){
    int somme = 0;
    for (int i = 0; i < CHAR_DIMENSION; ++i) somme += checkPossibleMove(player_y - 3, player_x - i);
    if(somme != 0) return 0;
    else return 1;
}
int canDrop(){
    int somme = 0;
    for (int i = 0; i < CHAR_DIMENSION; ++i) somme += checkPossibleMove(player_y + 1, player_x - i);
    if(somme != 0) return 0;
    else return 1;
}
//endregion CAN

//region CALCULATE MOVEMENTS
void getNextTypeBlockUpAndDown(){
    nextTypeBlockUp = checkPossibleMove(player_y+Y_OFFSET_HEAD-1, player_x+X_OFFSET_HEAD);
    nextTypeBlockDown = checkPossibleMove(player_y+Y_OFFSET_HEAD+1, player_x+X_OFFSET_HEAD);
}
// Fonction pour connaitre savoir si un bloc est un bloc mystère, vide ou plein
int checkPossibleMove(int y, int x){
    int blockType = fromCoordToElementInMapList(y,x); // on recupere le int en focntion des coordonnées
    if(blockType == CLOUDS_PAIR || blockType == SKY_PAIR) return 0;
    else if(blockType == POWER_BOX_PAIR) return 2;
    else return 1;
}
//endregion CALCULATE MOVEMENTS

//region DIVERS

void spawnPower(){
    power_y = player_y+Y_OFFSET_HEAD-3;
    power_x = player_x+X_OFFSET_HEAD;
    mvprintw(power_y, power_x, "o");
    startPlayerColor();
}

// Fonction pour changer la couleur du bloc mystere
void blockMystereDisplay(){
    char *ch_t = "!";
    //initialisation des couleurs
    init_pair(10, COLOR_YELLOW, COLOR_BLACK);
    attron(COLOR_PAIR(10));
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
    attroff(COLOR_PAIR(10));
    init_pair(20, COLOR_CYAN, COLOR_BLUE);
    attron(COLOR_PAIR(20));
    spawnPower();
}
//endregion DIVERS
//endregion Functions
