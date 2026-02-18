#include "TextureHandler.h"

#include <filesystem>
#include <format>
#include <iostream>

void TextureHandler::Load(std::string_view name, std::string_view path) {
  uint32_t new_id = ++id_;
  name_to_id_.emplace(std::string{name}, new_id);
  id_to_texture_.emplace(new_id, std::make_unique<Texture>(path));
}

void TextureHandler::LoadFromFolder(std::string_view path) {
  for (const auto& entry: std::filesystem::directory_iterator(path)) {
    Load(entry.path().stem().string(), entry.path().string());
  }
}

Texture* TextureHandler::Get(std::string_view name) {
  auto it = name_to_id_.find(std::string{name});
  if (it == name_to_id_.end())
    return nullptr;

  auto texIt = id_to_texture_.find(it->second);
  if (texIt == id_to_texture_.end())
    return nullptr;

  return texIt->second.get();
}

uint32_t TextureHandler::GetId(std::string_view name) {
  auto it = name_to_id_.find(std::string{name});
  if (it == name_to_id_.end())
    throw std::runtime_error(std::format("texture not found with name {}\n", std::string(name)));

  return it->second;
}

void TextureHandler::Clear()
{
  id_to_texture_.clear();
  name_to_id_.clear();
}
