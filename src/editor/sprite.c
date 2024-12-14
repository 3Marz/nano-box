
#include "sprite.h"
#include "../api.h"
#include "e_ui.h"
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int zoomLookup[4]    = {8, 4, 2, 1};
int zoomLookupRev[4] = {1, 2, 4, 8};

SpriteEditorLayout sprite_editor_layout_init() {
	SpriteEditorLayout lo;
	lo.zoomBoundingBox = button_init(8*22+4, 8*11+4, 8, 8*4, 0, 0);

	memcpy(&lo.tools, (Button[]){
		button_init(8*13-4, 8*12, 8, 8, SPRITE_TOOL_PENCIL, 2),
		button_init(8*14-2, 8*12, 8, 8, SPRITE_TOOL_FILL, 4),
		button_init(8*15, 8*12, 8, 8, SPRITE_TOOL_SELECT, 6),
	}, sizeof(Button)*3);

	memcpy(&lo.flagsBtns, (Button[]){
		button_init(104+(0*8)-2, 8*11-2, 5, 5, 0, 0),
		button_init(104+(1*8)-2, 8*11-2, 5, 5, 0, 0),
		button_init(104+(2*8)-2, 8*11-2, 5, 5, 0, 0),
		button_init(104+(3*8)-2, 8*11-2, 5, 5, 0, 0),
		button_init(104+(4*8)-2, 8*11-2, 5, 5, 0, 0),
		button_init(104+(5*8)-2, 8*11-2, 5, 5, 0, 0),
		button_init(104+(6*8)-2, 8*11-2, 5, 5, 0, 0),
		button_init(104+(7*8)-2, 8*11-2, 5, 5, 0, 0),
	}, sizeof(Button)*8);

	lo.canvas = button_init(100, 16, 8*8-1, 8*8-1, 0, 0);
	lo.colors = button_init(8*22-4, 8*2, 8*2-1, 8*8-1, 0, 0); 

	lo.sprites = button_init(0, 8, 8*12-1, 8*15-1, 0, 0);

	lo.spritesTab = button_init(1, 0, 7, 7, 0, 11);
	lo.tilesTab = button_init(9, 0, 7, 7, 0, 13);

	return lo;
}

SpriteEditor *sprite_editor_init() {
	SpriteEditor *e = malloc(sizeof(SpriteEditor));
	e->selected_sptite = 0;
	e->sprite_selection = (Rectangle){0, 8, 8, 8};
	e->selected_color = 0;
	e->zoom = 0;

	e->tab = 0;

	e->lo = sprite_editor_layout_init();

	e->selected_tool = SPRITE_TOOL_PENCIL;
	return e;
}

// Maddness
void tools_pencil(SpriteEditor *e, Button canvas, int mx, int my, Ram *consoleRam) {
	// get the mouse position relative to the canvas in a grid
	int smx = (int)((mx - canvas.x)/zoomLookup[e->zoom])%8;
	int smy = (int)((my - canvas.y)/zoomLookup[e->zoom])%8;
	int pixelPos = smx+(smy*8);

	// get the hovered sprite id
	int hoveredSprX = (mx-canvas.x)/(zoomLookup[e->zoom]*8);
	int hoveredSprY = (my-canvas.y)/(zoomLookup[e->zoom]*8);
	int hoveredId = hoveredSprX + (hoveredSprY*12);
	int sprId = e->selected_sptite+hoveredId;

	// set the pixel
	int pixelHex = Peek(consoleRam, RAM_SPRITES_START+(sprId*32)+(pixelPos/2));
	int newPixel;
	if (pixelPos % 2 == 0) { // --- Left Pixel
		newPixel = (e->selected_color<<4) | (pixelHex&0x0f);
	} else { // ------------------- Right Pixel
		newPixel = (pixelHex&0xf0) | (e->selected_color);
	}
	Poke(consoleRam, RAM_SPRITES_START+(sprId*32)+(pixelPos/2), newPixel);
}

void tools_eyedropper(SpriteEditor *e, Button canvas, int mx, int my, Ram *consoleRam) {
	int smx = (int)((mx - canvas.x)/zoomLookup[e->zoom]);
	int smy = (int)((my - canvas.y)/zoomLookup[e->zoom]);

	e->selected_color = SprGetPx(consoleRam, e->selected_sptite, smx, smy, zoomLookupRev[e->zoom], zoomLookupRev[e->zoom]);
};

