#ifndef CONSOLE_H
#define CONSOLE_H

#include <lua.h>
#include "ram.h"
#include "../include/tigr.h"

typedef struct Console {
	Ram ram;
	lua_State *L;
	Tigr *tscreen;
	double time_elapsed;
} Console;

void console_new(Console* console, char* luafile);

void console_run_boot(Console* console);
void console_run_update(Console* console);
void console_run_draw(Console* console);

void console_compose_frame(Console* console);

void console_close(Console *console);

#endif
