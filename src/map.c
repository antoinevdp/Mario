//
// Created by lucas on 18/11/2021.
//

#include "map.h"

int intMapList[MAPLISTSIZE]; // liste contenant les entiers correspondant a la map ascii
int listIndex; // index actuel de la liste
//endregion MAP PROPERTIES

//region Functions
//region Action Functions
void drawAction(int pair, char ch){
    attron(COLOR_PAIR(pair));
    printw("%c", ch);
}
void asciiToIntAction(int listIndex, int n){
    intMapList[listIndex] = n; //on change l'element de la liste avec un entier
}
//endregion Action Functions

int getIntMapList(){
    return *intMapList; //retourne la liste map des entiers
}

void printIntMapList(){
    for(int i=0; i<MAPLISTSIZE; i++){
        printf("%d", intMapList[i]);
    }
}

//fonction pour recuperer l'element correspondant aux coordonnées
int fromCoordToElementInMapList(int y, int x){
    //Tous les 209 elmts, y++
    //Donc position x=0 et y=1 -> elmt 210
    return intMapList[(y*209) + x+1];
}
int fromCoordToIndexInMapList(int y, int x){
    //Tous les 209 elmts, y++
    //Donc position x=0 et y=1 -> elmt 210
    return ((y*209) + x+1);
}

//region MAP
void drawMap(char *filename){
    //Ouverture du fichier
    FILE *src;
    src = fopen (filename, "r");
    if(src == NULL){ // on verifie que le fichier existe
        perror("Ouverture fail");
        exit(1);
    }
    //verification de la disponibilité des couleurs du terminal
    if (has_colors() == FALSE) {
        endwin();
        printf("les couleurs ne sont pas supporte par votre terminal\n");
        exit(1);
    }
    //start color
    start_color();
    //initialisation des couleurs pour chaque objet
    init_pair(GRASS_PAIR, COLOR_YELLOW, COLOR_GREEN);
    init_pair(DIRT_PAIR, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(CLOUDS_PAIR, COLOR_WHITE, COLOR_WHITE);
    init_pair(SKY_PAIR, COLOR_CYAN, COLOR_BLUE);
    init_pair(BLOCKS_PAIR, COLOR_BLACK, COLOR_RED);
    init_pair(POWER_BOX_PAIR, COLOR_BLACK, COLOR_YELLOW);
    init_pair(BORDER_PAIR, COLOR_BLACK, COLOR_BLACK);
    init_pair(MOB_PAIR, COLOR_WHITE, COLOR_BLACK);

    char ch; //caractere actuel de la map en ascii
    // caractere par caractere
    while ((ch = fgetc(src)) != EOF)
    {
        listIndex++; // on incremente index
        //switcher

        //region SWITCHER
        switch (ch) {
            case GRASS:
                drawAction(GRASS_PAIR, ch); // On dessine sur la map
                asciiToIntAction(listIndex, GRASS_PAIR); // On ajoute le nombre correspondant au caractère dans la liste
                break;
            case DIRT:
                drawAction(DIRT_PAIR, ch);
                asciiToIntAction(listIndex, DIRT_PAIR);
                break;
            case CLOUDS:
                drawAction(CLOUDS_PAIR, *" "); // On ne veut pas dessiner le caractere sur la map
                asciiToIntAction(listIndex, CLOUDS_PAIR);
                break;
            case SKY:
                drawAction(SKY_PAIR, *" ");
                asciiToIntAction(listIndex, SKY_PAIR);
                break;
            case BLOCKS:
                drawAction(BLOCKS_PAIR, ch);
                asciiToIntAction(listIndex, BLOCKS_PAIR);
                break;
            case POWER_BOX:
                drawAction(POWER_BOX_PAIR, ch);
                asciiToIntAction(listIndex, POWER_BOX_PAIR);
                break;
            case BORDER:
                asciiToIntAction(listIndex, BORDER_PAIR);
                drawAction(BORDER_PAIR, ch);
                break;
            case MOB:
                asciiToIntAction(listIndex, MOB_PAIR);
                drawAction(MOB_PAIR, ch);
                break;
            default:
                printw("%c", ch); // Si l'element n'est pas reconnu, on l'affiche tel quel
                break;
        }
        //endregion SWITCHER

    }
}
//endregion MAP

//endregion Functions