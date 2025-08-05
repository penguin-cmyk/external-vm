//
// Created by sigma on 02/08/2025.
//


#include "game.h"
#include "instance.h"

static int game::FindFirstChild(lua_State* L)
{
    auto* gameInstance = (Instance*)luaL_checkudata(L, 1, "Game");
    const char* childName = luaL_checkstring(L, 2);

    uintptr_t childAddr = gameInstance->FindFirstChild(childName);
    if (childAddr == 0) {
        lua_pushnil(L);
        return 1;
    }

    newInstance(childAddr)

    return 1;
}

static int game::FindFirstClass(lua_State* L)
{
    auto* gameInstance = (Instance*)luaL_checkudata(L, 1, "Game");
    const char* className = luaL_checkstring(L, 2);

    uintptr_t classAddr = gameInstance->FindFirstClass(className);
    if (classAddr == 0) {
        lua_pushnil(L);
        return 1;
    }

    newInstance(classAddr)
    return 1;
}

static int game::GetChildren(lua_State* L)
{
    auto* gameInstance = (Instance*)luaL_checkudata(L, 1, "Game");
    std::vector<uintptr_t> children = gameInstance->GetChildren(gameInstance->address);

    lua_newtable(L);

    for (size_t i = 0; i < children.size(); i++)
    {
        newInstance(children[i])
        lua_rawseti(L, -2, i + 1);
    }

    return 1;
}

static int game::GetDescendants(lua_State* L)
{
    auto* gameInstance = (Instance*)luaL_checkudata(L, 1, "Game");
    std::vector<uintptr_t> descendants = gameInstance->GetDescendants(gameInstance->address);

    lua_newtable(L);

    for (size_t i = 0; i < descendants.size(); i++)
    {
        newInstance(descendants[i])
        lua_rawseti(L, -2, i + 1);
    }

    return 1;
}


const std::unordered_map<std::string, lua_CFunction> methods = {
      {"FindFirstChild", game::FindFirstChild},
      {"FindFirstChildOfClass", game::FindFirstClass},
      {"GetService", game::FindFirstClass},
      {"GetChildren", game::GetChildren},
      {"GetDescendants", game::GetDescendants}
};

static int game::__index(lua_State* L)
{
    auto* gameInstance = (Instance*)(luaL_checkudata(L, 1, "Game"));
    std::string key = luaL_checkstring(L, 2);

    auto it = methods.find(key);

    if (it != methods.end()) {
        lua_pushcfunction(L, it->second);
        return 1;
    }


    if (key == "PlaceId") {
        lua_pushinteger(L, globals::PlaceId);
        return 1;
    }
    else if (key == "JobId") {
        lua_pushstring(L, globals::JobId.c_str());
        return 1;
    }

    else if (key == "Address") {
        lua_pushinteger(L, gameInstance->address);
        return 1;
    }

    else if (key == "Name") {
        std::string name = gameInstance->Name(gameInstance->address);
        lua_pushstring(L, name.c_str());
        return 1;
    }

    uintptr_t child = gameInstance->FindFirstChild(key);
    if (child) {
        newInstance(child)
        return 1;
    }

    return 1;
}

static int game::__gc(lua_State* L)
{
    auto* gameInstance = (Instance*)luaL_checkudata(L, 1, "Game");
    gameInstance->~Instance();
    return 0;
}

int luaopen_game(lua_State* L) {
    luaL_newmetatable(L, "Game");

    lua_pushcfunction(L, game::__index);
    lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, game::__gc);
    lua_setfield(L, -2, "__gc");

    auto* gameInstance = (Instance*)lua_newuserdata(L, sizeof(Instance));
    new(gameInstance) Instance(globals::DataModel);

    luaL_getmetatable(L, "Game");
    lua_setmetatable(L, -2);

    lua_setglobal(L, "game");

    return 0;
}

