#ifndef CPPRENDERER_MATERIAL_H
#define CPPRENDERER_MATERIAL_H

#include "Shader.h"
#include "Texture.h"
#include "glm/detail/type_vec3.hpp"

struct Material {
  Shader* shader{};
  Texture* diffuse_texture{};
  Texture* specular_texture{};
  glm::vec3 tint_{1.0, 1.0, 1.0};
};

#endif