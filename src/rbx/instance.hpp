//
// Created by sigma on 01/08/2025.
//

#ifndef EXTERNAL_LUA_INSTANCE_HPP
#define EXTERNAL_LUA_INSTANCE_HPP

#include "mem/mem.hpp"
#include "../ctx/globals.h"
#include "../ctx/offsets.h"

class Instance {
public:
    uintptr_t address;

    std::vector<uintptr_t> GetChildren(uintptr_t addr)
    {
         auto childrenArray = globals::mem->readMem<uintptr_t>(addr + offsets::instance::Children);
         auto childrenEnd   = globals::mem->readMem<uintptr_t>(childrenArray+ offsets::instance::ChildrenEnd);

         if (childrenEnd == 0) return std::vector<uintptr_t>();

         auto currentChild = globals::mem->readMem<uintptr_t>(childrenArray);
         if (currentChild == 0) return std::vector<uintptr_t>();
         auto childrenLength = (childrenEnd - currentChild) / 0x10;

         std::vector<uintptr_t> children;
         children.resize(childrenLength);

         for (uintptr_t i = 0; i < childrenLength; i++) {
             children.push_back(globals::mem->readMem<uintptr_t>(currentChild));
             currentChild += 0x10;
         }

         return children;
    }

    std::vector<uintptr_t> GetDescendants(uintptr_t addr)
    {
        std::vector<uintptr_t> results;

        std::vector<uintptr_t> children = GetChildren(addr);
        results.insert(results.begin(), children.begin(), children.end());

        for (uintptr_t child : children) {
            std::vector<uintptr_t> decendants = GetDescendants(child);
            results.insert(results.begin(), decendants.begin(), decendants.end());
        }
        return results;
    }


    std::string DisplayName()
    {
        return globals::mem->readString(address + offsets::player::DisplayName);
    }

    std::string Name(uintptr_t addr)
    {
        auto namePointer = globals::mem->readMem<uintptr_t>(addr + offsets::instance::Name);
        return globals::mem->readString(namePointer);
    }

    std::string Class(uintptr_t addr)
    {
        auto classDescriptor = globals::mem->readMem<uintptr_t>(addr + offsets::instance::ClassDescriptor);
        auto classnamePointer = globals::mem->readMem<uintptr_t>(classDescriptor + offsets::instance::ClassName);

        return globals::mem->readString(classnamePointer);
    }

    uintptr_t FindFirstChild(const std::string& childName)
    {
        std::vector<uintptr_t> children = GetChildren(address);
        for (auto child : children ) {
            if (Name(child) == childName) return child;
        }

        return 0;
    }

    uintptr_t FindFirstClass(const std::string& className)
    {
        std::vector<uintptr_t> children = GetChildren(address);
        for (auto child : children ) {
            if (Class(child) == className) return child;
        }

        return 0;
    }

    uintptr_t LocalPlayer() {
            return globals::mem->readMem<uintptr_t>(address + offsets::player::LocalPlayer );
    };

    uintptr_t Parent() {
        return globals::mem->readMem<uintptr_t>(address + offsets::instance::Parent);
    }


    uintptr_t ModelInstance() {
        return globals::mem->readMem<uintptr_t>(address + offsets::instance::ModelInstance);
    }

    explicit Instance(uintptr_t addr) {
        address = addr;
    }
};

#endif //EXTERNAL_LUA_INSTANCE_HPP
