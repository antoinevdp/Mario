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

int game(char *filename);

//region VARIABLES
//region player var
int player_x = SPAWN_POINT_X;
int player_y = SPAWN_POINT_Y;
int player_life = 1;

//les nextTypeBlocks sont des entiers correspondant au type de block au-dessus, en dessous, à droite et à gauche du joueur
int nextTypeBlockUp;
int nextTypeBlockDown;
int nextTypeBlockRight;
int nextTypeBlockLeft;
//endregion player var

//region map var
int mapListInt[MAPLISTSIZE]; // liste contenant tous les entiers correspondant a la map
//endregion map var

//region power var
int power_y = 0; // Coordonee y du champignon (pouvoir)
int power_x = 0;// Coordonee x du champignon (pouvoir)
int power_direction = 1; // Direction dans laquelle le champignon doit avancer (-1 -> left / +1 -> right)
//endregion power var

//region gameplay var
char previousInputList[1]; // liste permettant de garde en memoire le dernier input du joueur
time_t mytime_start; // variable de temps au debut d'un input
char * time_str_start;// variable char correspondant a la variable de temps start
time_t mytime_end;// variable de temps a la fin d'un input
char * time_str_end;// variable char correspondant a la variable de temps end
//endregion gameplay var

//endregion VARIABLES


#define ENTER 10
#define ESCAPE 27
void init_curses()
{
    initscr();
    start_color();
    init_pair(56,COLOR_WHITE,COLOR_BLACK);
    init_pair(57,COLOR_BLACK,COLOR_WHITE);
    curs_set(0);
    noecho();
    keypad(stdscr,TRUE);
}

WINDOW **draw_menu(int start_col)
{

    WINDOW **items;
    items=(WINDOW **)malloc(4*sizeof(WINDOW *));

    items[0]=newwin(5,19,1,start_col);
    wbkgd(items[0],COLOR_PAIR(57));
    box(items[0],ACS_VLINE,ACS_HLINE);
    items[1]=subwin(items[0],1,17,2,start_col+1);
    wprintw(items[1],"LEVEL 1",1);
    items[2]=subwin(items[0],1,17,3,start_col+1);
    wprintw(items[2],"LEVEL 2",2);
    items[3]=subwin(items[0],1,17,4,start_col+1);
    wprintw(items[3],"INFO",3);



    wbkgd(items[1],COLOR_PAIR(56));
    wrefresh(items[0]);
    return items;
}
void delete_menu(WINDOW **items,int count)
{
    int i;
    for (i=0;i<count;i++)
        delwin(items[i]);
    free(items);
}
int scroll_menu(WINDOW **items,int count,int menu_start_col)
{
    int key;
    int selected=0;
    while (1) {
        key=getch();
        if (key==KEY_DOWN || key==KEY_UP) {
            wbkgd(items[selected+1],COLOR_PAIR(57));
            wnoutrefresh(items[selected+1]);
            if (key==KEY_DOWN) {
                selected=(selected+1) % count;
            } else {
                selected=(selected+count-1) % count;
            }
            wbkgd(items[selected+1],COLOR_PAIR(56));
            wnoutrefresh(items[selected+1]);
            doupdate();
        }else if (key==ESCAPE) {
            return -1;
        } else if (key==ENTER) {
            return selected;
        }
    }
}
int main()
{
    int key;
    WINDOW *messagebar;

    init_curses();

    bkgd(COLOR_PAIR(56));
    messagebar=subwin(stdscr,1,79,25,100);
    move(0,0);
    printw("Press ESC quits.");
    refresh();

    do {
        int selected_item;
        WINDOW **menu_items;

        werase(messagebar);
        wrefresh(messagebar);

        menu_items=draw_menu(100);
        selected_item=scroll_menu(menu_items,3,100);
        delete_menu(menu_items,4);
        if (selected_item<0)
            wprintw(messagebar,"You haven't selected any item.");
        else if(selected_item==0) {
            //wprintw(messagebar,"Let's play lvl 1",1);
            clear();
            game("../bin/map.txt");
        }
        else if(selected_item==1)
            wprintw(messagebar,"Let's play lvl 2",2);
        else if(selected_item==2)
            wprintw(messagebar,"Mario game created by Lucas Mouquet and Antoine Vandeplanque",3);



        touchwin(stdscr);
        key=getch();
        refresh();

    }
    while (key!=ESCAPE);

    delwin(messagebar);
    endwin();
    return 0;
}


