#ifndef _LEVELS_H
#define _LEVELS_H

#include "physics.h"

#define MAX_WALLS 14
#define MAX_HOLES 23
#define LEVELS 6
#define HORIZONTAL 0
#define VERTICAL   1
#define MAX_WALLS_MOVERS 3

#ifndef LEVELS_C
    extern struct graphWall g_walls[MAX_WALLS];
    extern struct physWall g_physWalls[MAX_WALLS];
    extern struct Hole g_holes[MAX_HOLES];
    extern struct physWallMover g_physWallMovers[MAX_WALLS_MOVERS];
    extern unsigned char g_usedWalls;
    extern unsigned char g_usedHoles;
    extern unsigned char g_usedWallMovers;
    extern unsigned char g_level;
    extern unsigned char g_levelEndX, g_levelEndY;
    extern unsigned char g_endGame;
#endif

void level0();

#endif
