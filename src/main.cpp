//
// Created by sigma on 06/08/2025.
//

#include <thread>
#include <iostream>
#include <sstream>

#include "mem/mem.hpp"
#include "rbx/datamodel.hpp"
#include "env/definitons.h"
#include "ui/gui.h"

int __stdcall wWinMain(
        HINSTANCE instance,
        HINSTANCE previousInstance,
        PWSTR arguments,
        int commandShoW
) {
    AllocConsole();
    FILE* dummy;
    freopen_s(&dummy, "CONOUT$", "w", stdout); // Redirect std::cout to console
    std::cout << "Console initialized\n";

    globals::mem = std::make_unique<Memory>("RobloxPlayerBeta.exe");
    if (!globals::mem->isValid()) {
        std::cout << "Roblox not found\n";
        return 1;
    }

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    lua_close(L);

    DataModel::GetDataModel();

    gui::CreateHWindow("----------------------------------------------------------------------------------------------------------------");
    gui::CreateDevice();
    gui::CreateImGui();

    std::thread([]() {
        while (gui::isRunning)
        {
            auto DataModelPointer = globals::mem->readMem<uintptr_t>(globals::mem->rebase(offsets::datamodel::DataModelPointer));
            auto DataModel = globals::mem->readMem<uintptr_t>(DataModelPointer + offsets::datamodel::DataModelPointerToDatamodel);
            auto PlaceId = globals::mem->readMem<uintptr_t>(DataModel + offsets::datamodel::PlaceId);

            if (PlaceId != globals::PlaceId)
            {
                std::cout << globals::PlaceId << " -> " << PlaceId << "\n";

                globals::PlaceId = PlaceId;
                DataModel::GetDataModel();
            }

            Sleep(100);
        }
    }).detach();

    while (gui::isRunning)
    {

        gui::BeginRender();
        gui::Render();
        gui::EndRender();

        Sleep(10);
    }

    gui::DestroyImGui();
    gui::DestroyDevice();
    gui::DestroyHWindow();
    FreeConsole(); // Clean up console
    return EXIT_SUCCESS;
}