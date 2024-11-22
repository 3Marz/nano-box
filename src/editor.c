
#include "editor.h"
#include "api.h"
#include "ram.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Editor* editor_new(Console *c) {
	Editor *e = malloc(sizeof(Editor));
	e->console = c;
	e->mode = EDITOR_MODE_CODEEDITOR;

	e->code.yoff = 9;
	e->code.col = 0;
	e->code.row = 0;
	e->code.data = (sds *)malloc(sizeof(sds));
	if (e->code.data == NULL) {
		fprintf(stderr, "Editor: Could not create code editor\n");
		return NULL;
	}
	/*e->code.data[0] = (char *)malloc(2*sizeof(char));*/
	/*if (e->code.data[0] == NULL) {*/
	/*	fprintf(stderr, "Editor: Could not create line\n");*/
	/*	return NULL;*/
	/*}*/
	/*e->code.data[0] = NULL;*/
	e->code.data[0] = sdsnew("");
	printf("%s\n", e->code.data[0]);

	e->keyboard.initialDelay = 20;
	e->keyboard.repeatDelay = 2;
	for (int i = 0; i < 255; i++) {
		e->keyboard.holds[i] = 0;
		e->keyboard.prev[i] = 0;
	}

	return e;
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
	int len = sizeof(e->code.data)/sizeof(char*);
	for (int i = 0; i < len; i++) {
		Text(e->console, 0, (i*6)+e->code.yoff, e->code.data[i], 2);
	}
}

void handle_editor_on_input(Editor *e, char c) {
	if (c == '\b') { // Backspace
		printf("Backspace\n");	
	}
	else if (c == 13) { // Enter
		printf("Enter\n");	
	}
	else { // Normal character
		/*size_t newLen = strlen(e->code.data[e->code.row]) + 2;*/
		/**/
		/*char *tmp =	(char *)realloc(e->code.data[e->code.row], newLen);*/
		/*if (tmp == NULL) {*/
		/*	free(tmp);*/
		/*	fprintf(stderr, "Editor: Could not resize line\n");*/
		/*	return;*/
		/*}*/
		/*e->code.data[e->code.row] = tmp;*/
		/**/
		/*e->code.data[e->code.row][newLen-2] = c;*/
		/*e->code.data[e->code.row][newLen-1] = '\0';*/
		e->code.data[e->code.row] = sdsinschar(e->code.data[e->code.row], e->code.col-1, c);

		e->code.col++;
	}
}

void handle_keyboad_input(Editor *e) {
	// For keyboard
	GetKeys(e->console);
	char c = Peek(&e->console->ram, 0x526D); 
	if(c != 0) {
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
	if (arrow_keys(e, 0) && e->code.row > 0) {
		e->code.row--;
		if (e->code.col > strlen(e->code.data[e->code.row])) {
			e->code.col = strlen(e->code.data[e->code.row]);
		}
	}
	if (arrow_keys(e, 1) && e->code.row < (sizeof(e->code.data)/sizeof(char*)-1)) {
		e->code.row++;
		if (e->code.col > strlen(e->code.data[e->code.row])) {
			e->code.col = strlen(e->code.data[e->code.row]);
		}
	}
	if (arrow_keys(e, 2)) {
		if (e->code.col < strlen(e->code.data[e->code.row])) {
			e->code.col++;
		}
		else if (e->code.col == strlen(e->code.data[e->code.row]) && e->code.row < (sizeof(e->code.data)/sizeof(char*)-1)) {
			e->code.col = 0;
			e->code.row++;
		}
	}
	if (arrow_keys(e, 3)) {
		if (e->code.col > 0) {
			e->code.col--;
		}
		else if (e->code.col == 0 && e->code.row > 0) {
			e->code.col = strlen(e->code.data[e->code.row]);
			e->code.row--;
		}
	}
}

void editor_run(Editor *e) {
	Cls(e->console, 0);
	RectF(e->console, 0, 0, 192, 7, 1);
	if (e->mode == EDITOR_MODE_CODEEDITOR) {
		RectF(e->console, (e->code.col+1)*5-2, e->code.yoff+(e->code.row*6), 5, 6, 7);
		handle_arrow_input(e);
		handle_keyboad_input(e);
		draw_code(e);
	}
}

void editor_close(Editor *e) {
	int linesLen = sizeof(e->code.data)/sizeof(char*);
	printf("Closing editor with %d lines\n", linesLen);
	for (int i = 0; i < linesLen; i++) {
		sdsfree(e->code.data[i]);
	}
	free(e->code.data);
	free(e);
}
