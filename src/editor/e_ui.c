
#include "e_ui.h"
#include <raylib.h>

bool button_is_pressed(Button b, int mouseX, int mouseY) {
	if (mouseX >= b.x && mouseX <= b.x + b.w && mouseY >= b.y && mouseY <= b.y + b.h)
		return IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
	else
		return false;
}

bool button_is_held(Button b, int mouseX, int mouseY) {
	if (mouseX >= b.x && mouseX <= b.x + b.w && mouseY >= b.y && mouseY <= b.y + b.h)
		return IsMouseButtonDown(MOUSE_BUTTON_LEFT);
	else
		return false;
}

bool button_is_hovered(Button b, int mouseX, int mouseY) {
	if (mouseX >= b.x && mouseX <= b.x + b.w && mouseY >= b.y && mouseY <= b.y + b.h)
		return true;
	else
		return false;
}
