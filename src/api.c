
#include "api.h"
#include "console.h"
#include "globals.h"
#include "ram.h"
#include "utils.h"
#include <raylib.h>
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

		int addr = RAM_FONT_START+(char_height*(s[i]-32));

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

void SprOne(Ram *spriteRam, Ram *drawRam, int id, int x, int y, int colorkey, int scale) {
	int id_addr = RAM_SPRITES_START+(id*32);
	for(int j = 0; j < 8; j++) {
		for (int i = 0; i < 4; i++) {
			int colors = Peek(spriteRam, id_addr+i+(j*4));
			if((colors>>4) != colorkey) {
				RectF(drawRam, 
						x+((i*scale)*2), 
						y+j+(j*(scale-1)), 
						scale, scale, 
						colors>>4
				);
			}
			if ((colors&0xf) != colorkey) {
				RectF(drawRam, 
						x+((i*scale)*2)+scale, 
						y+j+(j*(scale-1)), 
						scale, scale, 
						colors&0xf
				);
			}
		}
	}
}
// Ugly
void Spr(Ram *spriteRam, Ram *drawRam, int id, int x, int y, int colorkey, int w, int h, int scale) {
	int nextId = id;
	int mapY = floor(id/12.0f);
	int mapX = id%12;
	for (int j = mapY; j < mapY+h; j++) {
		for (int i = 0; i < w; i++) {
			SprOne(spriteRam, drawRam, nextId, x+(i*(scale*8)), y+((j-mapY)*(scale*8)), colorkey, scale);	
			nextId++;
		}
		nextId = ((j+1)*12)+mapX;
	}
}

void Mouse(Ram *ram) {
	int x, y, btn;
	int scale = min((float)GetScreenWidth()/SCREEN_WIDTH, (float)GetScreenHeight()/SCREEN_HEIGHT);
	x = (GetMouseX()-((GetScreenWidth()-(SCREEN_WIDTH*scale))*0.5f))/scale;
	y = (GetMouseY()-((GetScreenHeight()-(SCREEN_HEIGHT*scale))*0.5f))/scale;
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		btn = 1;
	}
	Poke(ram, RAM_MOUSE_START ,  x);
	Poke(ram, RAM_MOUSE_START+1, y);
	Poke(ram, RAM_MOUSE_START+2, btn);
}

void GetKeys(Ram *ram) {
	Poke(ram, RAM_KEYBOARD_START, GetCharPressed());
}

void Btn(Ram *ram) {
	int binArray[8] = {0,0,0,0,0,0,0,0};
	int hex = Peek(ram, RAM_BUTTONS_START+1);
	for(int i = 0; i < 8; i++) {
		if(hex & (1 << (7-i))) {
			binArray[i] = 1;
		}
	}
	if(IsKeyDown(KEY_UP))     { binArray[0] = 1; } else { binArray[0] = 0; }
	if(IsKeyDown(KEY_DOWN))   { binArray[1] = 1; } else { binArray[1] = 0; }
	if(IsKeyDown(KEY_RIGHT))  { binArray[2] = 1; } else { binArray[2] = 0; }
	if(IsKeyDown(KEY_LEFT))   { binArray[3] = 1; } else { binArray[3] = 0; }
	if(IsKeyDown(KEY_Z))      { binArray[4] = 1; } else { binArray[4] = 0; }
	if(IsKeyDown(KEY_X))      { binArray[5] = 1; } else { binArray[5] = 0; }
	int bin = 0;
	int mult = 1;
	for(int i = 7; i >= 0; i--) {
		bin += binArray[i]*mult;
		mult *= 2;
	}
	Poke(ram, RAM_BUTTONS_START+1, bin);
}
void BtnP(Ram *ram) {
	int binArray[8] = {0,0,0,0,0,0,0,0};
	if(IsKeyDown(KEY_UP))     { binArray[0] = 1; }
	if(IsKeyDown(KEY_DOWN))   { binArray[1] = 1; }
	if(IsKeyDown(KEY_RIGHT))  { binArray[2] = 1; }
	if(IsKeyDown(KEY_LEFT))   { binArray[3] = 1; }
	if(IsKeyDown(KEY_Z))      { binArray[4] = 1; }
	if(IsKeyDown(KEY_X))      { binArray[5] = 1; }
	int bin = 0;
	int mult = 1;
	for(int i = 7; i >= 0; i--) {
		bin += binArray[i]*mult;
		mult *= 2;
	}
	Poke(ram, RAM_BUTTONS_START, bin);
}

int SprGetPx(Ram *ram, int id, int x, int y, int w, int h) {
	if (x < 0 || y < 0 || x >= w*8 || y >= h*8)
		return -1;

	int spr_offset = (x/8) + ((y/8) * (12));
	int id_addr = RAM_SPRITES_START+((id+spr_offset)*32);

	int pos = ((x%8)/2) + ((y%8)*4);

	uint8_t colors = Peek(ram, id_addr+pos);
	if(x % 2 == 0) {
		return colors >> 4; // Left most 4 bits (left pix)
	} else {
		return colors & 0xF; // Right most 4 bits (right pix)
	}
}
void SprSetPx(Ram *ram, int id, int x, int y, int w, int h, int col) {
	if (x < 0 || y < 0 || x >= w*8 || y >= h*8)
		return;
	
	int spr_offset = (x/8) + ((y/8) * (12));
	int id_addr = RAM_SPRITES_START+((id+spr_offset)*32);

	int pos = ((x%8)/2) + ((y%8)*4);

	uint8_t colors = Peek(ram, id_addr+pos);
	if(x % 2 == 0) {
		Poke(ram, id_addr+pos, (col<<4) | (colors&0xF));
	} else {
		Poke(ram, id_addr+pos, (colors&0xF0) | col);
	}
}

