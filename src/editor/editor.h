#ifndef EDITOR_H
#define EDITOR_H


#include "../console.h"
#include <stdbool.h>
#include "../../include/sds.h"
#include "code.h"

enum EditorMode {
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

	CodeEditor *code;

	struct {
		int initialDelay;
		int repeatDelay;
		int holds[255];
		bool prev[255];
	} keyboard;

	ArrowKeyState arrowKeys[4];

} Editor;

void editor_new(Editor* e, Console* c);
void editor_run(Editor* e);
void editor_close(Editor* e);

bool arrow_keys(Editor *e, int key);

#endif
