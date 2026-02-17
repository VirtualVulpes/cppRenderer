#ifndef CPPRENDERER_GAMEOBJECT_H
#define CPPRENDERER_GAMEOBJECT_H

#include "Renderable.h"
#include "Transform.h"

struct GameObject {
  Renderable* renderable;
  Transform transform;
};

#endif
