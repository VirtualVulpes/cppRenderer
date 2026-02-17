#ifndef CPPRENDERER_TEXTUREHANDLER_H
#define CPPRENDERER_TEXTUREHANDLER_H

#include <memory>
#include <string_view>
#include <unordered_map>

#include "Texture.h"

class TextureHandler {
public:
  void Load(std::string_view name, std::string_view path);
  void LoadFromFolder(std::string_view path);
  Texture* Get(std::string_view name);
  uint32_t GetId(std::string_view name);
private:
  std::unordered_map<std::string, uint32_t> name_to_id_;
  std::unordered_map<uint32_t, std::unique_ptr<Texture> > id_to_texture_;
  uint32_t id_{0};
};

#endif
