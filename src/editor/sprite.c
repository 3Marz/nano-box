
#include "sprite.h"
#include "../api.h"
#include "../utils.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>


SpriteEditor *sprite_editor_init() {
	SpriteEditor *e = malloc(sizeof(SpriteEditor));
	e->selected_sptite = 0;
	e->selected_color = 0;
	e->zoom = 4;
	return e;
}

void sprite_editor_run(SpriteEditor *e, Ram *ram) {
	Mouse(ram);
	int mx = Peek(ram, RAM_MOUSE_START);
	int my = Peek(ram, RAM_MOUSE_START+1);
	int mbtn = Peek(ram, RAM_MOUSE_START+2);
	RectF(ram, 192/2, 0, 192/2, 128, 15);

	// --------- Right Side --------- 
	Rectangle canvas = {100, 16, 8*8, 8*8};
	Rect(ram, canvas.x-1, canvas.y-1, canvas.width+2, canvas.height+2, 0);
	Spr(ram, ram, e->selected_sptite, canvas.x, canvas.y, -1, 1, 1, e->zoom*2);

	Rectangle colorRect = {8*22 - 4, 8*2, 8*2, 8*8};
	Rect(ram, 8*22 - 5, 8*2 - 1, 8*2 + 2, 8*8 + 2, 0);
	int selectedColorX, selectedColorY;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 8; j++) {
			RectF(ram, (8*22 - 4)+(i*8), (8*2)+(j*8), 8, 8, i+(j*2));
			if (e->selected_color == i+(j*2)) {
				selectedColorX = (8*22 - 4)+(i*8);
				selectedColorY = (8*2)+(j*8);
			}
		}
	}
	if (pos_in_rect(colorRect.x, colorRect.y, colorRect.width-1, colorRect.height-1, mx, my) && mbtn == 1) {
		int cmx = (mx - colorRect.x)/8;
		int cmy = (my - colorRect.y)/8;
		e->selected_color = cmx+(cmy*2);
	}
	Rect(ram, selectedColorX-1, selectedColorY-1, 10, 10, 2);
	Rect(ram, selectedColorX, selectedColorY, 8, 8, 0);

	if (pos_in_rect(canvas.x, canvas.y, canvas.width-1, canvas.height-1, mx, my) && mbtn == 1) {
		int smx = (mx - canvas.x)/8;
		int smy = (my - canvas.y)/8;
		int pixelPos = smx+(smy*8);
		int pixelHex = Peek(ram, RAM_SPRITES_START+(e->selected_sptite*32)+(pixelPos/2));
		int newPixel;
		if (pixelPos % 2 == 0) { // --- Left Pixel
			newPixel = (e->selected_color<<4) | (pixelHex&0x0f);
		} else { // ------------------- Right Pixel
			newPixel = (pixelHex&0xf0) | (e->selected_color);
		}
		Poke(ram, RAM_SPRITES_START+(e->selected_sptite*32)+(pixelPos/2), newPixel);
	}

	// --------- Left Side ---------
	int xoff = 0;
	int yoff = 8;

	if (mx < 192/2 && my >= 8 && mbtn == 1) {
		e->selected_sptite = ((mx/8) + (my/8)*12)-12; 
	}

	int selectedSpriteX, selectedSpriteY;
	for (int i = 0; i < 180; i++) {
		Spr(ram, ram, i, xoff, yoff, 0, 1, 1, 1);

		if (e->selected_sptite == i) {
			selectedSpriteX = xoff;
			selectedSpriteY = yoff;
		}
		/*Rect(ram, xoff, yoff, 8, 8, 2);*/
		xoff += 8;
		if (xoff >= 8*12) {
			xoff = 0;
			yoff += 8;
		}
	}
	Rect(ram, selectedSpriteX-1, selectedSpriteY-1, 10, 10, 2);
}

void sprite_editor_close(SpriteEditor *e) {
	free(e);
}

