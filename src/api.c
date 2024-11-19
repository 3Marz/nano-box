
#include "api.h"
#include "console.h"
#include "globals.h"
#include <string.h>
#include <math.h>
#include <stdint.h>

void Cls(Console *console, int c) {
	for(int i = 0; i < (SCREEN_WIDTH*SCREEN_HEIGHT)/2; i++) {
		Poke(&console->ram, i, (c | (c << 4)));
	}
}

void PxSet(Console *console, int x, int y, int c) {
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

int PxGet(Console *console, int x, int y) {
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

void Line(Console *console, int x1, int y1, int x2, int y2, int c) {
	float dx = x2 - x1;
	float dy = y2 - y1;
	float step = 0;

	if (fabs(dx) > fabs(dy)) {
		step = fabs(dx);
	} else {
		step = fabs(dy);
	}

	float x_incr = dx / step;
	float y_incr = dy / step;

	float x = x1;
	float y = y1;
	for(int i = 0; i < floor(step); i++) {
		PxSet(console, x, y, c);
		x += x_incr;
		y += y_incr;
	}
}

void Rect(Console *console, int x, int y, int w, int h, int c) {
	Line(console, x, y, x+w-1, y, c);
	Line(console, x, y+h-1, x+w-1, y+h, c);
	Line(console, x, y, x, y+h-1, c);
	Line(console, x+w-1, y, x+w, y+h, c);
}
void RectF(Console *console, int x, int y, int w, int h, int c) {
	for(int xi = x; xi < x+w; xi++) {
		for(int yi = y; yi < y+h; yi++) {
			PxSet(console, xi, yi, c);
		}
	}
}

void CircF(Console *console, int x, int y, int r, int c) {
	for(int xi = 0; xi <= r*2; xi++) {
		for(int yi = 0; yi <= r*2; yi++) {
			float dx = r-xi;
			float dy = r-yi;

			float dist = sqrtf((dx*dx) + (dy*dy));

			if (dist-r > 1) {
				continue;
			}

			if (r/dist < 0.98) {
				continue;
			}
			
			PxSet(console, (xi+x)-(r), (yi+y)-(r), c);
		}
	}
}
void Circ(Console *console, int x, int y, int r, int c) {
	int ax = r;
	int ay = 0;
	int b = 1-ax;
	while(ay <= ax) {
		PxSet(console, x+ax, y+ay, c);
		PxSet(console, x-ax, y+ay, c);
		PxSet(console, x+ax, y-ay, c);
		PxSet(console, x-ax, y-ay, c);
		PxSet(console, x+ay, y+ax, c);
		PxSet(console, x-ay, y+ax, c);
		PxSet(console, x+ay, y-ax, c);
		PxSet(console, x-ay, y-ax, c);
		
		ay++;
		if(b < 0) {
			b += 2*ay + 1;
		} else {
			ax--;
			b += 2*(ay-ax) + 1;
		}
	}
}

void Text(Console *console, int x, int y, char *s, int c) {
	int xoff = 0;
	int yoff = y;
	int char_height = 6;
	int char_width = 5;
	for(int i = 0; i < strlen(s); i++) {

		// TODO support more escape sequences.
		if(s[i] == '\n') {
			xoff = 0;
			yoff += char_height;
			i++;
		}

		int addr = 0x3030+(char_height*(s[i]-32));

		for(int j = 0; j < char_height; j++) {
			int hex = Peek(&console->ram, addr+j);
			for(int k = 0; k < 8; k++) {
				if(hex & (1 << (7-k))) { // Thanks Codium!
					PxSet(console, x+k+xoff, yoff+j, c);
				}
			}
		}
		xoff += char_width;
	}
}

void Spr(Console *console, int id, int x, int y, int w, int h) {
	int id_addr = 0x326A+(id*32);
	for(int j = 0; j < h*8; j++) {
		for (int i = 0; i < w*4; i++) {
			int colors = Peek(&console->ram, id_addr+i+(j*4));
			PxSet(console, x+(i*2), y+j, colors>>4);
			PxSet(console, x+(i*2)+1, y+j, colors&0xf);
		}
	}
}
