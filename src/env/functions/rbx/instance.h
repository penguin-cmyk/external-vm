//
// Created by sigma on 02/08/2025.
//

#ifndef EXTERNAL_LUA_INSTANCE_H
#define EXTERNAL_LUA_INSTANCE_H

#include "../../definitons.h"
#include "../../../ctx/offsets.h"
#include "../../../ctx/globals.h"
#include "../../../rbx/instance.hpp"


namespace instance {
    int __newindex(lua_State*L);
    int __index(lua_State*L);
    int __gc(lua_State*L);

    int FindFirstChild(lua_State*L);
    int FindFirstClass(lua_State*L);
    int GetDescendants(lua_State*L);
    int GetChildren(lua_State*L);
    int IsA(lua_State*L);
    int GetFullName(lua_State*L);
    int WaitForChild(lua_State* L);
    int GetAttributes(lua_State* L);
    int GetAttribute(lua_State* L);
}


int luaopen_instance(lua_State* L);
#endif //EXTERNAL_LUA_INSTANCE_H
