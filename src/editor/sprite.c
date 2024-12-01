
#include "sprite.h"
#include "../api.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>


SpriteEditor *sprite_editor_init() {
	SpriteEditor *e = malloc(sizeof(SpriteEditor));
	e->selected = 0;
	e->zoom = 4;
	return e;
}

void sprite_editor_run(SpriteEditor *e, Ram *ram) {
	RectF(ram, 192/2, 0, 192/2, 128, 15);
	// Right Side
	Rectangle canvas = {104, 16, 8*8, 8*8};
	Rect(ram, canvas.x-1, canvas.y-1, canvas.width+2, canvas.height+2, 0);
	Spr(ram, ram, e->selected, canvas.x, canvas.y, -1, 1, 1, e->zoom*2);

	// Left Side
	int xoff = 0;
	int yoff = 8;
	
	Mouse(ram);
	int mx = Peek(ram, RAM_MOUSE_START);
	int my = Peek(ram, RAM_MOUSE_START+1);
	int mbtn = Peek(ram, RAM_MOUSE_START+2);


	if (mx < 192/2 && my >= 8 && mbtn == 1) {
		e->selected = ((mx/8) + (my/8)*12)-12; 
	}

	for (int i = 0; i < 180; i++) {
		Spr(ram, ram, i, xoff, yoff, 0, 1, 1, 1);

		if (e->selected == i)
			Rect(ram, xoff-1, yoff-1, 10, 10, 2);
		/*Rect(ram, xoff, yoff, 8, 8, 2);*/
		xoff += 8;
		if (xoff >= 8*12) {
			xoff = 0;
			yoff += 8;
		}
	}
}

void sprite_editor_close(SpriteEditor *e) {
	free(e);
}

