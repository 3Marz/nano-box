#ifndef API_H
#define API_H

#include "console.h"
#include "raylib.h"

void Cls(Conosole *console, int c);
void PxSet(Conosole *console, int x, int y, int c);
int PxGet(Conosole *console, int x, int y);

#endif
