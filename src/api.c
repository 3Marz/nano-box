
#include "api.h"
#include "console.h"
#include "globals.h"
#include "ram.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

void Cls(Ram *ram, int c) {
	for(int i = 0; i < (SCREEN_WIDTH*SCREEN_HEIGHT)/2; i++) {
		Poke(ram, i, (c | (c << 4)));
	}
}

void PxSet(Ram *ram, int x, int y, int c) {
	if(x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
		return;
	}

	int pos = (x/2) + (y * (SCREEN_WIDTH/2));
	uint8_t colors = Peek(ram, pos);
	if(x % 2 == 0) {
		Poke(ram, pos, (c<<4) | (colors&0xF));
	} else {
		Poke(ram, pos, (colors&0xF0) | c);
	}
}

int PxGet(Ram *ram, int x, int y) {
	if(x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
		return 0;
	}

	int pos = (x/2) + (y * (SCREEN_WIDTH/2));
	uint8_t colors = Peek(ram, pos);
	if(x % 2 == 0) {
		return colors >> 4;
	} else {
		return colors & 0xF;
	}
}

void Line(Ram *ram, int x1, int y1, int x2, int y2, int c) {
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
		PxSet(ram, x, y, c);
		x += x_incr;
		y += y_incr;
	}
}

void Rect(Ram *ram, int x, int y, int w, int h, int c) {
	Line(ram, x, y, x+w-1, y, c);
	Line(ram, x, y+h-1, x+w-1, y+h, c);
	Line(ram, x, y, x, y+h-1, c);
	Line(ram, x+w-1, y, x+w, y+h, c);
}
void RectF(Ram *ram, int x, int y, int w, int h, int c) {
	for(int xi = x; xi < x+w; xi++) {
		for(int yi = y; yi < y+h; yi++) {
			PxSet(ram, xi, yi, c);
		}
	}
}

void CircF(Ram *ram, int x, int y, int r, int c) {
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
			
			PxSet(ram, (xi+x)-(r), (yi+y)-(r), c);
		}
	}
}
void Circ(Ram *ram, int x, int y, int r, int c) {
	int ax = r;
	int ay = 0;
	int b = 1-ax;
	while(ay <= ax) {
		PxSet(ram, x+ax, y+ay, c);
		PxSet(ram, x-ax, y+ay, c);
		PxSet(ram, x+ax, y-ay, c);
		PxSet(ram, x-ax, y-ay, c);
		PxSet(ram, x+ay, y+ax, c);
		PxSet(ram, x-ay, y+ax, c);
		PxSet(ram, x+ay, y-ax, c);
		PxSet(ram, x-ay, y-ax, c);
		
		ay++;
		if(b < 0) {
			b += 2*ay + 1;
		} else {
			ax--;
			b += 2*(ay-ax) + 1;
		}
	}
}

void Text(Ram *ram, int x, int y, char *s, int c) {
	int xoff = 0;
	int yoff = y;
	int char_height = 6;
	int char_width = 5;
	int color = c;
	for(int i = 0; i < strlen(s); i++) {

		// TODO support more escape sequences.
		if(s[i] == '\n') {
			xoff = 0;
			yoff += char_height;
			i++;
		}
		if(s[i] == '\x01')  { color = 0; i++; }
		if(s[i] == '\x02')  { color = 1; i++; }
		if(s[i] == '\x03')  { color = 2; i++; }
		if(s[i] == '\x04')  { color = 3; i++; }
		if(s[i] == '\x05')  { color = 4; i++; }
		if(s[i] == '\x06')  { color = 5; i++; }
		if(s[i] == '\x07')  { color = 6; i++; }
		if(s[i] == '\x08')  { color = 7; i++; }
		if(s[i] == '\x09')  { color = 8; i++; }
		if(s[i] == '\x10')  { color = 9; i++; }
		if(s[i] == '\x11') { color = 10; i++; }
		if(s[i] == '\x12') { color = 11; i++; }
		if(s[i] == '\x13') { color = 12; i++; }
		if(s[i] == '\x14') { color = 13; i++; }
		if(s[i] == '\x15') { color = 14; i++; }
		if(s[i] == '\x16') { color = 15; i++; }

		int addr = 0x3030+(char_height*(s[i]-32));

		for(int j = 0; j < char_height; j++) {
			int hex = Peek(ram, addr+j);
			for(int k = 0; k < 8; k++) {
				if(hex & (1 << (7-k))) { // Thanks Codium!
					PxSet(ram, (x+k+xoff)-3, yoff+j, color);
				}
			}
		}
		xoff += char_width;
	}
}

