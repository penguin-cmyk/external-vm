//
// Created by sigma on 02/08/2025.
//

#ifndef EXTERNAL_LUA_GAME_H
#define EXTERNAL_LUA_GAME_H

#include "../../definitons.h"

int luaopen_game(lua_State* L);
static int game_index(lua_State* L);

namespace game {
    int FindFirstChild(lua_State* L);
    int FindFirstClass(lua_State* L);
    int GetChildren(lua_State* L);
    int GetDescendants(lua_State* L);
    int __index(lua_State* L);
    int __gc(lua_State* L);
}

#endif //EXTERNAL_LUA_GAME_H
