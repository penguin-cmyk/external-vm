#include <string>
#include <iostream>

#include "../env/init.h"
#include "../rbx/datamodel.hpp"
#include "../ctx/globals.h"

#include "gui.h"
#include "themes.h"

// Credit: Cazz for the ui setup tutorial
// https://www.youtube.com/watch?v=Nrta_J_c9Cc&t
// https://github.com/cazzwastaken/borderless-imgui-window/tree/main

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
        HWND window,
        UINT message,
        WPARAM wideParameter,
        LPARAM longParameter
);

long __stdcall WindowProcess(
        HWND window,
        UINT message,
        WPARAM wideParameter,
        LPARAM longParameter)
{
    if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
        return true;

    switch (message)
    {
        case WM_SIZE: {
            if (gui::device && wideParameter != SIZE_MINIMIZED)
            {
                gui::presentParameters.BackBufferWidth = LOWORD(longParameter);
                gui::presentParameters.BackBufferHeight = HIWORD(longParameter);
                gui::ResetDevice();
            }
        }return 0;

        case WM_SYSCOMMAND: {
            if ((wideParameter & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
        }break;

        case WM_DESTROY: {
            PostQuitMessage(0);
        }return 0;

        case WM_LBUTTONDOWN: {
            gui::position = MAKEPOINTS(longParameter); // set click points
        }return 0;

        case WM_MOUSEMOVE: {
            if (wideParameter == MK_LBUTTON)
            {
                const auto points = MAKEPOINTS(longParameter);
                auto rect = ::RECT{ };

                GetWindowRect(gui::window, &rect);

                rect.left += points.x - gui::position.x;
                rect.top += points.y - gui::position.y;

                if (gui::position.x >= 0 &&
                    gui::position.x <= gui::WIDTH &&
                    gui::position.y >= 0 && gui::position.y <= 19)
                    SetWindowPos(
                            gui::window,
                            HWND_TOPMOST,
                            rect.left,
                            rect.top,
                            0, 0,
                            SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
                    );
            }

        }return 0;

    }

    return DefWindowProc(window, message, wideParameter, longParameter);
}

void gui::CreateHWindow(const char* windowName) noexcept
{
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_CLASSDC;
    windowClass.lpfnWndProc = (WNDPROC)WindowProcess;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = GetModuleHandleA(0);
    windowClass.hIcon = 0;
    windowClass.hCursor = 0;
    windowClass.hbrBackground = 0;
    windowClass.lpszMenuName = 0;
    windowClass.lpszClassName = "yep fud";
    windowClass.hIconSm = 0;

    RegisterClassEx(&windowClass);

    window = CreateWindowEx(
            WS_EX_LAYERED,
            "yep fud",
            windowName,
            WS_POPUP,
            100, 100, WIDTH, HEIGHT,
            nullptr, nullptr, windowClass.hInstance, nullptr
    );

    SetLayeredWindowAttributes(window, RGB(0,0,0), 0, LWA_COLORKEY);

    ShowWindow(window, SW_SHOWDEFAULT);
    UpdateWindow(window);
}

void gui::DestroyHWindow() noexcept
{
    DestroyWindow(window);
    UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool gui::CreateDevice() noexcept
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);

    if (!d3d)
        return false;

    ZeroMemory(&presentParameters, sizeof(presentParameters));

    presentParameters.Windowed = TRUE;
    presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
    presentParameters.EnableAutoDepthStencil = TRUE;
    presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
    presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    if (d3d->CreateDevice(
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            window,
            D3DCREATE_HARDWARE_VERTEXPROCESSING,
            &presentParameters,
            &device) < 0)
        return false;

    return true;
}

void gui::ResetDevice() noexcept
{
    ImGui_ImplDX9_InvalidateDeviceObjects();

    const auto result = device->Reset(&presentParameters);

    if (result == D3DERR_INVALIDCALL)
        IM_ASSERT(0);

    ImGui_ImplDX9_CreateDeviceObjects();
}

void gui::DestroyDevice() noexcept
{
    if (device)
    {
        device->Release();
        device = nullptr;
    }

    if (d3d)
    {
        d3d->Release();
        d3d = nullptr;
    }
}




void gui::CreateImGui() noexcept
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ::ImGui::GetIO();

    io.IniFilename = NULL;

    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX9_Init(device);
}

