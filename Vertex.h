#ifndef CPPRENDERER_VERTEX_H
#define CPPRENDERER_VERTEX_H

#include <glm/glm.hpp>

struct Vertex {
  glm::vec3 pos;
  glm::vec3 normal;
  glm::vec2 uv;
};

#endif