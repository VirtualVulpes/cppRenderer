#include "Renderer.h"

#include "glad/glad.h"
#include <GL/gl.h>

Renderer::Renderer() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_CULL_FACE);
}

void Renderer::Clear() {
  glClearColor(0.471f, 0.655f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
