#ifndef API_H
#define API_H

#include "console.h"

void Cls(Console *console, int c);
void PxSet(Console *console, int x, int y, int c);
int PxGet(Console *console, int x, int y);
void Line(Console *console, int x1, int y1, int x2, int y2, int c);
void Text(Console *console, int x, int y, char *s, int c);
void Rect(Console *console, int x, int y, int w, int h, int c);
void RectF(Console *console, int x, int y, int w, int h, int c);
void Circ(Console *console, int x, int y, int r, int c);
void CircF(Console *console, int x, int y, int r, int c);
void Spr(Console *console, int id, int x, int y, int w, int h);
void GetKeys(Console *console);
void Mouse(Console *console);

#endif
