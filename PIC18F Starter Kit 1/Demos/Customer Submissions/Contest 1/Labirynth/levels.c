
#define LEVELS_C

#include "physics.h"
#include "graph.h"
#include "levels.h"

struct graphWall g_walls[MAX_WALLS];
struct physWall g_physWalls[MAX_WALLS];
struct physWallMover g_physWallMovers[MAX_WALLS_MOVERS];
struct Hole g_holes[MAX_HOLES];
unsigned char g_usedWalls;
unsigned char g_usedWallMovers;
unsigned char g_usedHoles;
unsigned char g_level;
unsigned char g_endGame = 0;

unsigned char g_levelEndX, g_levelEndY;

void _generatePhysWalls();
void level1();
void level2();
void level3();
void level4();
void level5();

void _generatePhysWalls()
{
	unsigned char i;

	for (i = 0; i < g_usedWalls; i++)
	{
		GeneratePhysWall(&(g_physWalls[i]), g_walls[i]);
	}
}

void resetBall(unsigned char x, unsigned char y)
{
	g_ballX = x;
	g_ballY = y;
	g_ballZ = 0;
	g_ballVelX = g_ballVelY = g_ballVelZ = 0;
	g_ballForceX = g_ballForceY = g_ballForceZ = 0;
}

void level0()
{
	//0
	g_usedWalls = 2;
	g_usedHoles = 0;
	g_usedWallMovers = 0;

	g_levelEndX = 10; g_levelEndY = 7;

	resetBall(110,52);

	g_walls[0].orientation = HORIZONTAL;
	g_walls[0].c = 20;
	g_walls[0].c2_start = 0;
	g_walls[0].c2_end = 70;	

	g_walls[1].orientation = HORIZONTAL;
	g_walls[1].c = 40;
	g_walls[1].c2_start = 40;
	g_walls[1].c2_end = 128;
}

void level1()
{
	// 1
	g_usedWalls = 2;
	g_usedHoles = 6;
	g_usedWallMovers = 0;

	g_levelEndX = 7; g_levelEndY = 7;

	resetBall(110,49);

	g_walls[0].orientation = HORIZONTAL;
	g_walls[0].c = 31;
	g_walls[0].c2_start = 40;
	g_walls[0].c2_end = 87;

	g_walls[1].orientation = VERTICAL;
	g_walls[1].c = 61;
	g_walls[1].c2_start = 26;
	g_walls[1].c2_end = 38;

	g_holes[0].x = 58;
	g_holes[0].y = 7;

	g_holes[1].x = 58;
	g_holes[1].y = 49;

	g_holes[2].x = 110;
	g_holes[2].y = 28;

	g_holes[3].x = 7;
	g_holes[3].y = 28;

	g_holes[4].x = 110;
	g_holes[4].y = 7;

	g_holes[5].x = 7;
	g_holes[5].y = 49;
}

void level2()
{
	// 2
	g_usedWalls = 10;
	g_usedHoles = 3;
	g_usedWallMovers = 0;

	g_levelEndX = 7; g_levelEndY = 49;

	resetBall(110,7);

	g_walls[0].orientation = HORIZONTAL;
	g_walls[0].c = 20;
	g_walls[0].c2_start = 30;
	g_walls[0].c2_end = 127;

	g_walls[1].orientation = HORIZONTAL;
	g_walls[1].c = 40;
	g_walls[1].c2_start = 0;
	g_walls[1].c2_end = 97;

	g_walls[2].orientation = HORIZONTAL;
	g_walls[2].c = 18;
	g_walls[2].c2_start = 77;
	g_walls[2].c2_end = 97;

	g_walls[3].orientation = HORIZONTAL;
	g_walls[3].c = 16;
	g_walls[3].c2_start = 77;
	g_walls[3].c2_end = 97;

	g_walls[4].orientation = HORIZONTAL;
	g_walls[4].c = 2;
	g_walls[4].c2_start = 47;
	g_walls[4].c2_end = 67;
	
	g_walls[5].orientation = HORIZONTAL;
	g_walls[5].c = 4;
	g_walls[5].c2_start = 47;
	g_walls[5].c2_end = 67;

	g_walls[6].orientation = HORIZONTAL;
	g_walls[6].c = 38;
	g_walls[6].c2_start = 57;
	g_walls[6].c2_end = 77;
	
	g_walls[7].orientation = HORIZONTAL;
	g_walls[7].c = 36;
	g_walls[7].c2_start = 57;
	g_walls[7].c2_end = 77;

	g_walls[8].orientation = HORIZONTAL;
	g_walls[8].c = 22;
	g_walls[8].c2_start = 107;
	g_walls[8].c2_end = 127;
	
	g_walls[9].orientation = HORIZONTAL;
	g_walls[9].c = 24;
	g_walls[9].c2_start = 107;
	g_walls[9].c2_end = 127;

	g_holes[0].x = 7;
	g_holes[0].y = 7;

	g_holes[1].x = 110;
	g_holes[1].y = 49;

	g_holes[2].x = 7;
	g_holes[2].y = 25;
}

