#ifndef CPPRENDERER_RESOURCEHANDLER_H
#define CPPRENDERER_RESOURCEHANDLER_H

#include <format>
#include <memory>
#include <string_view>
#include <unordered_map>

#include "Shader.h"

template <typename T>
class ResourceHandler {
public:
  void Add(std::string_view name, std::unique_ptr<T> resource) {
    uint32_t new_id = ++id_;
    name_to_id_.emplace(std::string{name}, new_id);
    id_to_resource_.emplace(new_id, std::move(resource));
  }

  template<typename... Args>
  void Create(std::string_view name, Args&&... args) {
    uint32_t new_id = ++id_;
    name_to_id_.emplace(std::string{name}, new_id);
    std::unique_ptr<T> material = std::make_unique<T>(std::forward<Args>(args)...);
    id_to_resource_.emplace(new_id, std::move(material));
  }

  uint32_t GetId(std::string_view name) {
    auto it = name_to_id_.find(std::string{name});
    if (it == name_to_id_.end())
      throw std::runtime_error(std::format("resource not found with name {}\n", std::string(name)));

    return it->second;
  }

  T* GetPointer(std::string_view name) {
    auto it = name_to_id_.find(std::string{name});
    if (it == name_to_id_.end())
      return nullptr;

    return GetPointer(it->second);
  }

  T* GetPointer(uint32_t id) {
    auto texIt = id_to_resource_.find(id);
    if (texIt == id_to_resource_.end())
      return nullptr;

    return texIt->second.get();
  }

private:
  std::unordered_map<std::string, uint32_t> name_to_id_;
  std::unordered_map<uint32_t, std::unique_ptr<T> > id_to_resource_;
  uint32_t id_{0};
};

#endif