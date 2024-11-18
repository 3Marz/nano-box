
#include "console.h"
#include "globals.h"
#include "ram.h"
#include "utils.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <raylib.h>

void console_new(Console *c, char *luafile) {
	ram_init(&c->ram);
	c->L = luaL_newstate();
	if (c->L == NULL) {
		fprintf(stderr, "Lua: Could not create state\n");
		return;
	}
	luaL_openlibs(c->L);

	int err = luaL_loadfile(c->L, luafile);
	if (err) {
		fprintf(stderr, "Lua: Couldn't Load File: %s\n", lua_tostring(c->L, -1));
		return;
	}
	// Register Api
	/*register_lua_api(c->L);*/

	// Run Global Scope
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
	for(int y = 0; y < SCREEN_HEIGHT; y++) {
		for(int x = 0; x < SCREEN_WIDTH/2; x++) {
			uint8_t colors = Peek(&console->ram, x + (y * (SCREEN_WIDTH/2)));

			uint8_t pixelIndex1 = colors >> 4;
			uint8_t pixelIndex2 = colors & 0xF;
			
			Color c1 = color_from_palette(&console->ram, pixelIndex1);
			Color c2 = color_from_palette(&console->ram, pixelIndex2);

			DrawPixel(x*2 - 1, y, c1);
			DrawPixel(x*2, y, c2);
		}
	}
}

void console_close(Console *console) {
	lua_close(console->L);
}


