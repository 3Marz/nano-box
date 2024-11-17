#ifndef CONSOLE_H
#define CONSOLE_H

#include <lua.h>
#include "ram.h"

typedef struct Conosole {
	Ram ram;
	lua_State *L;
} Conosole;

void console_new(Conosole* console, char* luafile);

void console_run_boot(Conosole* console);
void console_run_update(Conosole* console);
void console_run_draw(Conosole* console);

void console_compose_frame(Conosole* console);

void console_close(Conosole *console);

#endif
