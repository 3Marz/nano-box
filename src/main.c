
#include "../include/tigr.h"
#include <time.h>
#include "console.h"
#include "editor/editor.h"
#include "globals.h"
#include "luaapi.h"


int main() {

	Console c;
	console_new(&c);
	luaapi_set_console(&c);

	Editor* e = editor_new(&c);

	Tigr *screen = tigrWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Nano-Box", 0);
	c.tscreen = screen;
	
	clock_t t = clock();

	while (!tigrClosed(screen)) {
		console_compose_frame(&c);

		if (c.mode == CONSOLE_MODE_EDITOR)
			editor_run(e);
		else if (c.mode == CONSOLE_MODE_GAME)
			console_run_update(&c);

		tigrUpdate(screen);

		// Shortcuts
		if(tigrKeyDown(c.tscreen, TK_ESCAPE)) {
			if(c.mode == CONSOLE_MODE_GAME)
				c.mode = CONSOLE_MODE_EDITOR;
			else
				break;
		}
		if(tigrKeyHeld(c.tscreen, TK_CONTROL) && tigrKeyDown(c.tscreen, 'R') && c.mode == CONSOLE_MODE_EDITOR) {
			sds code_string = sdsjoin(e->code->data, e->code->len, "\n");
			console_load_string(&c, code_string);
			console_run_global(&c);
			console_run_boot(&c);
			c.mode = CONSOLE_MODE_GAME;
		}

		c.time_elapsed = ((clock() - t)/(float)CLOCKS_PER_SEC);
	}
	tigrFree(screen);
	console_close(&c);
	editor_close(e);

	return 0;
}
