//
// Created by sigma on 03/08/2025.
//

#ifndef EXTERNAL_LUA_THEMES_H
#define EXTERNAL_LUA_THEMES_H

#include "gui.h"

void SetCatppuccinMochaTheme()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // Backgrounds
    colors[ImGuiCol_WindowBg]           = ImVec4(0.12f, 0.12f, 0.18f, 1.00f); // Base: #1e1e2e
    colors[ImGuiCol_ChildBg]            = ImVec4(0.10f, 0.10f, 0.15f, 1.00f); // Mantle: #181825
    colors[ImGuiCol_PopupBg]            = ImVec4(0.10f, 0.10f, 0.15f, 1.00f); // Mantle: #181825
    colors[ImGuiCol_MenuBarBg]          = ImVec4(0.12f, 0.12f, 0.18f, 1.00f); // Base: #1e1e2e
    colors[ImGuiCol_TitleBg]            = ImVec4(0.10f, 0.10f, 0.15f, 1.00f); // Mantle: #181825
    colors[ImGuiCol_TitleBgActive]      = ImVec4(0.18f, 0.18f, 0.27f, 1.00f); // Surface0: #313244
    colors[ImGuiCol_TitleBgCollapsed]   = ImVec4(0.10f, 0.10f, 0.15f, 1.00f); // Mantle: #181825

    // Borders
    colors[ImGuiCol_Border]             = ImVec4(0.31f, 0.31f, 0.42f, 1.00f); // Surface2: #585b70
    colors[ImGuiCol_BorderShadow]       = ImVec4(0.00f, 0.00f, 0.00f, 0.00f); // Transparent

    // Text
    colors[ImGuiCol_Text]               = ImVec4(0.80f, 0.84f, 0.96f, 1.00f); // Text: #cdd6f4
    colors[ImGuiCol_TextDisabled]       = ImVec4(0.51f, 0.53f, 0.64f, 1.00f); // Overlay1: #7f849c

    // Frame BG (Inputs, Sliders, etc.)
    colors[ImGuiCol_FrameBg]            = ImVec4(0.18f, 0.18f, 0.27f, 1.00f); // Surface0: #313244
    colors[ImGuiCol_FrameBgHovered]     = ImVec4(0.27f, 0.28f, 0.39f, 1.00f); // Surface1: #45475a
    colors[ImGuiCol_FrameBgActive]      = ImVec4(0.36f, 0.36f, 0.48f, 1.00f); // Surface2: #585b70

    // Tabs
    colors[ImGuiCol_Tab]                = ImVec4(0.18f, 0.18f, 0.27f, 1.00f); // Surface0: #313244
    colors[ImGuiCol_TabHovered]         = ImVec4(0.70f, 0.65f, 0.97f, 1.00f); // Mauve: #cba6f7
    colors[ImGuiCol_TabActive]          = ImVec4(0.27f, 0.28f, 0.39f, 1.00f); // Surface1: #45475a
    colors[ImGuiCol_TabUnfocused]       = ImVec4(0.18f, 0.18f, 0.27f, 1.00f); // Surface0: #313244
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.27f, 0.28f, 0.39f, 1.00f); // Surface1: #45475a

    // Buttons
    colors[ImGuiCol_Button]             = ImVec4(0.36f, 0.36f, 0.48f, 1.00f); // Surface2: #585b70
    colors[ImGuiCol_ButtonHovered]      = ImVec4(0.70f, 0.65f, 0.97f, 1.00f); // Mauve: #cba6f7
    colors[ImGuiCol_ButtonActive]       = ImVec4(0.80f, 0.65f, 0.72f, 1.00f); // Flamingo: #f2cdcd

    // Headers (e.g., TreeNode)
    colors[ImGuiCol_Header]             = ImVec4(0.36f, 0.36f, 0.48f, 1.00f); // Surface2: #585b70
    colors[ImGuiCol_HeaderHovered]      = ImVec4(0.70f, 0.65f, 0.97f, 1.00f); // Mauve: #cba6f7
    colors[ImGuiCol_HeaderActive]       = ImVec4(0.80f, 0.65f, 0.72f, 1.00f); // Flamingo: #f2cdcd

    // Checkmark & Slider
    colors[ImGuiCol_CheckMark]          = ImVec4(0.80f, 0.84f, 0.96f, 1.00f); // Text: #cdd6f4
    colors[ImGuiCol_SliderGrab]         = ImVec4(0.70f, 0.65f, 0.97f, 1.00f); // Mauve: #cba6f7
    colors[ImGuiCol_SliderGrabActive]   = ImVec4(0.80f, 0.65f, 0.72f, 1.00f); // Flamingo: #f2cdcd

    // Scrollbar
    colors[ImGuiCol_ScrollbarBg]        = ImVec4(0.18f, 0.18f, 0.27f, 1.00f); // Surface0: #313244
    colors[ImGuiCol_ScrollbarGrab]      = ImVec4(0.36f, 0.36f, 0.48f, 1.00f); // Surface2: #585b70
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.70f, 0.65f, 0.97f, 1.00f); // Mauve: #cba6f7
    colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.80f, 0.65f, 0.72f, 1.00f); // Flamingo: #f2cdcd

    // Separator
    colors[ImGuiCol_Separator]          = ImVec4(0.31f, 0.31f, 0.42f, 1.00f); // Surface2: #585b70
    colors[ImGuiCol_SeparatorHovered]   = ImVec4(0.70f, 0.65f, 0.97f, 1.00f); // Mauve: #cba6f7
    colors[ImGuiCol_SeparatorActive]    = ImVec4(0.80f, 0.65f, 0.72f, 1.00f); // Flamingo: #f2cdcd

    // Resize Grip
    colors[ImGuiCol_ResizeGrip]         = ImVec4(0.36f, 0.36f, 0.48f, 0.20f); // Surface2: #585b70 (low alpha)
    colors[ImGuiCol_ResizeGripHovered]  = ImVec4(0.70f, 0.65f, 0.97f, 0.60f); // Mauve: #cba6f7
    colors[ImGuiCol_ResizeGripActive]   = ImVec4(0.80f, 0.65f, 0.72f, 0.80f); // Flamingo: #f2cdcd

    // Rounding and Style
    style.WindowRounding    = 4.0f;
    style.ChildRounding     = 6.0f;
    style.FrameRounding     = 4.0f;
    style.PopupRounding     = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.GrabRounding      = 4.0f;
    style.TabRounding       = 4.0f;

    // Padding and Spacing
    style.WindowPadding     = ImVec2(8.0f, 8.0f);
    style.FramePadding      = ImVec2(4.0f, 3.0f);
    style.ItemSpacing       = ImVec2(8.0f, 4.0f);
    style.ItemInnerSpacing  = ImVec2(4.0f, 4.0f);
    style.IndentSpacing     = 20.0f;

    // Scrollbar and Border
    style.ScrollbarSize     = 12.0f;
    style.FrameBorderSize   = 1.0f;
    style.WindowBorderSize  = 1.0f;
    style.TabBorderSize     = 1.0f;
}

#endif //EXTERNAL_LUA_THEMES_H
