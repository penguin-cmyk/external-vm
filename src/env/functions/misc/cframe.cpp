//
// Created by sigma on 05/08/2025.
//
//
// Created by sigma on 02/08/2025.
//

#include "cframe.h"
const char* CFRAME_METATABLE = "CFrame";

CFrame* check_cframe(lua_State* L, int index) {
    return (CFrame*)luaL_checkudata(L, index, CFRAME_METATABLE);
}

void push_cframe(lua_State* L, const CFrame& cf) {
    CFrame* frame = (CFrame*)lua_newuserdata(L, sizeof(CFrame));
    *frame = cf;
    luaL_getmetatable(L, CFRAME_METATABLE);
    lua_setmetatable(L, -2);
}

int cframe_new(lua_State* L) {
    int argc = lua_gettop(L);

    if (argc == 0) {
        push_cframe(L, CFrame());
    }
    else if (argc == 3) {
        float x = (float)luaL_checknumber(L, 1);
        float y = (float)luaL_checknumber(L, 2);
        float z = (float)luaL_checknumber(L, 3);
        push_cframe(L, CFrame(x, y, z));
    }
    else if (argc == 12) {
        float x = (float)luaL_checknumber(L, 1);
        float y = (float)luaL_checknumber(L, 2);
        float z = (float)luaL_checknumber(L, 3);
        float r00 = (float)luaL_checknumber(L, 4);
        float r01 = (float)luaL_checknumber(L, 5);
        float r02 = (float)luaL_checknumber(L, 6);
        float r10 = (float)luaL_checknumber(L, 7);
        float r11 = (float)luaL_checknumber(L, 8);
        float r12 = (float)luaL_checknumber(L, 9);
        float r20 = (float)luaL_checknumber(L, 10);
        float r21 = (float)luaL_checknumber(L, 11);
        float r22 = (float)luaL_checknumber(L, 12);
        push_cframe(L, CFrame(x, y, z, r00, r01, r02, r10, r11, r12, r20, r21, r22));
    }
    else {
        return luaL_error(L, "Invalid arguments for CFrame constructor");
    }

    return 1;
}

int cframe_mul(lua_State* L) {
    CFrame* a = check_cframe(L, 1);
    CFrame* b = check_cframe(L, 2);

    CFrame result = *a * *b;
    push_cframe(L, result);
    return 1;
}

int cframe_tostring(lua_State* L) {
    CFrame* cf = check_cframe(L, 1);

    char buffer[256];
    sprintf(buffer, "CFrame(%.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f)",
            cf->m[12], cf->m[13], cf->m[14],
            cf->m[0], cf->m[4], cf->m[8],
            cf->m[1], cf->m[5], cf->m[9],
            cf->m[2], cf->m[6], cf->m[10]);
    lua_pushstring(L, buffer);
    return 1;
}

int cframe_index(lua_State* L) {
    CFrame* cf = check_cframe(L, 1);
    std::string key = luaL_checkstring(L, 2);

    if (key == "x" || key == "X") {
        lua_pushnumber(L, cf->m[12]);
        return 1;
    }
    else if (key == "y" || key == "Y") {
        lua_pushnumber(L, cf->m[13]);
        return 1;
    }
    else if (key == "z" || key == "Z") {
        lua_pushnumber(L, cf->m[14]);
        return 1;
    }
    else if (key == "position" || key == "Position") {
        Vector3 pos = cf->getPosition();
        push_vector3(L, pos.x, pos.y, pos.z);
        return 1;
    }
    else if (key == "lookVector" || key == "LookVector") {
        Vector3 look = cf->getLookVector();
        push_vector3(L, look.x, look.y, look.z);
        return 1;
    }
    else if (key == "rightVector" || key == "RightVector") {
        Vector3 right = cf->getRightVector();
        push_vector3(L, right.x, right.y, right.z);
        return 1;
    }
    else if (key == "upVector" || key == "UpVector") {
        Vector3 up = cf->getUpVector();
        push_vector3(L, up.x, up.y, up.z);
        return 1;
    }

    luaL_getmetatable(L, CFRAME_METATABLE);
    lua_getfield(L, -1, key.c_str());
    return 1;
}

int cframe_inverse(lua_State* L) {
    CFrame* cf = check_cframe(L, 1);
    CFrame result = cf->inverse();
    push_cframe(L, result);
    return 1;
}

int cframe_lookAt(lua_State* L) {
    Vector3* eye = check_vector3(L, 1);
    Vector3* target = check_vector3(L, 2);
    Vector3 up(0, 1, 0);

    if (lua_gettop(L) >= 3) {
        Vector3* upVec = check_vector3(L, 3);
        up = *upVec;
    }

    CFrame result = CFrame::lookAt(*eye, *target, up);
    push_cframe(L, result);
    return 1;
}

int cframe_fromEulerAnglesXYZ(lua_State* L) {
    float x = (float)luaL_checknumber(L, 1);
    float y = (float)luaL_checknumber(L, 2);
    float z = (float)luaL_checknumber(L, 3);

    CFrame result = CFrame::fromEulerAnglesXYZ(x, y, z);
    push_cframe(L, result);
    return 1;
}

void register_cframe(lua_State* L) {
    luaL_newmetatable(L, CFRAME_METATABLE);

    lua_pushcfunction(L, cframe_mul);
    lua_setfield(L, -2, "__mul");

    lua_pushcfunction(L, cframe_tostring);
    lua_setfield(L, -2, "__tostring");

    lua_pushcfunction(L, cframe_index);
    lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, cframe_inverse);
    lua_setfield(L, -2, "inverse");

    lua_pop(L, 1);

    lua_newtable(L);
    lua_pushcfunction(L, cframe_new);
    lua_setfield(L, -2, "new");

    lua_pushcfunction(L, cframe_lookAt);
    lua_setfield(L, -2, "lookAt");

    lua_pushcfunction(L, cframe_fromEulerAnglesXYZ);
    lua_setfield(L, -2, "fromEulerAnglesXYZ");

    push_cframe(L, CFrame());
    lua_setfield(L, -2, "identity");

    lua_setglobal(L, "CFrame");
}