#pragma once
#include "lua.h"
#include"cryptocoins.h"
#include"json_rpc.h"
int lua_rpc_requst(lua_State * L);

static const struct luaL_reg rpclib [] = {
      //{"example", example},
      {"request", lua_rpc_requst},
      //{"size", luagetcount_cryptocoins},
      {NULL, NULL}
};
int luaopen_rpc (lua_State *L);
