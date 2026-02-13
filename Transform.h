#ifndef CPPRENDERER_TRANSFORM_H
#define CPPRENDERER_TRANSFORM_H

#include "glm/detail/type_vec.hpp"
#include "glm/detail/type_vec3.hpp"

struct Transform {
  glm::vec3 pos{0.0f};
  glm::vec3 rotation{0.0f};
  glm::vec3 scale{1.0f};
};

#endif