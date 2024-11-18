#ifndef API_H
#define API_H

#include "console.h"
#include "raylib.h"

void Cls(Console *console, int c);
void PxSet(Console *console, int x, int y, int c);
int PxGet(Console *console, int x, int y);

#endif