void gui::DestroyImGui() noexcept
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void gui::BeginRender() noexcept
{
    MSG message;
    while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);

        if (message.message == WM_QUIT)
        {
            isRunning = !isRunning;
            return;
        }
    }

    // Start the Dear ImGui frame
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void gui::EndRender() noexcept
{
    ImGui::EndFrame();

    device->SetRenderState(D3DRS_ZENABLE, FALSE);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

    device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

    if (device->BeginScene() >= 0)
    {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        device->EndScene();
    }

    const auto result = device->Present(0, 0, 0, 0);

    if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
        ResetDevice();
}


// Just a simple recursive loop
void RenderInstanceChildren(Instance& DataModel, uintptr_t parentAddress) {
    if (parentAddress == 0) return;
    auto children = DataModel.GetChildren(parentAddress);

    for (uintptr_t child : children) {
        if (child == 0) continue;

        std::string childName;
        if (parentAddress == globals::DataModel) {
            childName = DataModel.Class(child);
        } else {
            childName = DataModel.Name(child);
        }

        if (childName.empty()) continue;

        std::string uniqueId = childName + "##" + std::to_string(child);

        auto childChildren = DataModel.GetChildren(child);
        bool hasChildren = !childChildren.empty();

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
        if (!hasChildren) {
            flags |= ImGuiTreeNodeFlags_Leaf;
        }

        bool nodeOpen = ImGui::TreeNodeEx(uniqueId.c_str(), flags, "%s", childName.c_str());

        auto className = DataModel.Class(child);
        auto isNotEmpty = !className.empty();

        if (ImGui::IsItemHovered() && isNotEmpty) {
            ImGui::SetTooltip("Class: %s", className.c_str());
        }

        if (nodeOpen) {
            if (hasChildren) {
                RenderInstanceChildren(DataModel, child);
            }
            ImGui::TreePop();
        }
    }
}


void gui::Render() noexcept
{
    static bool themeApplied = false;
    if (!themeApplied) {
        SetCatppuccinMochaTheme();
        themeApplied = true;
    }

    ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
    ImGui::Begin(
            "External vm",
            &isRunning,
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoMove
    );


    static char textBuffer[8192] = "";

    if (ImGui::BeginTabBar("MainTabs")) {
        if (ImGui::BeginTabItem("Script Editor")) {
            ImGui::InputTextMultiline(
                    "##ScriptInput",
                    textBuffer,
                    sizeof(textBuffer),
                    ImVec2(WIDTH * 0.97f, HEIGHT * 0.8f),
                    ImGuiInputTextFlags_AllowTabInput
            );

            if (ImGui::Button("Execute", ImVec2(WIDTH * 0.15f, 20))) {
                std::thread luaThread([text = std::string(textBuffer)] {
                    lua_State* threadL = luaL_newstate();
                    luaL_openlibs(threadL);
                    register_everything(threadL);

                    if (luaL_dostring(threadL, text.c_str()) != LUA_OK) {
                        std::cout << lua_tostring(threadL, -1) << "\n";
                    }

                    lua_close(threadL);
                });
                luaThread.detach();
            }
            ImGui::SameLine();
            if (ImGui::Button("Clear", ImVec2(WIDTH * 0.15f, 20))) {
                textBuffer[0] = '\0';
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Instance Explorer")) {
            ImGui::BeginChild("ExplorerScroll", ImVec2(0, HEIGHT * 0.9f), true);

            Instance DataModel(globals::DataModel);
            auto children = DataModel.GetChildren(DataModel.address);
            RenderInstanceChildren(DataModel, DataModel.address);

            ImGui::EndChild();
            ImGui::EndTabItem();
        }


        ImGui::EndTabBar();
    }

    ImGui::End();
}