#ifndef CPPRENDERER_RENDERABLE_H
#define CPPRENDERER_RENDERABLE_H
#include "Material.h"
#include "Mesh.h"

struct Renderable {
  Mesh* mesh;
  uint32_t material;
};
#endif