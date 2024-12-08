#ifndef E_UI_H
#define E_UI_H

#include <stdbool.h>

typedef struct {
	int x;
	int y;
	int w;
	int h;
	int type;
	int sprite;
} Button;

bool button_is_pressed(Button b, int mouseX, int mouseY);
bool button_is_held(Button b, int mouseX, int mouseY);
bool button_is_hovered(Button b, int mouseX, int mouseY);

#endif