//region main
int game (char *filename) {
    setlocale(LC_ALL, ""); // Permet d'afficher des caractères spéciaux dans le terminale
    //region variables
    WINDOW *win; // notre fenetre du terminale
    int input; // entree clavier du joueur
    int screen_width, screen_height; //taille de l'écran
    memcpy(intMapList, mapListInt, MAPLISTSIZE); // On copie la liste faite dans le map.c dans la liste déclarée pour la manipuler
    //endregion variables

    Setup(filename); //On démarre ncurses, le jeu
    startPlayerColor();
    getmaxyx(stdscr, screen_height, screen_width); //recupere la taille du terminal actuelle
    if (!(screen_height<SCREEN_HEIGHT || screen_width<SCREEN_WIDTH)){ // si en plein ecran
        startPlayerColor(); // on demarre l'affichage des couleurs du joueur
        // Tant que le joueur n'appui pas sur 'x'
        while ((input = getch()) != 'x'){
            resetCharacterDisplay(); // on efface les caractères aux positions precedentes
            getNextTypeBlocks(); // on recupere les next blocks types du joueur
            checkMobCollsion(); // on check si on touche un block ennemi

            if(player_y >= SCREEN_HEIGHT) { Shutdown(); exit(1); } // Si le joueur tombe dans le vide
            if(player_life <= 0) { Shutdown(); exit(1); } // Si le joueur n'a plus de vie

            if(input == 'd'){ // Si le joueur appui sur d
                previousInputList[0] = input; // On change l'input precedent en 'd'
                mytime_start = time(NULL); // on comence le compteur
                movePlayer(1); // on deplace le joueur avec une direction positive (vers la droite)
            }else if(input == 'q'){// Si le joueur appui sur q
                previousInputList[0] = input;// On change l'input precedent en 'q'
                mytime_start = time(NULL);// on comence le compteur
                movePlayer(-1); // on deplace le joueur avec une direction negative (vers la gauche)
            } else if(input == ' '){// Si le joueur appui sur espace
                mytime_end = time(NULL);// on recupere  le timer n°2
                double time_diff = difftime(mytime_end, mytime_start); // on recupere la difference en seconde entre le premier timer(start) el le deuxieme(end)
                if (time_diff >= 0.5) previousInputList[0] = input; // Si la difference en sec est inferieur ou egale à 0.5 sec, on change l'input en espace (' ')
                if (canJump(player_y, player_x, CHAR_DIMENSION)){ // Si le joueur peut sauter
                    jump(previousInputList[0]); // On fait sauter le joueur en passant en parametre le dernier input pour connaitre la direction du saut
                }
            } else{ // Sinon
                gravity(); // on applique la gravite (dans le else pour permettre un double dash)
                powerBoxMovements(); // on applique les movements du champignonss
            }
            displayCharacter(); // on affiche le caractère aux nouvelles positions
            UpdateScreen();// On actualise l'écran
        }
        // Si le joueur a appuye sur x
        Shutdown(); // On arrete ncurses
        exit(1); // on stoppe le programme

    }else{ //si l'écran n'est pas en plein écran
        Shutdown(); // On arrete ncurses
        printf("Mettre la fenetre en plein ecran svp !\n");
        return 0; // on stoppe le programme
    }

}
//endregion main

//region Functions

//region CHARACTER MOVEMENT
// Fonction pour deplacer le personnage
void movePlayer(int m_direction){
    if(m_direction == 1 && canGoRight(player_y, player_x, CHAR_DIMENSION) == 1)player_x++; // si ma direction est positive et que je peux aller vers la droite
    else if(m_direction == -1 && canGoLeft(player_y, player_x, CHAR_DIMENSION) == 1)player_x--;// si ma direction est negative et que je peux aller vers la gauche
}