void Spr(Ram *ram, int id, int x, int y, int colorkey, int w, int h) {
	int id_addr = 0x326A+(id*32);
	for(int j = 0; j < h*8; j++) {
		for (int i = 0; i < w*4; i++) {
			int colors = Peek(ram, id_addr+i+(j*4));
			if((colors>>4) != colorkey) {
				PxSet(ram, x+(i*2), y+j, colors>>4);
			}
			if ((colors&0xf) != colorkey) {
				PxSet(ram, x+(i*2)+1, y+j, colors&0xf);
			}
		}
	}
}

void Mouse(Console *console) {
	int x, y, btn;
	tigrMouse(console->tscreen, &x, &y, &btn);
	Poke(&console->ram, 0x526A, x);
	Poke(&console->ram, 0x526B, y);
	Poke(&console->ram, 0x526C, btn);
}

void GetKeys(Console *console) {
	Poke(&console->ram, 0x526D, tigrReadChar(console->tscreen));
}

void Btn(Console *console) {
	int binArray[8] = {0,0,0,0,0,0,0,0};
	int hex = Peek(&console->ram, 0x526F);
	for(int i = 0; i < 8; i++) {
		if(hex & (1 << (7-i))) {
			binArray[i] = 1;
		}
	}
	if(tigrKeyHeld(console->tscreen, TK_UP))    { binArray[0] = 1; } else { binArray[0] = 0; }
	if(tigrKeyHeld(console->tscreen, TK_DOWN))  { binArray[1] = 1; } else { binArray[1] = 0; }
	if(tigrKeyHeld(console->tscreen, TK_RIGHT)) { binArray[2] = 1; } else { binArray[2] = 0; }
	if(tigrKeyHeld(console->tscreen, TK_LEFT))  { binArray[3] = 1; } else { binArray[3] = 0; }
	if(tigrKeyHeld(console->tscreen, 'Z'))      { binArray[4] = 1; } else { binArray[4] = 0; }
	if(tigrKeyHeld(console->tscreen, 'X'))      { binArray[5] = 1; } else { binArray[5] = 0; }
	int bin = 0;
	int mult = 1;
	for(int i = 7; i >= 0; i--) {
		bin += binArray[i]*mult;
		mult *= 2;
	}
	Poke(&console->ram, 0x526F, bin);
}
void BtnP(Console *console) {
	int binArray[8] = {0,0,0,0,0,0,0,0};
	if(tigrKeyDown(console->tscreen, TK_UP))    { binArray[0] = 1; }
	if(tigrKeyDown(console->tscreen, TK_DOWN))  { binArray[1] = 1; }
	if(tigrKeyDown(console->tscreen, TK_RIGHT)) { binArray[2] = 1; }
	if(tigrKeyDown(console->tscreen, TK_LEFT))  { binArray[3] = 1; }
	if(tigrKeyDown(console->tscreen, 'Z'))      { binArray[4] = 1; }
	if(tigrKeyDown(console->tscreen, 'X'))      { binArray[5] = 1; }
	int bin = 0;
	int mult = 1;
	for(int i = 7; i >= 0; i--) {
		bin += binArray[i]*mult;
		mult *= 2;
	}
	Poke(&console->ram, 0x526E, bin);
}

