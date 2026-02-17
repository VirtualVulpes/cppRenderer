#ifndef CPPRENDERER_LIGHT_H
#define CPPRENDERER_LIGHT_H

#include "GameObject.h"

namespace Light {

  enum LightType {
    kDirectional,
    kPoint,
    kSpot
  };

  struct DirectionalLight {
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
  };

  struct PointLight {
    Transform transform;
    glm::vec3 color;
    float intensity;
    float radius;
  };

  struct SpotLight {
    Transform transform;
    glm::vec3 color;
    float intensity;
  };
}

#endif