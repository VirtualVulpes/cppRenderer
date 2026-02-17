#ifndef CPPRENDERER_MATERIALHANDLER_H
#define CPPRENDERER_MATERIALHANDLER_H

#include <memory>
#include <string_view>
#include <unordered_map>

#include "Material.h"

class MaterialHandler {
public:
  void Add(std::string_view name, std::unique_ptr<Material> material);
  void Create(std::string_view name, Shader* shader, uint32_t diffuse, uint32_t specular);

  Material* GetPointer(std::string_view name);
  Material* GetPointer(uint32_t id);

  uint32_t GetId(std::string_view name);

private:
  uint32_t id_{0};
  std::unordered_map<std::string, uint32_t> name_to_id_;
  std::unordered_map<uint32_t, std::unique_ptr<Material> > id_to_material_;
};

#endif
