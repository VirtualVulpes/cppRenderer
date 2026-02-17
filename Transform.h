#ifndef CPPRENDERER_TRANSFORM_H
#define CPPRENDERER_TRANSFORM_H

#include "glm/detail/type_vec.hpp"
#include "glm/detail/type_vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

struct Transform {
  glm::vec3 pos{0.0f};
  glm::vec3 rotation{0.0f};
  glm::vec3 scale{1.0f};

  glm::mat4 GetModelMatrix() const {
    return glm::translate(glm::mat4(1.0f), pos) *
           glm::toMat4(glm::quat(glm::radians(rotation))) *
           glm::scale(glm::mat4(1.0f), scale);
  }
};

#endif