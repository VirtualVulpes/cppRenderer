#ifndef CPPRENDERER_RENDERCONTEXT_H
#define CPPRENDERER_RENDERCONTEXT_H

#include "ResourceHandler.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"

struct RenderContext {
  ResourceHandler<Mesh>& meshes;
  ResourceHandler<Texture>& textures;
  ResourceHandler<Material>& materials;
  ResourceHandler<Shader>& shaders;
};

#endif