// Fonction pour faire sauter le personnage
void jump(int previousInput){
    // BAS vers HAUT -> Pour i de 1 jusqu'a la hauteur de saut du personnage
    for (int i = 1; i <= JUMP_STRENGHT; ++i) {
        getNextTypeBlocks();// A chaque nouvelle position, on recupere les next types blocks
        resetCharacterDisplay(); // on reset le personnage aux
        if (canJump(player_y, player_x, CHAR_DIMENSION) == 1){// Si le joueur peut sauter
            player_y-=0.01*i; // on incremente la coordonnee y du personnage, a chaque iteration supplementaire, il sautera plus
            if(previousInput == 'd' && canJump(player_y, player_x, CHAR_DIMENSION) == 1 && canGoRight(player_y, player_x, CHAR_DIMENSION)) player_x++; // Si j'ai saute en avançant, que je peux sauter et que je peux aller vers la droite
            else if(previousInput == 'q' && canJump(player_y, player_x, CHAR_DIMENSION) == 1 && canGoLeft(player_y, player_x, CHAR_DIMENSION)) player_x--;// Si j'ai saute en reculant, que je peux sauter et que je peux aller vers la gauche
        }
        else if(nextTypeBlockUp == POWER_BOX_VALUE){ // Si le block au-dessus de ma tête est un power box
            blockMystereDisplay(); //on display et on arrête de monter
            break; // on stop la boucle for
        }else break; // Si on se prend un mur (on ne peut plus sauter), on arrête la boucle for

        displayCharacter();// On affiche le personnage aux nouvelles positions
        napms(FPS); // on laisse un delai pour l'animation
        UpdateScreen(); // Rafraichi l'ecran pour voirr les modifications
    }

    int canGo = 0; // variable qui sert a savoir si on peut avancer (devant ou derriere)
    int counter = 0; // variable qui sert a stopper l'avancement du personnage
    while (canDrop(player_y, player_x, CHAR_DIMENSION) == 1){
        counter += 1; // on increment le compteur
        resetCharacterDisplay();
        player_y++; // on increment la coordonnee y du joueur (il descend)
        if (counter >= 5) canGo = 1; // Si mon compteur est arrive a 5, je n'avance plus

        if (canGoRight(player_y, player_x, CHAR_DIMENSION) != 1 || canGoLeft(player_y, player_x, CHAR_DIMENSION) != 1) canGo = 1; // Si je peux avancer ou reuler
        if (previousInput == 'd' && canDrop(player_y, player_x, CHAR_DIMENSION) == canGoRight(player_y, player_x, CHAR_DIMENSION) == 1 && canGo == 0)  player_x++;
        else if (previousInput == 'q' && canDrop(player_y, player_x, CHAR_DIMENSION) == canGoLeft(player_y, player_x, CHAR_DIMENSION) == 1 && canGo == 0) player_x--;

        displayCharacter();
        napms(FPS);
        UpdateScreen();
    }
    getNextTypeBlocks();
    flushinp(); // Permet de ne pas prendre en compte les inputs pendant que le saut est effectué (evite de se deplacer a la fin du saut)
}

// Fonction qui applique la gravité au joueur
void gravity(){
    if (canDrop(player_y, player_x, CHAR_DIMENSION) == 1){ // Si le joueur peut tomber
        resetCharacterDisplay();
        player_y++; // on increment sa coordonnee y (il descend)
        displayCharacter();
        UpdateScreen();
    }
    flushinp();// Permet de ne pas prendre en compte les inputs pendant que le joueur tombe
}
//endregion CHARACTER MOVEMENT

