//
// Created by sigma on 01/08/2025.
//

#ifndef OFFSETS_H
#define OFFSETS_H

#include <iostream>

namespace offsets {
    namespace datamodel {
        constexpr uintptr_t DataModelPointer = 0x6ED6E38;
        constexpr uintptr_t DataModelPointerToDatamodel = 0x1C0;
        constexpr uintptr_t PlaceId                     = 0x1A0;
        constexpr uintptr_t JobId                       = 0x140;
    }

    namespace instance {
        constexpr uintptr_t Name             = 0x78;
        constexpr uintptr_t ClassDescriptor  = 0x18;
        constexpr uintptr_t ClassName        = 0x8;
        constexpr uintptr_t Children         = 0x80;
        constexpr uintptr_t ChildrenEnd      = 0x8;
        constexpr uintptr_t ModelInstance    = 0x328;
        constexpr uintptr_t Parent           = 0x50;
        constexpr uintptr_t SoundId          = 0xE0;
    }

    namespace player {
        constexpr uintptr_t LocalPlayer      = 0x128;
        constexpr uintptr_t DisplayName      = 0x118;
        constexpr uintptr_t UserId           = 0x270;

        //constexpr uintptr_t MaxZoom          = 0x2B8;
        //constexpr uintptr_t MinZoom          = 0x2BC;
    }

    namespace humanoid {
        constexpr uintptr_t Health           = 0x19C;
        constexpr uintptr_t HipHeight        = 0x1A8;
        constexpr uintptr_t MaxHealth        = 0x1BC;
        constexpr uintptr_t Sit              = 0x1E3;
        constexpr uintptr_t JumpPower        = 0x1B8;
        constexpr uintptr_t WalkSpeed        = 0x1DC;
        constexpr uintptr_t WalkSpeedCheck   = 0x3B8;
        constexpr uintptr_t MoveDirection    = 0x160;
    }

    namespace camera {
        constexpr uintptr_t Fov              = 0x168;
        constexpr uintptr_t Subject          = 0xF0;
        constexpr uintptr_t Position         = 0x124;
    }

    namespace part {
        constexpr uintptr_t Primitive        = 0x178;
        constexpr uintptr_t Position         = 0x14C;
        constexpr uintptr_t Size             = 0x25C;
        constexpr uintptr_t Velocity         = 0x158;
        constexpr uintptr_t Material         = 0x2d8;
        constexpr uintptr_t AssemblyAngularVelocity = 0x164;
        constexpr uintptr_t AssemblyLinearVelocity = 0x158;
        constexpr uintptr_t CanCollide             = 0x309 ;
        constexpr uintptr_t Anchored               = 0x311;
    }
}

#endif