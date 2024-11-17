#include "luaapi.h"
#include "api.h"
#include "console.h"
#include <lauxlib.h>
#include <lua.h>

// IDK about this, but whatever
Conosole *console;
void set_console(Conosole *c) {
	console = c;
}

// Core Functions --------------------------------
int _time(lua_State *L) {
	lua_pushnumber(L, GetTime()*1000);
	return 1;
}

// Memory Functions --------------------------------
int peek(lua_State *L) {
	int addr = lua_tointeger(L, 1);
	lua_pushinteger(L, Peek(&console->ram, addr));
	return 1;
}
int peek2(lua_State *L) {
	int addr = luaL_checkinteger(L, 1);
	lua_pushinteger(L, Peek2(&console->ram, addr));
	return 1;
}
int peek4(lua_State *L) {
	int addr = luaL_checkinteger(L, 1);
	lua_pushinteger(L, Peek4(&console->ram, addr));
	return 1;
}

int poke(lua_State *L) {
	int addr = luaL_checkinteger(L, 1);
	int val = luaL_checkinteger(L, 2);
	Poke(&console->ram, addr, val);
	return 0;
}
int poke2(lua_State *L) {
	int addr = luaL_checkinteger(L, 1);
	int val = luaL_checkinteger(L, 2);
	Poke2(&console->ram, addr, val);
	return 0;
}

// Drawing Functions --------------------------------
int cls(lua_State *L) {
	int c = luaL_checkinteger(L, 1);
	Cls(console, c);
	return 0;
}

int pxset(lua_State *L) {
	int x = luaL_checkinteger(L, 1);
	int y = luaL_checkinteger(L, 2);
	int c = luaL_checkinteger(L, 3);
	PxSet(console, x, y, c);
	return 0;
}

int pxget(lua_State *L) {
	int x = luaL_checkinteger(L, 1);
	int y = luaL_checkinteger(L, 2);
	lua_pushinteger(L, PxGet(console, x, y));
	return 1;
}

Func funcs[] = {
	{"time", _time},

	{"peek", peek},
	{"peek2", peek2},
	{"peek4", peek4},
	{"poke", poke},
	{"poke2", poke2},

	{"cls", cls},
	{"pxset", pxset},
	{"pxget", pxget},
};

void register_lua_api(lua_State *L) {
	int len = sizeof(funcs)/sizeof(Func);
	for(int i = 0; i < len; i++) {
		lua_pushcfunction(L, funcs[i].func);
		lua_setglobal(L, funcs[i].name);
	}
}