void flood_fill(Ram *spriteRam, SpriteEditor *e, int baseCol, int targetCol, int x, int y) {
	int onPix = SprGetPx(spriteRam, e->selected_sptite, x, y, zoomLookupRev[e->zoom], zoomLookupRev[e->zoom]);
	if (onPix == baseCol) {
		SprSetPx(spriteRam, e->selected_sptite, x, y, zoomLookupRev[e->zoom], zoomLookupRev[e->zoom], targetCol);
		flood_fill(spriteRam, e, baseCol, targetCol, x-1, y);
		flood_fill(spriteRam, e, baseCol, targetCol, x+1, y);
		flood_fill(spriteRam, e, baseCol, targetCol, x, y-1);
		flood_fill(spriteRam, e, baseCol, targetCol, x, y+1);
	}
}

void tools_fill(SpriteEditor *e, Button canvas, int mx, int my, Ram *consoleRam) {
	// get the mouse position relative to the canvas in a grid
	int smx = (int)((mx - canvas.x)/zoomLookup[e->zoom]);
	int smy = (int)((my - canvas.y)/zoomLookup[e->zoom]);

	int baseCol = SprGetPx(consoleRam, e->selected_sptite, smx, smy, zoomLookupRev[e->zoom], zoomLookupRev[e->zoom]);
	if (baseCol == e->selected_color)
		return;

	flood_fill(consoleRam, e, baseCol, e->selected_color, smx, smy);
}

void ui_zoom_slider(SpriteEditor *e, Ram *editorRam, int mx, int my) {
	if (button_is_held(e->lo.zoomBoundingBox, mx, my, MOUSE_BUTTON_LEFT)) {
		int zmy = (my - e->lo.zoomBoundingBox.y-1)/8;
		e->zoom = zmy;

		e->sprite_selection.width = 8*zoomLookupRev[e->zoom];
		e->sprite_selection.height = 8*zoomLookupRev[e->zoom];

		if (e->sprite_selection.x+e->sprite_selection.width > 8*12)
			e->sprite_selection.x = 8*12 - e->sprite_selection.width;
		if (e->sprite_selection.y+e->sprite_selection.height > 8*16)
			e->sprite_selection.y = 8*16 - e->sprite_selection.height;
		e->selected_sptite = ((e->sprite_selection.x/8 + (e->sprite_selection.y/8)*12)-12)+(e->tab*180);
	}
	RectF(editorRam, 8*23-1, 8*11+3, 2, 8*4+2, 0);
	RectF(editorRam, 8*22+5, (8*11+6)+(e->zoom*8), 6, 4, 2);
	Rect(editorRam, 8*22+5, (8*11+6)+(e->zoom*8), 6, 4, 0);
	Spr(editorRam, editorRam, 10, 8*22+4, 8*10+2, 0, 1, 1, 1);
}

void handle_flags(SpriteEditor *e, Ram *editorRam, Ram *consoleRam, int mx, int my) {
	for (int i = 0; i < 8; i++) {
		Circ(editorRam, 104+(i*8), 8*11, 2, 0);
		if (FGet(consoleRam, e->selected_sptite, i)) {
			RectF(editorRam, 104+(i*8)-1, 8*11-1, 3,3 , i+5);
			PxSet(editorRam, 104+(i*8)-1, 8*11-1, 2);
		}
		if (button_is_pressed(e->lo.flagsBtns[i], mx, my, MOUSE_LEFT_BUTTON)) {
			FSet(consoleRam, e->selected_sptite, i, !FGet(consoleRam, e->selected_sptite, i));
		}
	}
}

