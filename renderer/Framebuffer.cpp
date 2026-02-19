#include "Framebuffer.h"

#include <iostream>

#include "glad/glad.h"

Framebuffer::Framebuffer(int width, int height, bool msaa)
  : width_(width)
  , height_(height)
  , is_msaa_(msaa) {
  if (!msaa) {
    Invalidate();
  } else {
    InvalidateMsaa();
  }
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

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment_, 0);

  // Depth
  glGenTextures(1, &depth_attachment_);
  glBindTexture(GL_TEXTURE_2D, depth_attachment_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width_, height_, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, nullptr);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_attachment_, 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    throw std::runtime_error("Framebuffer not complete");

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::InvalidateMsaa() {
  if (fbo_) {
    glDeleteFramebuffers(1, &fbo_);
    glDeleteTextures(1, &color_attachment_);
    glDeleteTextures(1, &depth_attachment_);
  }

  glGenFramebuffers(1, &fbo_);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

  // Color
  glGenTextures(1, &color_attachment_);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, color_attachment_);

  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 16, GL_RGBA8, width_, height_, GL_TRUE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, color_attachment_, 0);

  // Depth
  glGenTextures(1, &depth_attachment_);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, depth_attachment_);

  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 16, GL_DEPTH_COMPONENT24, width_, height_, GL_TRUE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, depth_attachment_, 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    throw std::runtime_error("Framebuffer not complete");

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(int width, int height) {
  width_ = width;
  height_ = height;

  if (is_msaa_) {
    InvalidateMsaa();
  } else {
    Invalidate();
  }
}

void Framebuffer::Bind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
}

void Framebuffer::Unbind() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Blit(int read_id, int write_id, int width, int height) {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, read_id);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, write_id);
  glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}
