#ifndef CPPRENDERER_RENDERCONTEXT_H
#define CPPRENDERER_RENDERCONTEXT_H

#include "ResourceHandler.h"
#include "TextureHandler.h"

struct RenderContext {
  ResourceHandler<Mesh>& meshes;
  TextureHandler& textures;
  ResourceHandler<Material>& materials;
  ResourceHandler<Shader>& shaders;
};

#endif