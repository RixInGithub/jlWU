#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "jorkdir/jorkdir.h"
#include <stdlib.h>
#include <sys/stat.h> // works on msys gcc and regular gcc, dw
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
	#include <windows.h>
#else
	#include <linux/limits.h> // this is for PATH_MAX btw
	#define MAX_PATH PATH_MAX
#endif

// |require("jlWU")(string dir) => table{string}|
static int /*mT.*/__call(lua_State*L) {
	int t = lua_type(L,2); // right, i forgot that you call __call with self THEN args...
	bool nil = ((t==LUA_TNONE)||(t==LUA_TNIL));
	if (!((nil) || (t==LUA_TSTRING))) {
		lua_pushstring(L, "directory must be a string or nil (to default to current dir)"); // why cant it just be like |error(string err)|...
		return lua_error(L);
	}
	char*a = ".";
	if (!(nil)) a = luaL_optstring(L,2,a); // i made it optstring bcuz you can never be too careful™
	int sz;
	int count = 0;
	char**res = jorkdir(a,&sz); // jork out the ℻
	lua_createtable(L,sz,0); // me when optimization impending!!! 🔥🔥🔥🔥
	int tblStkIdx = lua_gettop(L);
	while (count < sz) {
		lua_pushstring(L,res[count]);
		lua_rawseti(L,tblStkIdx,++count);
		free(res[count-1]);
	}
	free(res);
	return 1;
}

// |require("jlWU").fullPath(string path) => string|
static int fullPath(lua_State*L) {
	int t = lua_type(L,1);
	bool nil = ((t==LUA_TNONE)||(t==LUA_TNIL));
	if (!((nil) || (t==LUA_TSTRING))) {
		lua_pushstring(L, "directory must be a string or nil (to default to current dir)");
		return lua_error(L);
	}
	char*a = ".";
	if (!(nil)) a = luaL_optstring(L,1,a);
	char res[MAX_PATH] = {0};
	#ifdef _WIN32
		GetFullPathNameA(a, MAX_PATH, res, NULL);
	#else
		realpath(a, res);
	#endif
	lua_pushstring(L,res);
	return 1;
}

// |require("jlWU").isDir(string path) => boolean|
static int isDir(lua_State*L) {
	int t = lua_type(L,1);
	if (!(t==LUA_TSTRING)) {
		lua_pushstring(L, "path must be string");
		return lua_error(L);
	}
	fullPath(L); // hmm yes, very dry. (get it??)
	char*p = luaL_checkstring(L, lua_gettop(L));
	lua_pop(L, 1);
	// yanked from hjx.c
	struct stat*s=malloc(sizeof(struct stat));
	memset(s,0,sizeof(struct stat));
	stat(p,s);
	bool res=S_ISDIR(s->st_mode)!=0;
	free(s);
	// yanked from hjx.c
	lua_pushboolean(L, res);
	return 1;
}

int luaopen_jlWU(lua_State*L) {
	lua_newtable(L);
	int _M = lua_gettop(L);
	lua_createtable(L,0,1); // one key/value pair please!
	int mT = lua_gettop(L);
	lua_pushcfunction(L, __call);
	lua_setfield(L, mT, "__call");
	lua_setmetatable(L, _M); // mT is now officially garbage data!!!!!! 🔥🔥🔥🔥
	lua_pushcfunction(L, fullPath);
	lua_setfield(L, _M, "fullPath");
	lua_pushcfunction(L, isDir);
	lua_setfield(L, _M, "isDir");
	return 1;
}