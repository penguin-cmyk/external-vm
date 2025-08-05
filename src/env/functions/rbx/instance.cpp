//
// Created by sigma on 02/08/2025.
//


#include "instance.h"
#include "../../utlities/properties.hpp"

static int instance::FindFirstChild(lua_State* L)
{
    auto* instance = (Instance*)luaL_checkudata(L, 1, "Instance");
    const char* childName = luaL_checkstring(L, 2);

    uintptr_t childAddr = instance->FindFirstChild(childName);
    if (childAddr == 0) {
        lua_pushnil(L);
        return 1;
    }

    newInstance(childAddr)
    return 1;
}

static int instance::FindFirstClass(lua_State* L)
{
    auto* instance = (Instance*)luaL_checkudata(L, 1, "Instance");
    const char* className = luaL_checkstring(L, 2);

    uintptr_t classAddr = instance->FindFirstClass(className);
    if (classAddr == 0) {
        lua_pushnil(L);
        return 1;
    }

    newInstance(classAddr)

    return 1;
}

static int instance::GetChildren(lua_State* L)
{
    auto* instance = (Instance*)luaL_checkudata(L, 1, "Instance");
    std::vector<uintptr_t> children = instance->GetChildren(instance->address);

    lua_newtable(L);
    for (size_t i = 0; i < children.size(); i++)
    {
        newInstance(children[i])
        lua_rawseti(L, -2, i + 1);
    }

    return 1;
}

static int instance::GetDescendants(lua_State* L)
{
    auto* instance = (Instance*)luaL_checkudata(L, 1, "Instance");
    std::vector<uintptr_t> descendants = instance->GetDescendants(instance->address);

    lua_newtable(L);

    for (size_t i = 0; i < descendants.size(); i++)
    {
        newInstance(descendants[i])
        lua_rawseti(L, -2, i + 1);
    }

    return 1;
}

static int instance::IsA(lua_State* L)
{
    auto* instance = (Instance*)luaL_checkudata(L, 1, "Instance");
    std::string className = luaL_checkstring(L, 2);

    if (instance->Class(instance->address) == className) {
        lua_pushboolean(L, true);
        return 1;
    }

    lua_pushboolean(L, false);
    return 1;
}

static int instance::GetFullName(lua_State* L)
{
    auto* instance = (Instance*)luaL_checkudata(L, 1, "Instance");

    auto parentAddress = instance->Parent();
    std::string outName = instance->Name(instance->address);

    while (parentAddress != 0 && parentAddress != globals::DataModel) {
        Instance parentInstance(parentAddress);

        outName = parentInstance.Name(parentAddress) + "." + outName;
        parentAddress = parentInstance.Parent();
    }

    lua_pushstring(L, outName.c_str());
    return 1;
}

static int instance::WaitForChild(lua_State* L) {
    auto* instance = (Instance*)luaL_checkudata(L, 1, "Instance");
    auto childName = luaL_checkstring(L, 2);
    auto timeOut = luaL_optnumber(L, 3, 5);

    auto timeSpent = 0;

    while ( timeOut != timeSpent ) {
        auto child = instance->FindFirstChild(childName);
        if (child != 0) {
            newInstance(child);
            return 1;
        }

        timeSpent++;
        Sleep(1000);
    }

    std::cout << "Timeout exceeded\n";

    lua_pushnil(L);
    return 1;
}

const std::unordered_map<std::string, lua_CFunction> methods = {
        {"FindFirstChild", instance::FindFirstChild},
        {"FindFirstChildOfClass", instance::FindFirstClass},
        {"GetChildren", instance::GetChildren},
        {"GetDescendants", instance::GetDescendants},
        {"IsA", instance::IsA},
        {"GetFullName", instance::GetFullName},
        {"WaitForChild", instance::WaitForChild}
};


