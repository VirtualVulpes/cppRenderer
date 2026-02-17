#include "MaterialHandler.h"

#include <filesystem>
#include <format>

void MaterialHandler::Add(std::string_view name, Material* material) {
    uint32_t new_id = ++id_;
    name_to_id_.emplace(std::string{name}, new_id);
    id_to_material_.emplace(new_id, material);
}

Material* MaterialHandler::GetPointer(std::string_view name) {
    auto it = name_to_id_.find(std::string{name});
    if (it == name_to_id_.end())
        return nullptr;

    return GetPointer(it->second);
}

Material* MaterialHandler::GetPointer(uint32_t id) {
    auto texIt = id_to_material_.find(id);
    if (texIt == id_to_material_.end())
        return nullptr;

    return texIt->second.get();
}

uint32_t MaterialHandler::GetId(std::string_view name) {
    auto it = name_to_id_.find(std::string{name});
    if (it == name_to_id_.end())
        throw std::runtime_error(std::format("material not found with name {}\n", std::string(name)));

    return it->second;
}
