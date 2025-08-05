//
// Created by sigma on 02/08/2025.
//

#ifndef EXTERNAL_LUA_VECTOR3_H
#define EXTERNAL_LUA_VECTOR3_H

#include "../../classes/vector3.hpp"
#include "../../definitons.h"

void register_vector3(lua_State* L);
Vector3* check_vector3(lua_State* L, int index);
void push_vector3(lua_State* L, float x, float y, float z);

#endif //EXTERNAL_LUA_VECTOR3_H