int instance::__index(lua_State* L)
{
    auto* instance = (Instance*) luaL_checkudata(L, 1, "Instance");
    std::string key = luaL_checkstring(L, 2);


    auto it = methods.find(key);
    if (it != methods.end()) {
        lua_pushcfunction(L, it->second);
        return 1;
    }

    // Basics
    if (key == "Name") {
        std::string name = instance->Name(instance->address);
        lua_pushstring(L, name.c_str());
        return 1;
    }

    else if (key == "ClassName") {
        std::string className = instance->Class(instance->address);
        lua_pushstring(L, className.c_str());
        return 1;
    }

    else if (key == "Address") {
        lua_pushinteger(L, instance->address);
        return 1;
    }

    else if (key == "Parent") {
        uintptr_t parent = instance->Parent();
        if (parent == 0) {
            lua_pushnil(L);
            return 1;
        }

        newInstance(parent)
        return 1;
    }

    // Specifics
     if (key == "LocalPlayer" && instance->Class(instance->address) == "Players") {
        newInstance(instance->LocalPlayer())
        return 1;
    }

    // useless to do it for the property for this one
    if (instance->Class(instance->address) == "Player") {
        if (key == "UserId") {
            auto userId = globals::mem->readMem<uintptr_t>( instance->address + offsets::player::UserId);
            lua_pushinteger(L, userId);
            return 1;
        }

        else if (key ==  "Character") {
            newInstance(instance->ModelInstance())
            return 1;
        }

        else if (key == "DisplayName") {
            lua_pushstring(L, instance->DisplayName().c_str());
            return 1;
        }

    }

    if (instance->Class(instance->address) == "Part" || instance->Class(instance->address) == "MeshPart") {
        auto& props = getPartProperties();
        ReadProperty(L, instance->address, props, key);
        return 1;
    }

    if (instance->Class(instance->address) == "Humanoid") {
        auto& props = getHumanoidProperties();
        ReadProperty(L, instance->address, props, key);
        return 1;
    }

    if (instance->Class(instance->address) == "Sound") {
        auto& props = getSoundProperties();
        ReadProperty(L, instance->address, props, key);
        return 1;
    }

    if (instance->Name(instance->address) == "Camera") {
        if (key == "Subject")  {
            auto subject = globals::mem->readMem<uintptr_t>( instance->address + offsets::camera::Subject);
            newInstance(subject)
        } else {
            auto& props = getCameraProperties();
            ReadProperty(L, instance->address, props, key);
        }
        return 1;
    }

    uintptr_t child = instance->FindFirstChild(key);
    if (child) {
        newInstance(child)
        return 1;
    }

    // Methods

    return 1;
}

int instance::__newindex(lua_State* L) {
    auto* instance = (Instance*) luaL_checkudata(L, 1, "Instance");
    std::string key = luaL_checkstring(L, 2);

    if (instance->Class(instance->address) == "Humanoid") {
        auto& props = getHumanoidProperties();
        WriteProperty(L, instance->address, props, key.c_str());
    }

    if (instance->Class(instance->address) == "Part" || instance->Class(instance->address) == "MeshPart") {
        auto& props = getPartProperties();
        WriteProperty(L, instance->address, props, key.c_str());
        return 1;
    }

    if (instance->Name(instance->address) == "Camera" && key == "Subject") {
        auto* destionationInstance = (Instance*) luaL_checkudata(L, 3, "Instance");
        globals::mem->writeMem(instance->address + offsets::camera::Subject, destionationInstance->address);

        lua_pushnil(L);
        return 1;
    }

    return 0;
}

static int instance::__gc(lua_State* L)
{
    auto* instance = (Instance*)luaL_checkudata(L, 1, "Instance");
    instance->~Instance();
    return 0;
}

int luaopen_instance(lua_State* L) {
    luaL_newmetatable(L, "Instance");

    lua_pushcfunction(L, instance::__index);
    lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, instance::__newindex);
    lua_setfield(L, -2, "__newindex");


    lua_pushcfunction(L, instance::__gc);
    lua_setfield(L, -2, "__gc");

    lua_pop(L, 1);
    return 0;
}
