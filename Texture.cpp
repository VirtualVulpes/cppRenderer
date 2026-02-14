#include "Texture.h"

#include <iostream>

#include "stb_image.h"
#include "glad/glad.h"

Texture::Texture() {
  glGenTextures(1, &id_);
}

Texture::Texture(std::string_view path)
  : Texture()
{
  LoadFromFile(path);
}

Texture::~Texture() {
  glDeleteTextures(1, &id_);
}

void Texture::LoadFromFile(std::string_view path) {
  Bind();

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  stbi_set_flip_vertically_on_load(true);

  unsigned char* data =
      stbi_load(path.data(), &width_, &height_, &num_channels_, 4);
  if (!data)
    std::cout << "Failed to load texture" << std::endl;

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);


  stbi_image_free(data);
}

void Texture::Bind() const {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture::SetMagFilter(GLenum filter) const {
  glBindTexture(GL_TEXTURE_2D, id_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
}

void Texture::SetMinFilter(GLenum filter) const {
  glBindTexture(GL_TEXTURE_2D, id_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
}

void Texture::SetData(int width, int height, unsigned char* data) {
  Bind();

  width_ = width;
  height_ = height;

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
}
