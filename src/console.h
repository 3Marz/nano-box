#ifndef CONSOLE_H
#define CONSOLE_H

#include <lua.h>
#include "ram.h"

enum ConsoleMode {
	CONSOLE_MODE_TERMINAL,
	CONSOLE_MODE_EDITOR,
	CONSOLE_MODE_GAME
};

typedef struct Console {
	Ram ram;
	lua_State *L;
	enum ConsoleMode mode;
} Console;

void console_new(Console* console);
void console_load_string(Console *c, char *str);

void console_run_global(Console *c);
void console_run_boot(Console* console);
void console_run_update(Console* console);
void console_run_draw(Console* console);

void console_compose_frame(Ram* ram);

void console_close(Console *console);

#endif
