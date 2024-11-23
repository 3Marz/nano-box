#ifndef EDITOR_H
#define EDITOR_H


#include "console.h"
#include <stdbool.h>
#include "../include/sds.h"

enum EditorMode {
	EDITOR_MODE_TERM,
	EDITOR_MODE_CODEEDITOR,
	EDITOR_MODE_SPRITEEDITOR
};

typedef struct ArrowKeyState {
	int heldFor;
	bool prev;
} ArrowKeyState;

typedef struct Editor {
	Console *console;

	enum EditorMode mode;

	struct {
		int yoff;
		int col;
		int row;
		int len;
		sds* data;
	} code;

	struct {
		int initialDelay;
		int repeatDelay;
		int holds[255];
		bool prev[255];
	} keyboard;

	ArrowKeyState arrowKeys[4];

} Editor;

Editor* editor_new(Console* c);
void editor_run(Editor* e);
void editor_close(Editor* e);

#endif
