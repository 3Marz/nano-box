#ifndef API_H
#define API_H

#include "console.h"

void Cls(Console *console, int c);
void PxSet(Console *console, int x, int y, int c);
int PxGet(Console *console, int x, int y);
void Line(Console *console, int x1, int y1, int x2, int y2, int c);

#endif
