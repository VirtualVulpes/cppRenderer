#ifndef CPPRENDERER_RENDERABLE_H
#define CPPRENDERER_RENDERABLE_H

#include "Material.h"

enum class RenderType {
  Opaque,
  Cutout,
  Transparent
};

struct Renderable {
  uint32_t mesh;
  uint32_t material;
  RenderType type{RenderType::Opaque};
};
#endif