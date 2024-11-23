
#include "../include/tigr.h"
#include <time.h>
#include "console.h"
#include "editor/editor.h"
#include "globals.h"
#include "luaapi.h"


int main() {

	Console c;
	console_new(&c, "editor.lua");
	luaapi_set_console(&c);

	Editor* e = editor_new(&c);

	/*console_run_global(&c);*/

	/*console_run_boot(&c);*/

	Tigr *screen = tigrWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Nano-Box", 0);
	c.tscreen = screen;
	
	clock_t t = clock();

	while (!tigrClosed(screen)) {
		/*console_run_update(&c);*/
		console_compose_frame(&c);
		editor_run(e);
		tigrUpdate(screen);
		if(tigrKeyDown(c.tscreen, TK_ESCAPE)) {
			break;	
		}
		c.time_elapsed = ((clock() - t)/(float)CLOCKS_PER_SEC);
	}
	tigrFree(screen);
	console_close(&c);
	editor_close(e);

	return 0;
}
