//
// Created by lucas on 18/11/2021.
//

#ifndef INNOVATION_PLAYER_H
#define INNOVATION_PLAYER_H

#include <ncurses.h>

//region PLAYER PROPERTIES
#define SPAWN_POINT_X   66
#define SPAWN_POINT_Y   26
#define JUMP_STRENGHT   5
#define CHAR_DIMENSION   3

/*
 *  ‿ -> bloc gris
 *
 *
 *
 *
 */

#define CHAR_HEAD *"O"
#define CHAR_BODY *"°"
#define CHAR_ARM_L '/'
#define CHAR_ARM_R '\\'
#define CHAR_FOOT_L '/'
#define CHAR_FOOT_R '\\'

/*
 *     O
 *    /|\
 *    / \
 */
#define Y_OFFSET_HEAD -2
#define X_OFFSET_HEAD -1
#define Y_OFFSET_BODY -1
#define X_OFFSET_BODY -1
#define Y_OFFSET_ARM_L -1
#define X_OFFSET_ARM_L -2
#define Y_OFFSET_ARM_R -1
#define X_OFFSET_ARM_R 0
#define Y_OFFSET_FOOT_L 0
#define X_OFFSET_FOOT_L -2
#define Y_OFFSET_FOOT_R 0
#define X_OFFSET_FOOT_R 0
//endregion PLAYER PROPERTIES

//region PLAYER MOVEMENTS

//endregion PLAYER MOVEMENTS

#endif //INNOVATION_PLAYER_H