void level3()
{
	// 2
	g_usedWalls = 0;
	g_usedHoles = 22;
	g_usedWallMovers = 0;

	g_levelEndX = 22; g_levelEndY = 49;

	resetBall(110,7);

	//left
	g_holes[0].x = 7;
	g_holes[0].y = 6;

	g_holes[1].x = 7;
	g_holes[1].y = 17;

	g_holes[2].x = 7;
	g_holes[2].y = 28;

	g_holes[3].x = 7;
	g_holes[3].y = 39;

	g_holes[4].x = 7;
	g_holes[4].y = 50;

	//up
	g_holes[5].x = 22;
	g_holes[5].y = 6;

	g_holes[6].x = 37;
	g_holes[6].y = 6;

	g_holes[7].x = 52;
	g_holes[7].y = 6;

	g_holes[8].x = 67;
	g_holes[8].y = 6;

	g_holes[9].x = 82;
	g_holes[9].y = 6;

	g_holes[10].x = 98;
	g_holes[10].y = 6;

	//right
	//g_holes[11].x = 111;
	//g_holes[11].y = 17;

	g_holes[11].x = 111;
	g_holes[11].y = 28;

	g_holes[12].x = 111;
	g_holes[12].y = 39;

	g_holes[13].x = 111;
	g_holes[13].y = 50;

	//down
	g_holes[14].x = 37;
	g_holes[14].y = 50;

	g_holes[15].x = 52;
	g_holes[15].y = 50;

	g_holes[16].x = 67;
	g_holes[16].y = 50;

	g_holes[17].x = 82;
	g_holes[17].y = 50;

	g_holes[18].x = 98;
	g_holes[18].y = 50;

	//right wall
	g_holes[19].x = 67;
	g_holes[19].y = 17;

	g_holes[20].x = 67;
	g_holes[20].y = 28;

	g_holes[21].x = 37;
	g_holes[21].y = 39;
}

void level4()
{
	//0
	g_usedWalls = 3;
	g_usedHoles = 0;
	g_usedWallMovers = 1;

	g_levelEndX = 10; g_levelEndY = 7;

	resetBall(110,52);

	g_walls[0].orientation = HORIZONTAL;
	g_walls[0].c = 20;
	g_walls[0].c2_start = 40;
	g_walls[0].c2_end = 128;	

	g_walls[1].orientation = HORIZONTAL;
	g_walls[1].c = 40;
	g_walls[1].c2_start = 40;
	g_walls[1].c2_end = 128;

	g_walls[2].orientation = HORIZONTAL;
	g_walls[2].c = 30;
	g_walls[2].c2_start = 0;
	g_walls[2].c2_end = 80;

	g_physWallMovers[0].y = 31;
	g_physWallMovers[0].wallId = 2;
	g_physWallMovers[0].orientation = VERTICAL;
	g_physWallMovers[0].amp = 8;
	g_physWallMovers[0].speedCoef = 1;	
}

void level5()
{
	//0
	g_usedWalls = 6;
	g_usedHoles = 2;
	g_usedWallMovers = 3;

	g_levelEndX = 10; g_levelEndY = 52;

	resetBall(110,52);

	g_walls[0].orientation = VERTICAL;
	g_walls[0].c = 30;
	g_walls[0].c2_start = 32;
	g_walls[0].c2_end = 63;	

	g_walls[1].orientation = VERTICAL;
	g_walls[1].c = 62;
	g_walls[1].c2_start = 0;
	g_walls[1].c2_end = 32;

	g_walls[2].orientation = VERTICAL;
	g_walls[2].c = 94;
	g_walls[2].c2_start = 32;
	g_walls[2].c2_end = 64;

	//moving walls
	g_walls[3].orientation = VERTICAL;
	g_walls[3].c = 30;
	g_walls[3].c2_start = 13;
	g_walls[3].c2_end = 21;	

	g_walls[4].orientation = VERTICAL;
	g_walls[4].c = 62;
	g_walls[4].c2_start = 44;
	g_walls[4].c2_end = 52;

	g_walls[5].orientation = VERTICAL;
	g_walls[5].c = 94;
	g_walls[5].c2_start = 13;
	g_walls[5].c2_end = 21;

	g_physWallMovers[0].x = 13;
	g_physWallMovers[0].y = 21;
	g_physWallMovers[0].wallId = 3;
	g_physWallMovers[0].orientation = VERTICAL;
	g_physWallMovers[0].amp = 9;
	g_physWallMovers[0].speedCoef = 3;	

	g_physWallMovers[1].x = 13;
	g_physWallMovers[1].y = 21;
	g_physWallMovers[1].wallId = 5;
	g_physWallMovers[1].orientation = VERTICAL;
	g_physWallMovers[1].amp = 9;
	g_physWallMovers[1].speedCoef = 1;

	g_physWallMovers[2].x = 43;
	g_physWallMovers[2].y = 51;
	g_physWallMovers[2].wallId = 4;
	g_physWallMovers[2].orientation = VERTICAL;
	g_physWallMovers[2].amp = 9;
	g_physWallMovers[2].speedCoef = 2;

	g_holes[0].x = 44;
	g_holes[0].y = 28;

	g_holes[1].x = 76;
	g_holes[1].y = 28;
}
/*void level1() { return; }
void level2() { return; }
void level3() { return; }
void level4() { return; }
void level5() { return; }*/
void loadLevel(unsigned char l)
{
	switch(l)
	{
		case 0:
			level0();
			break;

		case 1:
			level1();
			break;

		case 2:
			level2();
			break;

		case 3:
			level3();
			break;

		case 4:
			level4();
			break;

		case 5:
			level5();
			break;

		case LEVELS:
			g_endGame = 1;
			break;
	}

	_generatePhysWalls();
}

