//
// Created by lucas on 18/11/2021.
//

#ifndef INNOVATION_MAP_H
#define INNOVATION_MAP_H

#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>

//region MAP PROPERTIES
#define GRASS     '#'
#define DIRT     '&'
#define CLOUDS     '-'
#define SKY     '.'
#define POWER_BOX  '?'
#define BLOCKS    '|'
#define BORDER    '$'
#define MOB    '/'

#define POWER_BOX_VALUE 2
#define AIR_BLOCK_VALUE 0
#define BLOCK_VALUE 4
#define MOB_VALUE 3

#define GRASS_PAIR     1
#define DIRT_PAIR     2
#define CLOUDS_PAIR     3
#define SKY_PAIR     4
#define POWER_BOX_PAIR  5
#define BLOCKS_PAIR    6
#define BORDER_PAIR    7
#define MOB_PAIR    8

#define MAPLISTSIZE 10659

void drawAction(int pair, char ch);
void asciiToIntAction(int listIndex, int n);
int getIntMapList();
int fromCoordToIndexInMapList(int y, int x);
void printIntMapList();
int fromCoordToElementInMapList(int y, int x);
void drawMap(char *filename);


#endif //INNOVATION_MAP_H