//region POWER MOVEMENTS
void powerBoxMovements(){
    // Si le champignon est apparu et qu'il peut se deplacer soit a droite soit a gauche soit en bas (permet d'éviter des appels lorsque la champignon n'est pas sur la map ou qu'il ne peut pas bouger)
    if(power_x != 0 && (canDrop(power_y, power_x, 1) == 1 || (canGoRight(power_y, power_x, 1) == 1) || (canGoLeft(power_y, power_x, 1) == 1))){
        mvprintw(power_y, power_x, " "); // on efface le champignon aux positions precedentes
        if(canDrop(power_y, power_x, 1) == 1) power_y++; // Si le champignon peut tomber, on le fait descendre

        if((canGoRight(power_y, power_x, 1) == 1) && (power_direction == 1)) power_direction=1;// Si le champignon peut avancer, on set la direction en positive
        else{
            // On a trouve le mur de droite où le champignon ne peut plus passer
            power_direction = -1; // le champignon doit maintenant aller dans l'autre sens
        }
        if((canGoLeft(power_y, power_x, 1) == 1) && (power_direction == -1)) power_direction=-1;// Si le champignon peut avancer, on set la direction en negative
        else{
            // On a trouve le mur de droite où le champignon ne peut plus passer
            power_direction = 1;// le champignon doit maintenant aller dans l'autre sens
        }
        power_x += power_direction; // On déplace le champignon en fonction de la direction
        mvprintw(power_y, power_x, "o"); // On affiche le champignon aux nouvelles coordonnées
        napms(FPS);
    }

}
//endregion POWER MOVEMENTS

//region CHARACTER DISPLAY
// Fonction pou afficher le caractère aux coordonnees actuelles
void displayCharacter(){
    // Les offsets sont necessaires car notre personnages fait plusieurs caractères mais une seule coordonnées représente la position du joueur(le pied droit du joueur)
    mvaddch(player_y+Y_OFFSET_HEAD,player_x+X_OFFSET_HEAD,CHAR_HEAD); // Ajoute le bon caractère à la position x, y du joueur
    mvaddch(player_y+Y_OFFSET_BODY,player_x+X_OFFSET_BODY,CHAR_BODY);
    mvaddch(player_y+Y_OFFSET_ARM_L,player_x+X_OFFSET_ARM_L,CHAR_ARM_L);
    mvaddch(player_y+Y_OFFSET_ARM_R,player_x+X_OFFSET_ARM_R,CHAR_ARM_R);
    mvaddch(player_y+Y_OFFSET_FOOT_L,player_x+X_OFFSET_FOOT_L,CHAR_FOOT_L);
    mvaddch(player_y+Y_OFFSET_FOOT_L,player_x+X_OFFSET_FOOT_L+1, ' ');
    mvaddch(player_y+Y_OFFSET_FOOT_R,player_x+X_OFFSET_FOOT_R,CHAR_FOOT_R);
    // Si les coordonnees du joueur sont les mêmes que celles du champignons et qu'il n'a pas déjà un pouvoir
    if((player_life < 2) && ((player_x == power_x) && (player_y == power_y)) || ((player_x-1 == power_x) && (player_y == power_y)) || ((player_x-2 == power_x) && (player_y == power_y))){
        startPlayerStarColor(); // On demarre l'affichage du joueur quand il a un pouvoir
        player_life++;// On incremente sa vie
    }
}

