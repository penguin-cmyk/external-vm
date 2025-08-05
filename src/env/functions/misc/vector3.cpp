//
// Created by sigma on 02/08/2025.
//
#include "vector3.h"
const char* VECTOR3_METATABLE = "Vector3";

Vector3* check_vector3(lua_State* L, int index) {
    return (Vector3*)luaL_checkudata(L, index, VECTOR3_METATABLE);
}

void push_vector3(lua_State* L, float x, float y, float z) {
    Vector3* vec = (Vector3*)lua_newuserdata(L, sizeof(Vector3));
    *vec = Vector3(x, y, z);
    luaL_getmetatable(L, VECTOR3_METATABLE);
    lua_setmetatable(L, -2);
}

int vector3_new(lua_State* L) {
    float x = (float)luaL_optnumber(L, 1, 0.0);
    float y = (float)luaL_optnumber(L, 2, 0.0);
    float z = (float)luaL_optnumber(L, 3, 0.0);

    push_vector3(L, x, y, z);
    return 1;
}

int vector3_add(lua_State* L) {
    Vector3* a = check_vector3(L, 1);
    Vector3* b = check_vector3(L, 2);

    Vector3 result = *a + *b;
    push_vector3(L, result.x, result.y, result.z);
    return 1;
}

int vector3_sub(lua_State* L) {
    Vector3* a = check_vector3(L, 1);
    Vector3* b = check_vector3(L, 2);

    Vector3 result = *a - *b;
    push_vector3(L, result.x, result.y, result.z);
    return 1;
}

int vector3_mul(lua_State* L) {
    Vector3* vec = nullptr;
    float scalar = 0;

    if (lua_isuserdata(L, 1)) {
        vec = check_vector3(L, 1);
        scalar = (float)luaL_checknumber(L, 2);
    }
    else if (lua_isuserdata(L, 2)) {
        scalar = (float)luaL_checknumber(L, 1);
        vec = check_vector3(L, 2);
    }
    else {
        return luaL_error(L, "Invalid arguments for Vector3 multiplication");
    }

    Vector3 result = *vec * scalar;
    push_vector3(L, result.x, result.y, result.z);
    return 1;
}

int vector3_div(lua_State* L) {
    Vector3* vec = check_vector3(L, 1);
    float scalar = (float)luaL_checknumber(L, 2);

    if (scalar == 0) {
        return luaL_error(L, "Division by zero");
    }

    Vector3 result = *vec / scalar;
    push_vector3(L, result.x, result.y, result.z);
    return 1;
}

int vector3_tostring(lua_State* L) {
    Vector3* vec = check_vector3(L, 1);

    char buffer[64];
    sprintf(buffer, "Vector3(%.3f, %.3f, %.3f)", vec->x, vec->y, vec->z);
    lua_pushstring(L, buffer);
    return 1;
}

int vector3_index(lua_State* L) {
    Vector3* vec = check_vector3(L, 1);
    std::string key = luaL_checkstring(L, 2);

    if (key == "x" || key == "X") {
        lua_pushnumber(L, vec->x);
        return 1;
    }
    else if (key == "y" || key == "Y") {
        lua_pushnumber(L, vec->y);
        return 1;
    }
    else if (key == "z"  || key == "Z") {
        lua_pushnumber(L, vec->z);
        return 1;
    }
    else if (key == "magnitude") {
        lua_pushnumber(L, vec->magnitude());
        return 1;
    }

    luaL_getmetatable(L, VECTOR3_METATABLE);
    lua_getfield(L, -1, key.c_str());
    return 1;
}

int vector3_newindex(lua_State* L) {
    Vector3* vec = check_vector3(L, 1);
    std::string key = luaL_checkstring(L, 2);
    float value = (float)luaL_checknumber(L, 3);

    if (key == "x" || key == "X") {
        vec->x = value;
    }
    else if (key == "y" || key == "Y") {
        vec->y = value;
    }
    else if (key == "z"  || key == "Z") {
        vec->z = value;
    }
    else {
        return luaL_error(L, "Cannot set property '%s' on Vector3", key.c_str());
    }

    return 0;
}

int vector3_dot(lua_State* L) {
    Vector3* a = check_vector3(L, 1);
    Vector3* b = check_vector3(L, 2);

    lua_pushnumber(L, a->dot(*b));
    return 1;
}

int vector3_cross(lua_State* L) {
    Vector3* a = check_vector3(L, 1);
    Vector3* b = check_vector3(L, 2);

    Vector3 result = a->cross(*b);
    push_vector3(L, result.x, result.y, result.z);
    return 1;
}

int vector3_magnitude(lua_State* L) {
    Vector3* vec = check_vector3(L, 1);
    lua_pushnumber(L, vec->magnitude());
    return 1;
}

int vector3_normalize(lua_State* L) {
    Vector3* vec = check_vector3(L, 1);
    Vector3 result = vec->normalize();
    push_vector3(L, result.x, result.y, result.z);
    return 1;
}

void register_vector3(lua_State* L) {
    luaL_newmetatable(L, VECTOR3_METATABLE);

    lua_pushcfunction(L, vector3_add);
    lua_setfield(L, -2, "__add");

    lua_pushcfunction(L, vector3_sub);
    lua_setfield(L, -2, "__sub");

    lua_pushcfunction(L, vector3_mul);
    lua_setfield(L, -2, "__mul");

    lua_pushcfunction(L, vector3_div);
    lua_setfield(L, -2, "__div");

    lua_pushcfunction(L, vector3_tostring);
    lua_setfield(L, -2, "__tostring");

    lua_pushcfunction(L, vector3_index);
    lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, vector3_newindex);
    lua_setfield(L, -2, "__newindex");

    lua_pushcfunction(L, vector3_dot);
    lua_setfield(L, -2, "dot");

    lua_pushcfunction(L, vector3_cross);
    lua_setfield(L, -2, "cross");

    lua_pushcfunction(L, vector3_normalize);
    lua_setfield(L, -2, "normalize");



    lua_pop(L, 1);

    lua_newtable(L);
    lua_pushcfunction(L, vector3_new);
    lua_setfield(L, -2, "new");

    push_vector3(L, 0, 0, 0);
    lua_setfield(L, -2, "zero");


    lua_setglobal(L, "Vector3");
}
