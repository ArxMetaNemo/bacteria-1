#pragma once
#define luaL_reg      luaL_Reg
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include<dirent.h>

//typedef lua_State *L ...;
#define INITLUAFUNC(name) int lua_##name (lua_State *L)
#define LUAPAIR(name) {"" #name , lua_##name },


void runAllLuaFilesInDir(lua_State * L, const char * pathdir);
