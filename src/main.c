
#include "../include/tigr.h"
#include <time.h>
#include "console.h"
#include "editor/editor.h"
#include "globals.h"
#include "luaapi.h"


int main() {

	Console console;
	console_new(&console);
	luaapi_set_console(&console);

	Editor editor;
	editor_new(&editor, &console);

	Tigr *screen = tigrWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Nano-Box", 0);
	console.tscreen = screen;
	
	clock_t t = clock();

	while (!tigrClosed(screen)) {

		if (console.mode == CONSOLE_MODE_EDITOR) {
			console_compose_frame(&console, &editor.ram);
			editor_run(&editor);
		}
		else if (console.mode == CONSOLE_MODE_GAME) {
			console_compose_frame(&console, &console.ram);
			console_run_update(&console);
		}

		tigrUpdate(screen);

		// Shortcuts
		if(tigrKeyDown(console.tscreen, TK_ESCAPE)) {
			if(console.mode == CONSOLE_MODE_GAME)
				console.mode = CONSOLE_MODE_EDITOR;
			else
				break;
		}
		if(tigrKeyHeld(console.tscreen, TK_CONTROL) && tigrKeyDown(console.tscreen, 'R') && console.mode == CONSOLE_MODE_EDITOR) {
			sds code_string = sdsjoin(editor.code->data, editor.code->len, "\n");
			console_load_string(&console, code_string);
			console_run_global(&console);
			console_run_boot(&console);
			console.mode = CONSOLE_MODE_GAME;
		}

		console.time_elapsed = ((clock() - t)/(float)CLOCKS_PER_SEC);
	}
	tigrFree(screen);
	console_close(&console);
	editor_close(&editor);

	return 0;
}
