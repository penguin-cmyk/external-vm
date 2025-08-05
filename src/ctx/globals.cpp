//
// Created by sigma on 02/08/2025.
//

#include "globals.h"

namespace globals {
        uintptr_t DataModel = 0;
        uintptr_t Workspace = 0;
        uintptr_t Players = 0;
        uintptr_t LocalPlayer = 0;

        uintptr_t PlaceId = 0;
        std::string JobId = "";

        std::unique_ptr<Memory> mem = nullptr;
}