#ifndef SPRITE_H
#define SPRITE_H

#include "../ram.h"
#include "e_ui.h"

enum SpriteTools {
	SPRITE_TOOL_PENCIL,
	SPRITE_TOOL_FILL,
	SPRITE_TOOL_SELECT,
};

typedef struct SpriteEditorLayout {
	// Right
	Button tools[3];

	Button zoomBoundingBox;

	Button canvas;
	Button colors;

	// Left
	Button sprites;

} SpriteEditorLayout;

SpriteEditorLayout sprite_editor_layout_init();

typedef struct SpriteEditor {
	int selected_sptite;
	int selected_color;
	int zoom;

	SpriteEditorLayout lo;

	enum SpriteTools selected_tool;
} SpriteEditor;

SpriteEditor *sprite_editor_init();
void sprite_editor_run(SpriteEditor *e, Ram *editorRam, Ram *consoleRam);
void sprite_editor_close(SpriteEditor *e);

#endif
