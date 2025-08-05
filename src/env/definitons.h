//
// Created by sigma on 02/08/2025.
//

#ifndef EXTERNAL_LUA_DEFINITIONS_H
#define EXTERNAL_LUA_DEFINITIONS_H

#include <iostream>
#include <unordered_map>

extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
};

#define newInstance(addr) \
    auto* childInstance = (Instance*)lua_newuserdata(L, sizeof(Instance));\
    new(childInstance) Instance(addr);                                     \
    luaL_getmetatable(L, "Instance");                                       \
    lua_setmetatable(L, -2);                                                 \


#endif //EXTERNAL_LUA_DEFINITIONS_H
