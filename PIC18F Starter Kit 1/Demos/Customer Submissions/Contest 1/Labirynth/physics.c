#include "physics.h"
#include "graph.h"
#include "GenericTypeDefs.h"
#include "BMA150.h"
#include "levels.h"
#include "math.h"

signed short int g_devAccelerationX;
signed short int g_devAccelerationY;
signed short int g_devAccelerationZ;

signed short int g_devAccOffsetX = 0;
signed short int g_devAccOffsetY = 0;
signed short int g_devAccOffsetZ = 0;

float g_ballX, g_ballY, g_ballZ;
float g_ballVelX, g_ballVelY, g_ballVelZ;
float g_ballForceX, g_ballForceY, g_ballForceZ;
float g_ballMass;
unsigned char g_ballGth = 0;
unsigned char g_ballGtt = 0;
unsigned char g_colDirs = 0; //0000.x1 x2 y1 y2;

//rom unsigned char sin100_tab[32] = {0, 9, 19, 29, 38, 47, 56, 64, 71, 78, 84, 89, 93, 96, 98, 99, 99, 99, 97, 94, 90, 86, 80, 74, 67, 59, 51, 42, 33, 23, 14, 4};


float wallLineUp(unsigned char x, struct physWall w)
{
	return ((((float)x-(float)w.Ax)/((float)w.Bx-(float)w.Ax))*((float)w.Ay-(float)w.By) + (float)w.By);
}

float wallLineDown(unsigned char x, struct physWall w)
{
	return ((((float)x-(float)w.Ax)/((float)w.Bx-(float)w.Ax))*((float)w.By-(float)w.Ay) + (float)w.Ay);
}

void checkWall(struct physWall w)
{
	if (g_ballX >= w.Ax && g_ballX <= w.Bx && g_ballY >= w.Ay && g_ballY <= w.By)
	{
		if (g_ballY>=wallLineDown(g_ballX, w) && g_ballY<=wallLineUp(g_ballX, w))
		{
			g_ballX = w.Ax;
			g_ballVelX = -g_ballVelX*0.6f;
			g_colDirs |= 0x01;
		}
		else if (g_ballY<=wallLineDown(g_ballX, w) && g_ballY>=wallLineUp(g_ballX, w))
		{
			g_ballX = w.Bx;
			g_ballVelX = -g_ballVelX*0.6f;
			g_colDirs |= 0x02;
		}
		else if (g_ballY>wallLineDown(g_ballX, w) && g_ballY>wallLineUp(g_ballX, w))
		{
			g_ballY = w.By;
			g_ballVelY = -g_ballVelY*0.6f;
			g_colDirs |= 0x04;
		}
		else if (g_ballY<wallLineDown(g_ballX, w) && g_ballY<wallLineUp(g_ballX, w))
		{
			g_ballY = w.Ay;
			g_ballVelY = -g_ballVelY*0.6f;
			g_colDirs |= 0x08;
		}
	}
}

void ReadAccState()
{
	//do sprawdzenia i ew poprawy
	unsigned char lsb, msb;
	lsb = BMA150_ReadByte(BMA150_ACC_X_LSB);
	msb = BMA150_ReadByte(BMA150_ACC_X_MSB);
	
	g_devAccelerationX = (signed short int)(((unsigned short int)msb << 8) | (lsb));
	g_devAccelerationX /= 32;

	lsb = BMA150_ReadByte(BMA150_ACC_Y_LSB);
	msb = BMA150_ReadByte(BMA150_ACC_Y_MSB);
	
	g_devAccelerationY = (signed short int)(((unsigned short int)msb << 8) | (lsb));
	g_devAccelerationY /= 32;

	lsb = BMA150_ReadByte(BMA150_ACC_Z_LSB);
	msb = BMA150_ReadByte(BMA150_ACC_Z_MSB);
	
	/*g_devAccelerationZ = (signed short int)(((unsigned short int)msb << 8) | (lsb));
	g_devAccelerationZ /= 32;*/
}

