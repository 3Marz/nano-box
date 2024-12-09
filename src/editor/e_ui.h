#ifndef E_UI_H
#define E_UI_H

#include <stdbool.h>

typedef struct { 
	int focused;
	bool locked;

	// Keep track of new ui elements ids
	int uidGen;
} UI_Context;

void ui_ctx_update();

typedef struct {
	int id;
	int x;
	int y;
	int w;
	int h;
	int type;
	int sprite;
} Button;

Button button_init(int x, int y, int w, int h, int type, int sprite);

bool button_is_pressed(Button b, int mouseX, int mouseY);
bool button_is_held(Button b, int mouseX, int mouseY);
bool button_is_hovered(Button b, int mouseX, int mouseY);

#endif
