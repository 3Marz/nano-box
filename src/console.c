
#include "console.h"
#include "luaapi.h"
#include "globals.h"
#include "ram.h"
#include "utils.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

void console_new(Console *c) {
	ram_init(&c->ram);
	c->L = luaL_newstate();
	if (c->L == NULL) {
		fprintf(stderr, "Lua: Could not create state\n");
		return;
	}
	luaL_openlibs(c->L);
	c->mode = CONSOLE_MODE_EDITOR;
}

void console_load_string(Console *c, char *str) {
	int err = luaL_loadstring(c->L, str);
	if (err) {
		fprintf(stderr, "Lua: Couldn't Load String: %s\n", lua_tostring(c->L, -1));
		return;
	}
	// Register Api
	register_lua_api(c->L);
}

void console_run_global(Console *c) {
	if(lua_pcall(c->L, 0, LUA_MULTRET, 0) != 0) {
		fprintf(stderr, "Lua: %s\n", lua_tostring(c->L, -1));
		return;
	}
}

void console_run_boot(Console *c) {
	lua_getglobal(c->L, "_boot");
	if(lua_pcall(c->L, 0, 0, 0) != 0) {
		fprintf(stderr, "Lua: %s\n", lua_tostring(c->L, -1));
		return;
	}
}

void console_run_update(Console *c) {
	lua_getglobal(c->L, "_update");
	if(lua_pcall(c->L, 0, 0, 0) != 0) {
		fprintf(stderr, "Lua: %s\n", lua_tostring(c->L, -1));
		return;
	}
}

void console_run_draw(Console *c) {
	lua_getglobal(c->L, "_draw");
	if(lua_pcall(c->L, 0, 0, 0) != 0) {
		fprintf(stderr, "Lua: %s\n", lua_tostring(c->L, -1));
		return;
	}
}

void console_compose_frame(Console *console) {
	int y = 0;
	for(int i = 0; i < 0x3000; i++) {
		uint8_t colors = Peek(&console->ram, i);
		TPixel c1 = color_from_palette(&console->ram, colors >> 4);
		TPixel c2 = color_from_palette(&console->ram, colors & 0xF);
		int x = i * 2;
		tigrPlot(console->tscreen, x%SCREEN_WIDTH, y, c1);
		tigrPlot(console->tscreen, (x%SCREEN_WIDTH)+1, y, c2);
		if(i%(SCREEN_WIDTH/2) == (SCREEN_WIDTH/2)-1) {
			y++;
		}
	}
}

void console_close(Console *console) {
	lua_close(console->L);
}


