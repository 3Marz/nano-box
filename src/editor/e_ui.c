
#include "e_ui.h"
#include "../utils.h"
#include <raylib.h>
#include <stdio.h>

UI_Context ui_ctx = { -1, false, 0 };

void ui_ctx_update() {
	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
		ui_ctx.locked = false;
		ui_ctx.focused = -1;
	}
}

Button button_init(int x, int y, int w, int h, int type, int sprite) {
	Button b = { ui_ctx.uidGen++, x, y, w, h, type, sprite };
	return b;
}

bool button_is_pressed(Button b, int mouseX, int mouseY) {
	if (ui_ctx.locked && ui_ctx.focused != b.id) return false;

	if (pos_in_rect(b.x, b.y, b.w, b.h, mouseX, mouseY) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		ui_ctx.focused = b.id;
		ui_ctx.locked = true;
		return true;
	}
	else 
		return false;
}

bool button_is_held(Button b, int mouseX, int mouseY) {
	if (ui_ctx.locked && ui_ctx.focused != b.id) return false;

	if (pos_in_rect(b.x, b.y, b.w, b.h, mouseX, mouseY) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		ui_ctx.focused = b.id;
		ui_ctx.locked = true;
		return true;
	}
	else
		return false;
}

bool button_is_hovered(Button b, int mouseX, int mouseY) {

	if (pos_in_rect(b.x, b.y, b.w, b.h, mouseX, mouseY))
		return true;
	else
		return false;
}
