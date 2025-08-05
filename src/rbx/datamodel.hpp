//
// Created by sigma on 01/08/2025.
//

#ifndef EXTERNAL_LUA_DATAMODEL_HPP
#define EXTERNAL_LUA_DATAMODEL_HPP

#include "mem/mem.hpp"

#include "../ctx/offsets.h"
#include "../ctx/globals.h"
#include "instance.hpp"

namespace DataModel {
    void GetDataModel();
    uintptr_t GetPlaceId();
    std::string JobId();
}


#endif //EXTERNAL_LUA_DATAMODEL_HPP
