#ifndef CPPRENDERER_MATERIAL_H
#define CPPRENDERER_MATERIAL_H

#include "Shader.h"
#include "Texture.h"
#include "glm/detail/type_vec3.hpp"

struct Material {
  uint32_t shader{0};
  uint32_t diffuse_texture{0};
  uint32_t specular_texture{0};
  glm::vec3 tint_{1.0, 1.0, 1.0};
};

#endif