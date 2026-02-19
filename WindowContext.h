#ifndef CPPRENDERER_WINDOWCONTEXT_H
#define CPPRENDERER_WINDOWCONTEXT_H

#include "Camera.h"
#include "Framebuffer.h"

struct WindowContext {
  Camera* camera;
  Framebuffer* framebuffer;
};

#endif