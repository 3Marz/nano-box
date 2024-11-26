
#include "editor.h"
#include "../api.h"
#include "../ram.h"
#include "../utils.h"
#include "code.h"
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
	e->mode = EDITOR_MODE_CODEEDITOR;
	ram_init(&e->ram);

	e->code = code_init();
	int sprlen = sizeof(editorSprites) / sizeof(int);
	//Load sprites
	for (int i = 0x326A; i < 0x326A+sprlen; i++) {
		Poke(&e->ram, i, editorSprites[i - 0x326A]);
	}

	e->keyboard.initialDelay = 20;
	e->keyboard.repeatDelay = 2;
	for (int i = 0; i < 255; i++) {
		e->keyboard.holds[i] = 0;
		e->keyboard.prev[i] = 0;
	}
	code_update_syntax(e->code);
}

bool arrow_keys(Editor *e, int key) {
	Btn(e->console);
	int byte = Peek(&e->console->ram, 0x526F);
	int bit = byte >> (7-key) & 0b1;
	if (bit) {
		e->arrowKeys[key].heldFor++;
		if (e->arrowKeys[key].prev == false) {
			e->arrowKeys[key].prev = true;
			return true;
		}
		int heloTooLong = e->arrowKeys[key].heldFor > e->keyboard.initialDelay;
		if (heloTooLong) {
			e->arrowKeys[key].heldFor = e->keyboard.initialDelay - e->keyboard.repeatDelay;
			return true;
		}
	} else {
		e->arrowKeys[key].heldFor = 0;
		e->arrowKeys[key].prev = false;
	}
	return false;
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

void handle_editor_on_input(Editor *e, char c) {
	if (c == '\b') { // Backspace
		if (e->code->col >= 1) {
			delete_char(e->code);
		} else if (e->code->row > 0) {
			delete_line(e->code);
		}
	}
	else if (c == 13) {	
		insert_new_line(e->code);
	}
	else { // Normal character
		if (c == 9) c = 32; // Translate tab into space
		e->code->data[e->code->row] = sdsinschar(e->code->data[e->code->row], e->code->col-1, c);
		e->code->col++;
	}
	code_update_syntax(e->code);
}

void handle_keyboad_input(Editor *e) {
	// For keyboard
	GetKeys(e->console);
	char c = Peek(&e->console->ram, 0x526D); 
	if(c == 8 || c == 13 || c == 9 || c > 31) {
		e->keyboard.holds[c]++;
		if(e->keyboard.prev[c] == false) {
			handle_editor_on_input(e, c);
			e->keyboard.prev[c] = 1;
		}
		bool heldToLong = e->keyboard.holds[c] > e->keyboard.initialDelay; 
		if (heldToLong) {
			handle_editor_on_input(e, c);
			e->keyboard.holds[c] = e->keyboard.initialDelay - e->keyboard.repeatDelay;
		}
	} else {
		for (int i = 0; i < 255; i++) {
			e->keyboard.holds[i] = 0;
			e->keyboard.prev[i] = false;
		}
	}
}
void handle_arrow_input(Editor *e) {
	if (arrow_keys(e, 0) && e->code->row > 0) {
		e->code->row--;
		if (e->code->col > sdslen(e->code->data[e->code->row])) {
			e->code->col = sdslen(e->code->data[e->code->row]);
		}
	}
	if (arrow_keys(e, 1) && e->code->row < e->code->len-1) {
		e->code->row++;
		if (e->code->col > sdslen(e->code->data[e->code->row])) {
			e->code->col = sdslen(e->code->data[e->code->row]);
		}
	}
	if (arrow_keys(e, 2)) {
		if (e->code->col < sdslen(e->code->data[e->code->row])) {
			e->code->col++;
		}
		else if (e->code->col == sdslen(e->code->data[e->code->row]) && e->code->row < e->code->len-1) {
			e->code->col = 0;
			e->code->row++;
		}
	}
	if (arrow_keys(e, 3)) {
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
	}
	RectF(&e->ram, 0, 0, 192, 8, 14);
	Spr(&e->ram, 0, 0, 0, e->mode==EDITOR_MODE_CODEEDITOR ? 0 : 15, 1, 1);
	Spr(&e->ram, 1, 8, 0, e->mode==EDITOR_MODE_SPRITEEDITOR ? 0 : 15, 1, 1);

	int mx, my, mbtn;
	tigrMouse(e->console->tscreen, &mx, &my, &mbtn);
	printf("%i\n", mbtn);

	if (pos_in_rect(0, 0, 8, 8, mx, my) && mbtn == 1) {
		e->mode = EDITOR_MODE_CODEEDITOR;
	}
	if (pos_in_rect(8, 0, 8, 8, mx, my) && mbtn == 1) {
		e->mode = EDITOR_MODE_SPRITEEDITOR;
	}
}

void editor_close(Editor *e) {
	printf("Closing editor with acc %d lines\n", e->code->len);
	code_close(e->code);
}
