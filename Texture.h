#ifndef CPPRENDERER_TEXTURE_H
#define CPPRENDERER_TEXTURE_H

#include <string_view>

#include "glad/glad.h"

class Texture {
public:
  Texture();
  explicit Texture(std::string_view path);
  ~Texture();

  void LoadFromFile(std::string_view path);
  unsigned int GetId() const { return id_; }
  void Bind(unsigned int unit) const;
  static void Bind(unsigned int unit, unsigned int id);

  void SetParameter(GLenum pname, int param) const;
  void SetData(int width, int height, unsigned char* data);

  static bool kDrawTextures;
private:
  unsigned int id_;

  int width_;
  int height_;
  int num_channels_;
};

#endif
