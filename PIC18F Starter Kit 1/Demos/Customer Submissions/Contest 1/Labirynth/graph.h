#ifndef GRAPH_H
#define GRAPH_H

#include "physics.h"

#pragma idata graph_buffer
extern unsigned char g_graphBuffer[128][8];
#pragma idata

extern unsigned char * g_graphBufPtr;// = &g_graphBuffer[0];

#define SetPx(x,y) g_graphBufPtr[x+(y >> 3)*128] |= (0x01 << (y&0x07))

struct graphWall {
	unsigned char c;
	unsigned char c2_start, c2_end;
	unsigned char orientation; // 0 - horizontal, 1 - vertical
};

struct Hole {
	unsigned char x,y;
};

void DrawBall(unsigned char x, unsigned char y, unsigned char p1);
void DrawScene();
void Fill(unsigned char clr);

void GeneratePhysWall(struct physWall *w, struct graphWall g);

#endif
