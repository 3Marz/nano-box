
#include "sprite.h"
#include "../api.h"
#include "../utils.h"
#include "e_ui.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>


SpriteEditor *sprite_editor_init() {
	SpriteEditor *e = malloc(sizeof(SpriteEditor));
	e->selected_sptite = 0;
	e->selected_color = 0;
	e->zoom = 4;

	e->selected_tool = SPRITE_TOOL_PENCIL;
	return e;
}

void tools_pencil(SpriteEditor *e, Rectangle canvas, int mx, int my, Ram *consoleRam) {
	if (!IsMouseButtonDown(MOUSE_LEFT_BUTTON)) return;
	int smx = (mx - canvas.x)/8;
	int smy = (my - canvas.y)/8;
	int pixelPos = smx+(smy*8);
	int pixelHex = Peek(consoleRam, RAM_SPRITES_START+(e->selected_sptite*32)+(pixelPos/2));
	int newPixel;
	if (pixelPos % 2 == 0) { // --- Left Pixel
		newPixel = (e->selected_color<<4) | (pixelHex&0x0f);
	} else { // ------------------- Right Pixel
		newPixel = (pixelHex&0xf0) | (e->selected_color);
	}
	Poke(consoleRam, RAM_SPRITES_START+(e->selected_sptite*32)+(pixelPos/2), newPixel);
}

void sprite_editor_run(SpriteEditor *e, Ram *editorRam, Ram *consoleRam) {
	Mouse(editorRam);
	int mx = Peek(editorRam, RAM_MOUSE_START);
	int my = Peek(editorRam, RAM_MOUSE_START+1);
	int mbtn = Peek(editorRam, RAM_MOUSE_START+2);
	RectF(editorRam, 192/2, 0, 192/2, 128, 15);

	// --------- Right Side --------- 
	Rectangle canvas = {100, 16, 8*8, 8*8};
	Rect(editorRam, canvas.x-1, canvas.y-1, canvas.width+2, canvas.height+2, 0);
	Spr(consoleRam, editorRam, e->selected_sptite, canvas.x, canvas.y, -1, 1, 1, e->zoom*2);

	Rectangle colorRect = {8*22 - 4, 8*2, 8*2, 8*8};
	Rect(editorRam, 8*22 - 5, 8*2 - 1, 8*2 + 2, 8*8 + 2, 0);
	int selectedColorX, selectedColorY;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 8; j++) {
			RectF(editorRam, (8*22 - 4)+(i*8), (8*2)+(j*8), 8, 8, i+(j*2));
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
	Rect(editorRam, selectedColorX-1, selectedColorY-1, 10, 10, 2);
	Rect(editorRam, selectedColorX, selectedColorY, 8, 8, 0);

	// Handling Tools
	if (pos_in_rect(canvas.x, canvas.y, canvas.width-1, canvas.height-1, mx, my)) {
		switch (e->selected_tool) {
			case SPRITE_TOOL_PENCIL:
				tools_pencil(e, canvas, mx, my, consoleRam);
				break;
			case SPRITE_TOOL_FILL:
				break;
			case SPRITE_TOOL_SELECT:
				break;
			default:
				break;
		}
	}

	// sprite and color indicators
	char selectedSprStr[5];
	char selectedColorStr[4];
	sprintf(selectedSprStr, "#%d", e->selected_sptite);
	sprintf(selectedColorStr, "#%d", e->selected_color);

	Text(editorRam, 8*12+4, 9, selectedSprStr, 2);
	Text(editorRam, 8*21+4, 9, selectedColorStr, 2);

	// flags TODO
	for (int i = 0; i < 8; i++) {
		Circ(editorRam, 104+(i*8), 8*11, 2, 0);
	}

	// Tools
	Button tools[3] = {
		{8*13-4, 8*12, 8, 8, SPRITE_TOOL_PENCIL, 2},
		{8*14-2, 8*12, 8, 8, SPRITE_TOOL_FILL, 4},
		{8*15, 8*12, 8, 8, SPRITE_TOOL_SELECT, 6},
		/*{8*16, 8*12, 8, 8, 3},	*/
	};
	for (int i = 0; i < 3; i++) {
		Spr(editorRam, editorRam, 
				e->selected_tool == i ? tools[i].sprite+1 : tools[i].sprite, 
				tools[i].x, tools[i].y, 0, 1, 1, 1);
		if (button_is_pressed(tools[i], mx, my, mbtn)) {
			e->selected_tool = tools[i].type;
		}
	}
	

	// --------- Left Side ---------
	int xoff = 0;
	int yoff = 8;

	if (mx < 192/2 && my >= 8 && mbtn == 1) {
		e->selected_sptite = ((mx/8) + (my/8)*12)-12; 
	}

	int selectedSpriteX, selectedSpriteY;
	for (int i = 0; i < 180; i++) {
		Spr(consoleRam, editorRam, i, xoff, yoff, 0, 1, 1, 1);

		if (e->selected_sptite == i) {
			selectedSpriteX = xoff;
			selectedSpriteY = yoff;
		}
		/*Rect(editorRam, xoff, yoff, 8, 8, 2);*/
		xoff += 8;
		if (xoff >= 8*12) {
			xoff = 0;
			yoff += 8;
		}
	}
	Rect(editorRam, selectedSpriteX-1, selectedSpriteY-1, 10, 10, 2);

	/*if (IsKeyPressed(KEY_Q)) {*/
	/*	int drop = 0;*/
	/*	for (int i = RAM_SPRITES_START; i < RAM_SPRITES_START+(32*5); i++) {*/
	/*		drop++;*/
	/*		printf("0x%x, ", Peek(consoleRam, i));*/
	/*		if (drop >= 8) {*/
	/*			printf("\n");*/
	/*			drop = 0;*/
	/*		}*/
	/*	}*/
	/*}*/
}

void sprite_editor_close(SpriteEditor *e) {
	free(e);
}

