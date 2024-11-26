#include "luaapi.h"
#include "api.h"
#include "console.h"
#include "ram.h"
#include <lauxlib.h>
#include <lua.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

// IDK about this, but whatever
Console *console;
void luaapi_set_console(Console *c) {
	console = c;
}

// Core Functions --------------------------------
int _time(lua_State *L) {
	lua_pushnumber(L, console->time_elapsed);
	return 1;
}
int getkeys(lua_State *L) { // Cheating TODO
	GetKeys(console);
	lua_pushinteger(L, Peek(&console->ram, 0x526D));
	return 1;
}
int mouse(lua_State *L) { // Cheating TODO
	Mouse(console);
	lua_pushinteger(L, Peek(&console->ram, 0x526A));
	lua_pushinteger(L, Peek(&console->ram, 0x526B));
	lua_pushinteger(L, Peek(&console->ram, 0x526C));
	return 3;
}
int btn(lua_State *L) {
	Btn(console);
	int b = luaL_checkinteger(L, 1);
	int byte = Peek(&console->ram, 0x526F);
	int bit = byte >> (7-b) & 0b1;
	if(bit == 1) {
		lua_pushboolean(L, 1);
	} else {
		lua_pushboolean(L, 0);
	}
	return 1;
}
int btnp(lua_State *L) {
	BtnP(console);
	int b = luaL_checkinteger(L, 1);
	int byte = Peek(&console->ram, 0x526E);
	int bit = byte >> (7-b) & 0b1;
	if(bit == 1) {
		lua_pushboolean(L, 1);
	} else {
		lua_pushboolean(L, 0);
	}
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
	Cls(&console->ram, c);
	return 0;
}

int pxset(lua_State *L) {
	int x = luaL_checknumber(L, 1);
	int y = luaL_checknumber(L, 2);
	int c = luaL_checkinteger(L, 3);
	PxSet(&console->ram, floor(x), floor(y), c);
	return 0;
}

int pxget(lua_State *L) {
	int x = luaL_checkinteger(L, 1);
	int y = luaL_checkinteger(L, 2);
	lua_pushinteger(L, PxGet(&console->ram, x, y));
	return 1;
}

int line(lua_State *L) {
	int x1 = luaL_checknumber(L, 1);
	int y1 = luaL_checknumber(L, 2);
	int x2 = luaL_checknumber(L, 3);
	int y2 = luaL_checknumber(L, 4);
	int c = luaL_checkinteger(L, 5);
	Line(&console->ram, x1, y1, x2, y2, c);
	return 0;
}

int text(lua_State *L) {
	int x = luaL_checkinteger(L, 1);
	int y = luaL_checkinteger(L, 2);
	char *s = luaL_checkstring(L, 3);
	int c = luaL_checkinteger(L, 4);
	Text(&console->ram, x, y, s, c);
	return 0;
}

int rect(lua_State *L) {
	int x = luaL_checknumber(L, 1);
	int y = luaL_checknumber(L, 2);
	int w = luaL_checknumber(L, 3);
	int h = luaL_checknumber(L, 4);
	int c = luaL_checkinteger(L, 5);
	Rect(&console->ram, x, y, w, h, c);
	return 0;
}

int rectf(lua_State *L) {
	int x = luaL_checknumber(L, 1);
	int y = luaL_checknumber(L, 2);
	int w = luaL_checknumber(L, 3);
	int h = luaL_checknumber(L, 4);
	int c = luaL_checkinteger(L, 5);
	RectF(&console->ram, x, y, w, h, c);
	return 0;
}

int circ(lua_State *L) {
	int x = luaL_checknumber(L, 1);
	int y = luaL_checknumber(L, 2);
	int r = luaL_checknumber(L, 3);
	int c = luaL_checkinteger(L, 4);
	Circ(&console->ram, x, y, r, c);
	return 0;
}
int circf(lua_State *L) {
	int x = luaL_checknumber(L, 1);
	int y = luaL_checknumber(L, 2);
	int r = luaL_checknumber(L, 3);
	int c = luaL_checkinteger(L, 4);
	CircF(&console->ram, x, y, r, c);
	return 0;
}

int spr(lua_State *L) {
	int id = luaL_checkinteger(L, 1);
	int x = luaL_checknumber(L, 2);
	int y = luaL_checknumber(L, 3);
	int w = luaL_checkinteger(L, 4);
	int h = luaL_checkinteger(L, 5);
	Spr(&console->ram, id, x, y, w, h);
	return 0;
}

Func funcs[] = {
	{"time", _time},
	{"getkeys", getkeys},
	{"mouse", mouse},
	{"btnp", btnp},
	{"btn", btn},

	{"peek", peek},
	{"peek2", peek2},
	{"peek4", peek4},
	{"poke", poke},
	{"poke2", poke2},

	{"cls", cls},
	{"pxset", pxset},
	{"pxget", pxget},
	{"line", line},
	{"text", text},
	{"rect", rect},
	{"rectf", rectf},
	{"circ", circ},
	{"circf", circf},
	{"spr", spr},
};

void register_lua_api(lua_State *L) {
	int len = sizeof(funcs)/sizeof(Func);
	for(int i = 0; i < len; i++) {
		lua_pushcfunction(L, funcs[i].func);
		lua_setglobal(L, funcs[i].name);
	}
}