//Fonction pour afficher les couleurs du personnage
void startPlayerColor(){
    start_color();
    //initialisation des couleurs pour chaque objet
    init_pair(50, COLOR_RED, COLOR_BLUE);
    attron(COLOR_PAIR(50));
}
//Fonction pour afficher les couleurs du personnage quand il a un pouvoir
void startPlayerStarColor(){
    power_x = 0;
    power_y = 0;
    start_color();
    //initialisation des couleurs pour chaque objet
    init_pair(60, COLOR_YELLOW, COLOR_BLUE);
    attron(COLOR_PAIR(60));
}
//Fonction pour effacer le personnage aux coordonnées précédentes
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
// Les canGo permettent de renvoyer 0 si on ne peut pas et 1 si on peut (avancer, reculer ...)
int canGoRight(int y, int x, int dimension){
    int somme = 0; // On initialise une somme
    // on parcourt pour tout le cote droit de notre personnage (du pied a la tête)
    for (int i = 0; i < dimension; ++i) somme += checkPossibleMove(y - i, x + 1); // on ajoute a somme l'entier correspondant a la position de notre joueur + 1 devant
    // Si le bloc suivant est un bloc vide, son entier est zéro, si c'est un bloc different de 0, ma somme ne sera pas nuls dans tous les cas
    if(somme != 0) return 0; // Si ma somme n'est pas nulle, c'est qu'on a un bloc qui nous empêche de passer
    else return 1;// Si ma somme est pas nulle, c'est qu'on a que des blocs traversable
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
// Foncion pour recuperer la valeur des blocs qui entourent le personnage
void getNextTypeBlocks(){
    // On recupere les valeurs pour chaque position
    nextTypeBlockUp = checkPossibleMove(player_y+Y_OFFSET_HEAD-1, player_x+X_OFFSET_HEAD); // Au dessus (position + niveau de la tête + prochain bloc)
    nextTypeBlockDown = new_max(checkPossibleMove(player_y+1, player_x+X_OFFSET_FOOT_L), checkPossibleMove(player_y+1, player_x+X_OFFSET_FOOT_R)); // Ici, peut importe si le bloc en dessous est vers le pied gauche ou droit, on recupere le max
    nextTypeBlockRight = checkPossibleMove(player_y, player_x+X_OFFSET_FOOT_R+1);
    nextTypeBlockLeft = checkPossibleMove(player_y, player_x+X_OFFSET_FOOT_L-1);
}
// Fonction pour connaitre si un bloc est un bloc traversable, pouvoir ou un bloc ennemi
int checkPossibleMove(int y, int x){
    int blockType = fromCoordToElementInMapList(y,x); // on recupere le int en focntion des coordonnées dans notre liste d'entier (on peut ici choisir la liste non modifié)
    if(blockType == CLOUDS_PAIR || blockType == SKY_PAIR) return 0; // Si c'est un bloc traversable
    else if(blockType == POWER_BOX_PAIR) return 2; // Si c'est un bloc pouvoir
    else if(blockType == MOB_PAIR) return 3; // Si c'est un bloc ennemi
    else return 1; // Sinon c'est un bloc non traversable
}

// Fonction pour vérifier que le joueur touche ou non un bloc ennemi et effectué l'action
void checkMobCollsion(){
    // Si les blocs suivants sont des bloc ennemi
    if(nextTypeBlockUp == MOB_VALUE || nextTypeBlockDown == MOB_VALUE || nextTypeBlockRight == MOB_VALUE || nextTypeBlockLeft == MOB_VALUE){
        player_life--; // le personnage perd une vie
        if(previousInputList[0] == 'q') player_x+=4; // Si on était en train de reculer, on se fait ejecter dans la direction inverse
        if(previousInputList[0] == 'd') player_x-=4;// Si on était en train de'avancer, on se fait ejecter dans la direction inverse
        startPlayerColor(); // On a perdu notre pouvoir, donc on redevient en couleur classique
    }
}
//endregion CALCULATE MOVEMENTS

//region DIVERS
// Fonction pour faire apparaître le champignon
void spawnPower(){
    // On fait spawn le champignon là où la tête a tapé
    power_y = player_y+Y_OFFSET_HEAD-3;
    power_x = player_x+X_OFFSET_HEAD;
    mvprintw(power_y, power_x, "o");
}

// Fonction pour changer la couleur du bloc mystere et actions
void blockMystereDisplay(){
    char *ch_t = "!"; // nouveau caractère du bloc mystere
    //initialisation des couleurs
    init_pair(10, COLOR_YELLOW, COLOR_BLACK);
    attron(COLOR_PAIR(10));
    //verifie si les blocs adjacents sont des blocs mystere pour les colorier
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
    int index = fromCoordToIndexInMapList(player_y+Y_OFFSET_HEAD-1, player_x+X_OFFSET_HEAD); // On recupere l'index de l'élément tapé pour qu'il ne soit plus tapable
    attroff(COLOR_PAIR(10));
    init_pair(20, COLOR_CYAN, COLOR_BLUE);
    attron(COLOR_PAIR(20));
    startPlayerColor();
    if(mapListInt[index] == 0) spawnPower(); // Si l'élément n'a jamais été tapé
    // On rend tous les blocs adjacents non tapable
    mapListInt[index] = 1;
    mapListInt[index-1] = 1;
    mapListInt[index+1] = 1;
    mapListInt[index-2] = 1;
    mapListInt[index+2] = 1;
}
//endregion DIVERS
//endregion Functions
