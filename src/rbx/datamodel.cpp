//
// Created by sigma on 01/08/2025.
//

#include "datamodel.hpp"

void DataModel::GetDataModel()
{
    auto DataModelPointer = globals::mem->readMem<uintptr_t>(globals::mem->rebase(offsets::datamodel::DataModelPointer));
    auto DataModel = globals::mem->readMem<uintptr_t>(DataModelPointer + offsets::datamodel::DataModelPointerToDatamodel);

    std::cout << "[Datamodel] : " << std::hex << DataModel << "\n";

    globals::DataModel = DataModel;

    if (DataModel == 0) return;
    Instance DatamodelInstance(DataModel);

    globals::Workspace = DatamodelInstance.FindFirstClass("Workspace");
    globals::Players = DatamodelInstance.FindFirstClass("Players");

    std::cout << "[Workspace] : " << std::hex << globals::Workspace << "\n";
    std::cout << "[Players] : " << std::hex << globals::Players << "\n";


    globals::LocalPlayer = globals::mem->readMem<uintptr_t>(globals::Players + offsets::player::LocalPlayer);

    while (globals::LocalPlayer == 0) globals::LocalPlayer = globals::mem->readMem<uintptr_t>(globals::Players + offsets::player::LocalPlayer);


    std::cout << "[LocalPlayer] : " << std::hex << globals::LocalPlayer << "\n";


    globals::PlaceId = DataModel::GetPlaceId();
    globals::JobId = DataModel::JobId();

}

uintptr_t DataModel::GetPlaceId() {
    return globals::mem->readMem<uintptr_t>(globals::DataModel + offsets::datamodel::PlaceId);
}

std::string DataModel::JobId() {
    return globals::mem->readString(globals::DataModel + offsets::datamodel::JobId);
}


