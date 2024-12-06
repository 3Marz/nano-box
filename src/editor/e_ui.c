
#include "e_ui.h"
#include <raylib.h>

bool button_is_pressed(Button b, int mouseX, int mouseY, int mouseBtn) {
	if (mouseX >= b.x && mouseX <= b.x + b.w && mouseY >= b.y && mouseY <= b.y + b.h)
		return IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
	else
		return false;
}

