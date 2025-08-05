//
// Created by sigma on 01/08/2025.
//

#ifndef EXTERNAL_LUA_GLOBALS_H
#define EXTERNAL_LUA_GLOBALS_H

#include <iostream>
#include "mem/mem.hpp"

namespace globals {
    extern uintptr_t DataModel;
    extern uintptr_t Workspace;
    extern uintptr_t Players;
    extern uintptr_t LocalPlayer;

    extern uintptr_t PlaceId;
    extern std::string JobId;

    extern std::unique_ptr<Memory> mem;
}

#endif //EXTERNAL_LUA_GLOBALS_H
