#ifndef CPPRENDERER_RENDERCONTEXT_H
#define CPPRENDERER_RENDERCONTEXT_H

#include "MaterialHandler.h"
#include "TextureHandler.h"

struct RenderContext {
  TextureHandler& textures;
  MaterialHandler& materials;
};

#endif