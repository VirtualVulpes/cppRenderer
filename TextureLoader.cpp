#include "TextureLoader.h"

#include <filesystem>
#include <memory>

void TextureLoader::LoadFromFolder(std::string_view path, ResourceHandler<Texture> &texture_handler) {
  for (const auto &entry: std::filesystem::directory_iterator(path)) {
    texture_handler.Add(entry.path().stem().string(), std::make_unique<Texture>(entry.path().string()));
  }
}