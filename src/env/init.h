//
// Created by sigma on 02/08/2025.
//

#ifndef EXTERNAL_LUA_INIT_H
#define EXTERNAL_LUA_INIT_H

#include "functions/rbx/game.h"
#include "functions/rbx/instance.h"
#include "functions/misc/cframe.h"
#include "functions/misc/vector3.h"

#include "thread"

// Yes this wait is not ideal but since we don't have any thread related functions this is ok
int wait(lua_State* L) {
    const auto time = luaL_optnumber(L, 1, 0);
    Sleep(time * 1000);

    lua_pushinteger(L, time);
    return 1;
}

void register_everything(lua_State* L) {
    luaopen_game(L);
    luaopen_instance(L);
    register_vector3(L);
    register_cframe(L);

    lua_pushcclosure(L, wait, 0);
    lua_setglobal(L, "wait");

}


#endif //EXTERNAL_LUA_INIT_H
