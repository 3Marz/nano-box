#ifndef EDITOR_H
#define EDITOR_H


#include "../console.h"
#include <stdbool.h>
#include "../../include/sds.h"
#include "code.h"
#include "sprite.h"

enum EditorMode {
	EDITOR_MODE_CODEEDITOR,
	EDITOR_MODE_SPRITEEDITOR
};

typedef struct Editor {
	Console *console;
	Ram ram;

	enum EditorMode mode;

	CodeEditor *code;
	SpriteEditor *sprite;

} Editor;

void editor_new(Editor* e, Console* c);
void editor_run(Editor* e);
void editor_close(Editor* e);

#endif
