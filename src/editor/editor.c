
#include "editor.h"
#include "../api.h"
#include "../ram.h"
#include "../utils.h"
#include "code.h"
#include <stdbool.h>
#include <stdio.h>

void editor_new(Editor* e, Console *c) {
	e->console = c;
	e->mode = EDITOR_MODE_CODEEDITOR;

	e->code = code_init();

	e->keyboard.initialDelay = 20;
	e->keyboard.repeatDelay = 2;
	for (int i = 0; i < 255; i++) {
		e->keyboard.holds[i] = 0;
		e->keyboard.prev[i] = 0;
	}
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
		Text(e->console, e->code->scrollx*5, (e->code->scrolly*6) + (i*6) + e->code->yoff, e->code->data[i], 2);
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
	int cursorX = (e->code->scrollx*5) + (e->code->col+1)*5 - 2;
	int cursorY = (e->code->scrolly*6) + e->code->yoff + (e->code->row*6);
	if (cursorY > 128)             { e->code->scrolly--; }
	if (cursorY < e->code->yoff)   { e->code->scrolly++; }
	if (cursorX > 190)             { e->code->scrollx--; }
	if (cursorX < 0)               { e->code->scrollx++; }
	RectF(e->console, cursorX, cursorY, 5, 6, 7);
	handle_arrow_input(e);
	handle_keyboad_input(e);
	draw_code(e);
}

void editor_run(Editor *e) {
	Cls(e->console, 0);
	if (e->mode == EDITOR_MODE_CODEEDITOR) {
		code_editor_run(e);
	}
	RectF(e->console, 0, 0, 192, 7, 1);
}

void editor_close(Editor *e) {
	printf("Closing editor with acc %d lines\n", e->code->len);
	code_close(e->code);
}
