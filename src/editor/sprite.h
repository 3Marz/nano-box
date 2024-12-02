#ifndef SPRITE_H
#define SPRITE_H

#include "../ram.h"

typedef struct SpriteEditor {
	int selected_sptite;
	int selected_color;
	int zoom;
} SpriteEditor;

SpriteEditor *sprite_editor_init();
void sprite_editor_run(SpriteEditor *e, Ram *editorRam, Ram *consoleRam);
void sprite_editor_close(SpriteEditor *e);

#endif