void sprite_editor_run(SpriteEditor *e, Ram *editorRam, Ram *consoleRam) {
	Mouse(editorRam);
	int mx = Peek(editorRam, RAM_MOUSE_START);
	int my = Peek(editorRam, RAM_MOUSE_START+1);
	RectF(editorRam, 192/2, 0, 192/2, 128, 15);

	// --------- Right Side --------- 
	Rect(editorRam, e->lo.canvas.x-1, e->lo.canvas.y-1, e->lo.canvas.w+3, e->lo.canvas.h+3, 0);
	Spr(consoleRam, editorRam, e->selected_sptite, e->lo.canvas.x, e->lo.canvas.y, -1, zoomLookupRev[e->zoom], zoomLookupRev[e->zoom], zoomLookup[e->zoom]);

	Rect(editorRam, e->lo.colors.x-1, e->lo.colors.y-1, e->lo.colors.w+3, e->lo.colors.h+3, 0);
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
	if (button_is_held(e->lo.colors, mx, my, MOUSE_BUTTON_LEFT)) {
		int cmx = (mx - e->lo.colors.x)/8;
		int cmy = (my - e->lo.colors.y)/8;
		e->selected_color = cmx+(cmy*2);
	}
	Rect(editorRam, selectedColorX-1, selectedColorY-1, 10, 10, 2);
	Rect(editorRam, selectedColorX, selectedColorY, 8, 8, 0);

	// Handling Tools
	switch (e->selected_tool) {
		case SPRITE_TOOL_PENCIL:
			if (button_is_held(e->lo.canvas, mx, my, MOUSE_BUTTON_LEFT))
				tools_pencil(e, e->lo.canvas, mx, my, consoleRam);
			else if (button_is_held(e->lo.canvas, mx, my, MOUSE_BUTTON_RIGHT))
				tools_eyedropper(e, e->lo.canvas, mx, my, consoleRam);
			break;
		case SPRITE_TOOL_FILL:
			if (button_is_pressed(e->lo.canvas, mx, my, MOUSE_BUTTON_LEFT))
				tools_fill(e, e->lo.canvas, mx, my, consoleRam);
			break;
		case SPRITE_TOOL_SELECT:
			break;
		default:
			break;
	}

	// sprite and color indicators
	char selectedSprStr[5];
	char selectedColorStr[4];
	sprintf(selectedSprStr, "#%d", e->selected_sptite);
	sprintf(selectedColorStr, "#%d", e->selected_color);

	Text(editorRam, 8*12+4, 9, selectedSprStr, 2);
	Text(editorRam, 8*21+4, 9, selectedColorStr, 2);

	// flags
	handle_flags(e, editorRam, consoleRam, mx, my);
	
	// Tools
	for (int i = 0; i < 3; i++) {
		Spr(editorRam, editorRam, 
				e->selected_tool == i ? e->lo.tools[i].sprite+1 : e->lo.tools[i].sprite, 
				e->lo.tools[i].x, e->lo.tools[i].y, 0, 1, 1, 1);
		if (button_is_pressed(e->lo.tools[i], mx, my, MOUSE_BUTTON_LEFT)) {
			e->selected_tool = e->lo.tools[i].type;
		}
	}

	// Zoom Slider
	ui_zoom_slider(e, editorRam, mx, my);

	// --------- Left Side ---------
	int xoff = 0;
	int yoff = 8;

	if (button_is_held(e->lo.sprites, mx, my, MOUSE_BUTTON_LEFT)) {
		// Calculate the x and y
		e->sprite_selection.x = (int)(mx/8)*8; 
		e->sprite_selection.y = (int)(my/8)*8;
		// Clamp the x and y
		e->sprite_selection.x = Clamp(e->sprite_selection.x, 0, 8*12 - e->sprite_selection.width); 
		e->sprite_selection.y = Clamp(e->sprite_selection.y, 0, 8*16 - e->sprite_selection.height); 

		e->selected_sptite = ((e->sprite_selection.x/8 + (e->sprite_selection.y/8)*12)-12)+(e->tab*180);
	}

	Spr(consoleRam, editorRam, e->tab*180, 0, 8, 0, 12, 15, 1);
	Rect(editorRam, e->sprite_selection.x-1, e->sprite_selection.y-1, e->sprite_selection.width+2, e->sprite_selection.height+2, 2);

	ui_ctx_update();
}

void sprite_editor_toolbar(SpriteEditor *e, Ram *editorRam, int mx, int my) {
	Spr(editorRam, editorRam, e->tab == 0 ? e->lo.spritesTab.sprite+1 : e->lo.spritesTab.sprite, 
			e->lo.spritesTab.x,e->lo.spritesTab.y, 0, 1,1 ,1);
	Spr(editorRam, editorRam, e->tab == 1 ? e->lo.tilesTab.sprite+1 : e->lo.tilesTab.sprite, 
			e->lo.tilesTab.x,e->lo.tilesTab.y, 0, 1,1 ,1);
	if (button_is_pressed(e->lo.spritesTab, mx, my, MOUSE_BUTTON_LEFT)) {
		e->tab = 0;
		if (e->selected_sptite >= 180) e->selected_sptite -= 180;
	}
	if (button_is_pressed(e->lo.tilesTab, mx, my, MOUSE_BUTTON_LEFT)) {
		e->tab = 1;
		if (e->selected_sptite < 180) e->selected_sptite += 180;
	}
}

void sprite_editor_close(SpriteEditor *e) {
	free(e);
}

