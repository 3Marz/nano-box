
#include "editor.h"
#include "../api.h"
#include "../ram.h"
#include "../utils.h"
#include "code.h"
#include "sprite.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>


const int editorSprites[] = {
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0x2F, 0xF2, 0xFF,
	0xF2, 0xFF, 0xFF, 0x2F,
	0xF2, 0xFF, 0xFF, 0x2F,
	0xF2, 0xFF, 0xFF, 0x2F,
	0xF2, 0xFF, 0xFF, 0x2F,
	0xFF, 0x2F, 0xF2, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,

	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0x2F, 0x2F, 0x2F,
	0xF2, 0xF2, 0xF2, 0xFF,
	0xFF, 0x2F, 0x2F, 0x2F,
	0xF2, 0xF2, 0xF2, 0xFF,
	0xFF, 0x2F, 0x2F, 0x2F,
	0xF2, 0xF2, 0xF2, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
};

void editor_new(Editor* e, Console *c) {
	e->console = c;
	e->mode = EDITOR_MODE_SPRITEEDITOR;
	ram_init(&e->ram);

	e->code = code_init();
	e->sprite = sprite_editor_init();
	int sprlen = sizeof(editorSprites) / sizeof(int);
	//Load sprites
	for (int i = RAM_SPRITES_START; i < RAM_SPRITES_START+sprlen; i++) {
		Poke(&e->ram, i, editorSprites[i - 0x326A]);
	}

	code_update_syntax(e->code);
}

void draw_code(Editor *e) {
	for (int i = 0; i < e->code->len; i++) {
		for (int j = 0; j < sdslen(e->code->data[i]); j++) {
			char c[2] = "\0";
			c[0] = e->code->data[i][j];
			Text(&e->ram, (j*5) + e->code->scrollx*5, (e->code->scrolly*6) + (i*6) + e->code->yoff, c, e->code->syntax[i][j]);
		}
	}
}

void handle_keyboad_input(Editor *e) {
	// For Characters
	GetKeys(&e->ram);
	char c = Peek(&e->ram, RAM_KEYBOARD_START); 
	if (c != 0) {
		e->code->data[e->code->row] = sdsinschar(e->code->data[e->code->row], e->code->col-1, c);
		e->code->col++;
	}

	// For Delete
	if (IsKeyPressedNRepeat(KEY_BACKSPACE)) {
		if (e->code->col >= 1) {
			delete_char(e->code);
		} else if (e->code->row > 0) {
			delete_line(e->code);
		}
	}

	// For Enter
	if (IsKeyPressedNRepeat(KEY_ENTER)) {
		insert_new_line(e->code);
	}

	// For Tabs
	if (IsKeyPressedNRepeat(KEY_TAB)) {
		e->code->data[e->code->row] = sdsinschar(e->code->data[e->code->row], e->code->col-1, ' ');
		e->code->col++;
	}

	code_update_syntax(e->code);
}
void handle_arrow_input(Editor *e) {
	if (IsKeyPressedNRepeat(KEY_UP) && e->code->row > 0) {
		e->code->row--;
		if (e->code->col > sdslen(e->code->data[e->code->row])) {
			e->code->col = sdslen(e->code->data[e->code->row]);
		}
	}
	if (IsKeyPressedNRepeat(KEY_DOWN) && e->code->row < e->code->len-1) {
		e->code->row++;
		if (e->code->col > sdslen(e->code->data[e->code->row])) {
			e->code->col = sdslen(e->code->data[e->code->row]);
		}
	}
	if (IsKeyPressedNRepeat(KEY_RIGHT)) {
		if (e->code->col < sdslen(e->code->data[e->code->row])) {
			e->code->col++;
		}
		else if (e->code->col == sdslen(e->code->data[e->code->row]) && e->code->row < e->code->len-1) {
			e->code->col = 0;
			e->code->row++;
		}
	}
	if (IsKeyPressedNRepeat(KEY_LEFT)) {
		if (e->code->col > 0) {
			e->code->col--;
		}
		else if (e->code->col == 0 && e->code->row > 0) {
			e->code->row--;
			e->code->col = sdslen(e->code->data[e->code->row]);
		}
	}
}

void code_editor_run(Editor *e) {
	int cursorX = (e->code->scrollx*5) + (e->code->col+1)*5-5;
	int cursorY = (e->code->scrolly*6) + e->code->yoff + (e->code->row*6);
	if (cursorY > 128)             { e->code->scrolly--; }
	if (cursorY < e->code->yoff)   { e->code->scrolly++; }
	if (cursorX > 190)             { e->code->scrollx--; }
	if (cursorX < 0)               { e->code->scrollx++; }
	RectF(&e->ram, cursorX, cursorY, 5, 6, SYNTAX_CURSOR);
	handle_arrow_input(e);
	handle_keyboad_input(e);
	draw_code(e);
}

void editor_run(Editor *e) {
	Cls(&e->ram, SYNTAX_BACKGROUND);

	if (e->mode == EDITOR_MODE_CODEEDITOR) {
		code_editor_run(e);
	} else if (e->mode == EDITOR_MODE_SPRITEEDITOR) {
		sprite_editor_run(e->sprite, &e->ram);
	}

	// Topbar
	RectF(&e->ram, 0, 0, 192, 8, 14);
	Spr(&e->ram, 0, 192-8, 0, e->mode==EDITOR_MODE_CODEEDITOR ? 0 : 15, 1, 1, 1);
	Spr(&e->ram, 1, 192-16, 0, e->mode==EDITOR_MODE_SPRITEEDITOR ? 0 : 15, 1, 1, 1);

	// Handle mouse & button
	Mouse(&e->ram);
	int mx = Peek(&e->ram, RAM_MOUSE_START);
	int my = Peek(&e->ram, RAM_MOUSE_START+1);
	int mbtn = Peek(&e->ram, RAM_MOUSE_START+2);

	if (pos_in_rect(192-8, 0, 8, 8, mx, my) && mbtn == 1) {
		e->mode = EDITOR_MODE_CODEEDITOR;
	}
	if (pos_in_rect(192-16, 0, 8, 8, mx, my) && mbtn == 1) {
		e->mode = EDITOR_MODE_SPRITEEDITOR;
	}
}

void editor_close(Editor *e) {
	printf("Closing editor with acc %d lines\n", e->code->len);
	code_close(e->code);
	sprite_editor_close(e->sprite);
}
