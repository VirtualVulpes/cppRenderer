#ifndef CPPRENDERER_WINDOWCONTEXT_H
#define CPPRENDERER_WINDOWCONTEXT_H

#include "Camera.h"
#include "shader.h"

struct WindowContext {
  Camera* camera;
  Shader* shader;
};

#endif