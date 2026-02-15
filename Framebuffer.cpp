#include "Framebuffer.h"

#include <iostream>

#include "glad/glad.h"

Framebuffer::Framebuffer(int width, int height)
  :width_(width)
  ,height_(height) {
  Invalidate();
}

Framebuffer::~Framebuffer() {
  glDeleteFramebuffers(1, &fbo_);
  glDeleteTextures(1, &color_attachment_);
  glDeleteTextures(1, &depth_attachment_);
}

unsigned int Framebuffer::GetColorAttachment() const {
  return color_attachment_;
}

unsigned int Framebuffer::GetDepthAttachment() const {
  return depth_attachment_;
}

void Framebuffer::Invalidate() {
  if (fbo_) {
    glDeleteFramebuffers(1, &fbo_);
    glDeleteTextures(1, &color_attachment_);
    glDeleteTextures(1, &depth_attachment_);
  }

  glGenFramebuffers(1, &fbo_);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

  // Color
  glGenTextures(1, &color_attachment_);
  glBindTexture(GL_TEXTURE_2D, color_attachment_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment_, 0);

  // Depth
  glGenTextures(1, &depth_attachment_);
  glBindTexture(GL_TEXTURE_2D, depth_attachment_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width_, height_, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth_attachment_, 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    throw std::runtime_error("Framebuffer not complete");


  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(int width, int height) {
  width_ = width;
  height_ = height;
}

void Framebuffer::Bind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
  glEnable(GL_DEPTH_TEST);
}

void Framebuffer::Unbind() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDisable(GL_DEPTH_TEST);
}


