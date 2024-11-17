
#include "api.h"
#include "console.h"
#include "globals.h"
#include <raylib.h>
#include <stdint.h>

void Cls(Conosole *console, int c) {
	for(int i = 0; i < (SCREEN_WIDTH*SCREEN_HEIGHT)/2; i++) {
		Poke(&console->ram, i, (c | (c << 4)));
	}
}

void PxSet(Conosole *console, int x, int y, int c) {
	if(x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
		return;
	}

	int pos = (x/2) + (y * (SCREEN_WIDTH/2));
	uint8_t colors = Peek(&console->ram, pos);
	if(x % 2 == 0) {
		Poke(&console->ram, pos, (c<<4) | (colors&0xF));
	} else {
		Poke(&console->ram, pos, (colors&0xF0) | c);
	}
}

int PxGet(Conosole *console, int x, int y) {
	if(x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
		return 0;
	}

	int pos = (x/2) + (y * (SCREEN_WIDTH/2));
	uint8_t colors = Peek(&console->ram, pos);
	if(x % 2 == 0) {
		return colors >> 4;
	} else {
		return colors & 0xF;
	}
}

