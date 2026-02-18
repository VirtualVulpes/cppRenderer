#ifndef CPPRENDERER_TEXTURELOADER_H
#define CPPRENDERER_TEXTURELOADER_H

#include <string_view>

#include "ResourceHandler.h"
#include "Texture.h"

class TextureLoader {
public:
  static void LoadFromFolder(std::string_view path, ResourceHandler<Texture> &texture_handler);
};

#endif
