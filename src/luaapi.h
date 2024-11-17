#ifndef LUAAPI_H
#define LUAAPI_H

#include "console.h"
#include <lua.h>

typedef struct {
	const char *name;
	int (*func)(lua_State *L);
} Func;

void set_console(Conosole *c);

// Memory Functions
int peek(lua_State *L);
int peek2(lua_State *L);
int peek4(lua_State *L);

int poke(lua_State *L);
int poke2(lua_State *L);

// Drawing Functions
int cls(lua_State *L);
int pxset(lua_State *L);
int pxget(lua_State *L);

void register_lua_api(lua_State *L);

#endif
