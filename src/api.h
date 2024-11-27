#ifndef API_H
#define API_H

#include "console.h"
#include "ram.h"

void Cls(Ram *ram, int c);
void PxSet(Ram *ram, int x, int y, int c);
int PxGet(Ram *ram, int x, int y);
void Line(Ram *ram, int x1, int y1, int x2, int y2, int c);
void Text(Ram *ram, int x, int y, char *s, int c);
void Rect(Ram *ram, int x, int y, int w, int h, int c);
void RectF(Ram *ram, int x, int y, int w, int h, int c);
void Circ(Ram *ram, int x, int y, int r, int c);
void CircF(Ram *ram, int x, int y, int r, int c);
void Spr(Ram *ram, int id, int x, int y, int colorkey, int w, int h);
void GetKeys(Ram *ram);
void Mouse(Ram *ram);
void BtnP(Ram *ram);
void Btn(Ram *ram);

#endif