void Step(float dtime, unsigned char animBall)
{
	float posChange, d;
	unsigned char i, j;
	static float time = 0.0f;

	time = time+dtime;

	ReadAccState();

	if (animBall)
	{
		g_ballForceX = (float)(g_devAccelerationY-g_devAccOffsetY)/2.0f;
		g_ballForceY = (float)(g_devAccelerationX-g_devAccOffsetX)/2.0f;
		
		posChange = g_ballX + ((float)g_ballVelX * dtime) + (((g_ballForceX) / g_ballMass) * dtime * dtime) / 2.0f;
	    g_ballVelX = (posChange - g_ballX) / dtime;
	    g_ballX = posChange;
	
		posChange = g_ballY + ((float)g_ballVelY * dtime) + (((g_ballForceY) / g_ballMass) * dtime * dtime) / 2.0f;
	    g_ballVelY = (posChange - g_ballY) / dtime;
	    g_ballY = posChange;
	
	/*	posChange = g_ballZ + (g_ballVelZ * dtime) + (((g_ballForceZ) / g_ballMass) * dtime * dtime) / 2.0f;
	    g_ballVelZ = (posChange - g_ballZ) / dtime;
	    g_ballZ = posChange;*/
	
		if (g_ballY <= 3)
		{
			g_ballY = 3;
			g_ballVelY = -g_ballVelY*0.6f;

			if ((g_colDirs & 0x08) == 0x08)
				g_ballGth = 1;
		}
	
		if (g_ballY >= 53)
		{
			g_ballY = 53;
			g_ballVelY = -g_ballVelY*0.6f;

			if ((g_colDirs & 0x04) == 0x04)
				g_ballGth = 1;
		}
	
		if (g_ballX <= 3)
		{
			g_ballX = 3;
			g_ballVelX = -g_ballVelX*0.6f;

			if ((g_colDirs & 0x02) == 0x02)
				g_ballGth = 1;
		}
	
		if (g_ballX >= 118)
		{
			g_ballX = 118;
			g_ballVelX = -g_ballVelX*0.6f;

			if ((g_colDirs & 0x01) == 0x01)
				g_ballGth = 1;
		}
	}

for (i = 0; i < g_usedWalls; i++)
	{
		for (j = 0; j < g_usedWallMovers; j++)
		{
			if (i == g_physWallMovers[j].wallId)
			{
				if (g_physWallMovers[j].orientation == HORIZONTAL)
				{
					if (g_walls[i].orientation == HORIZONTAL)
					{
						// < ------ >
						g_walls[i].c2_start = (float)g_physWallMovers[j].x + (float)g_physWallMovers[j].amp*sin( (float)g_physWallMovers[j].speedCoef * time);
						g_walls[i].c2_end = (float)g_physWallMovers[j].y + (float)g_physWallMovers[j].amp*sin( (float)g_physWallMovers[j].speedCoef * time);
					}
					else
					{
						// <|>
						g_walls[i].c = (float)g_physWallMovers[j].x + (float)g_physWallMovers[j].amp*sin( (float)g_physWallMovers[j].speedCoef * time);
					}
				}
				else
				{
					if (g_walls[i].orientation == HORIZONTAL)
					{
						// /\ ----- \/
						g_walls[i].c = (float)g_physWallMovers[j].y + (float)g_physWallMovers[j].amp*sin( (float)g_physWallMovers[j].speedCoef * time);
					}
					else
					{
						// /\ | \/
						g_walls[i].c2_start = (float)g_physWallMovers[j].x + (float)g_physWallMovers[j].amp*sin( (float)g_physWallMovers[j].speedCoef * time);
						g_walls[i].c2_end = (float)g_physWallMovers[j].y + (float)g_physWallMovers[j].amp*sin( (float)g_physWallMovers[j].speedCoef * time);
					}
				}

				GeneratePhysWall(&g_physWalls[i], g_walls[i]);
			}
		}
	}


	if (animBall)
	{
		// wall
		g_colDirs = 0;
		for (i = 0; i < g_usedWalls; i++)
		{
			checkWall(g_physWalls[i]);
		}
		
		if ((g_colDirs & 0x03) == 0x03 || (g_colDirs & 0x0C) == 0x0C)
			g_ballGth = 1;
	
		//holes
	
		for (i = 0; i < g_usedHoles; i++)
		{
			d = sqrt((float)(g_holes[i].x - g_ballX)*(float)(g_holes[i].x - g_ballX)+(float)(g_holes[i].y - g_ballY)*(float)(g_holes[i].y - g_ballY));
			if (d < 8)
			{
				g_ballVelX += (g_holes[i].x - g_ballX)*0.5f;
				g_ballVelY += (g_holes[i].y - g_ballY)*0.5f;
			}
	
			if (d<4)
			{
					g_ballX = g_holes[i].x;
					g_ballY = g_holes[i].y;
					g_ballGth = 1;
			}
		}
	
		//Trigger
		
		d = sqrt((float)(g_levelEndX - g_ballX)*(float)(g_levelEndX - g_ballX)+(float)(g_levelEndY - g_ballY)*(float)(g_levelEndY - g_ballY));
		if (d <= 5)
		{
			g_ballX = g_levelEndX;
			g_ballY = g_levelEndY;
			g_ballGtt = 1;
		}
	
		g_ballVelX *= 0.985f;
		g_ballVelY *= 0.985f;
	}

	return;
}

void InitPhysics()
{
	g_ballX = 64; g_ballY = 32; g_ballZ = 0;
	g_ballVelX = g_ballVelY = g_ballVelZ = 0;
	g_ballForceX = g_ballForceY = g_ballForceZ = 0;
	g_ballMass = 0.5f;		
}



