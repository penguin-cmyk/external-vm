#ifndef EXTERNAL_LUA_PROPERTIES_H
#define EXTERNAL_LUA_PROPERTIES_H

#include <iostream>


#include "../../ctx/offsets.h"
#include "../../ctx/globals.h"
#include "../../env/definitons.h"
#include "../../env/functions/misc/vector3.h"


typedef struct Property {
    uintptr_t offset;
    enum Type { FLOAT, INT, BOOL, VECTOR3, STRING, UNITPTR_T } type;
    uintptr_t mirrorOffset = 0;
} Property;

struct vec3 {
    float x,y,z;
};

inline std::unordered_map<std::string, Property>& getHumanoidProperties() {
    static std::unordered_map<std::string, Property> humanoidProperties = {
            {"WalkSpeed", {offsets::humanoid::WalkSpeed, Property::FLOAT, offsets::humanoid::WalkSpeedCheck}},
            {"JumpPower", {offsets::humanoid::JumpPower, Property::FLOAT}},
            {"HipHeight", {offsets::humanoid::HipHeight, Property::FLOAT}},
            {"Sit",       {offsets::humanoid::Sit,       Property::BOOL}},
            {"Health",    {offsets::humanoid::Health,    Property::FLOAT}},
            {"MaxHealth", {offsets::humanoid::MaxHealth, Property::FLOAT}},
            {"MoveDirection", {offsets::humanoid::MoveDirection, Property::VECTOR3}},
    };
    return humanoidProperties;
}

inline std::unordered_map<std::string, Property>& getPartProperties() {
    static std::unordered_map<std::string, Property> partProperties = {
            {"Position", {offsets::part::Position, Property::VECTOR3, offsets::part::Primitive}},
            {"Velocity", {offsets::part::Velocity, Property::VECTOR3, offsets::part::Primitive}},
            {"AssemblyAngularVelocity", {offsets::part::AssemblyAngularVelocity, Property::VECTOR3, offsets::part::Primitive}},
            {"AssemblyLinearVelocity", {offsets::part::AssemblyLinearVelocity, Property::VECTOR3, offsets::part::Primitive}},
            {"Size", {offsets::part::Size, Property::VECTOR3, offsets::part::Primitive}},
            {"CanCollide", { offsets::part::CanCollide, Property::BOOL,  offsets::part::Primitive}},
            {"Anchored", { offsets::part::Anchored, Property::BOOL, offsets::part::Primitive}}
    };
    return partProperties;
}

inline std::unordered_map<std::string, Property>& getSoundProperties() {
    static std::unordered_map<std::string, Property> soundProperties = {
            {"SoundId", {offsets::instance::SoundId, Property::STRING}},
    };
    return soundProperties;
}

inline std::unordered_map<std::string, Property>& getCameraProperties() {
    static std::unordered_map<std::string, Property> cameraProperties = {
            {"FieldOfView", { offsets::camera::Fov, Property::FLOAT }},
            {"Position", {offsets::camera::Position, Property::VECTOR3}}

    };
    return cameraProperties;
}

void WriteProperty(lua_State* L, uintptr_t addr, std::unordered_map<std::string, Property> &prop, const char* key) {
    auto it = prop.find(key);
    if (it == prop.end()) return;
    auto& property = it->second;

    switch (property.type) {
        case Property::FLOAT: {
            float value = (float)luaL_checknumber(L, 3);
            globals::mem->writeMem<float>(addr + property.offset, value);
            if (property.mirrorOffset)
                globals::mem->writeMem<float>(addr+ property.mirrorOffset, value);
            break;
        }
        case Property::INT: {
            int value = (int)luaL_checkinteger(L, 3);
            globals::mem->writeMem<int>(addr + property.offset, value);
            break;
        }
        case Property::BOOL: {
            if (property.mirrorOffset) {
                addr = globals::mem->readMem<uintptr_t>( addr + property.mirrorOffset);
            }
            globals::mem->writeMem<bool>(addr + property.offset, lua_toboolean(L, 3));
            break;
        }

        case Property::VECTOR3: {
            Vector3* vector = check_vector3(L, 3);
            if (vector) {
                if (property.mirrorOffset) {
                    addr = globals::mem->readMem<uintptr_t>(addr + property.mirrorOffset); // turn to primitive for stuff like part pos
                }

                vec3 vec = {vector->x, vector->y, vector->z};
                globals::mem->writeMem<vec3>(addr + property.offset, vec);
            }
            break;

        }

        case Property::STRING: {
            auto read = addr + property.offset;
            if (property.mirrorOffset) {
                read = globals::mem->readMem<uintptr_t>(addr + property.offset);
            }
            const char* newString = luaL_checkstring(L, 3);

            globals::mem->writeString(read, newString);
            break;

        }

        case Property::UNITPTR_T: {
            if (property.mirrorOffset) {
                addr = addr + property.offset;
            }

            uintptr_t value = luaL_checkinteger(L, 3);
            globals::mem->writeMem<uintptr_t>( addr, value);
            break;

        }
    }
}

int ReadProperty(lua_State* L, uintptr_t addr, std::unordered_map<std::string, Property> &prop, std::string key) {
    auto it = prop.find(key);
    if (it == prop.end()) {
        lua_pushnil(L);
        return 1;
    }

    auto& property = it->second;
    switch (property.type) {
        case Property::FLOAT: {
            float value = globals::mem->readMem<float>(addr + property.offset);
            lua_pushnumber(L, value);
            break;
        }
        case Property::INT: {
            int value = globals::mem->readMem<int>(addr + property.offset);
            lua_pushinteger(L, value);
            break;
        }
        case Property::BOOL: {
            int value = globals::mem->readMem<int>(addr + property.offset);
            lua_pushboolean(L, value != 0);
            break;
        }

        case Property::VECTOR3: {
            if (property.mirrorOffset) {
                addr = globals::mem->readMem<uintptr_t>(addr + property.mirrorOffset); // turn to primitive for stuff like part pos
            }

            vec3 outVector  = globals::mem->readMem<vec3>(addr + property.offset);
            push_vector3(L, outVector.x, outVector.y, outVector.z);
            break;
        }

        case Property::STRING: {
            auto read = addr + property.offset;
            if (property.mirrorOffset) {
                read = globals::mem->readMem<uintptr_t>(addr + property.offset);
            }

            lua_pushstring(L, globals::mem->readString(read).c_str());
            break;
        }

        case Property::UNITPTR_T: {
            if (property.mirrorOffset) {
                addr = addr + property.offset;
            }

            lua_pushinteger(L, globals::mem->readMem<uintptr_t>(addr));
            break;
        }
    }

    return 1;
}

#endif //EXTERNAL_LUA_PROPERTIES_H