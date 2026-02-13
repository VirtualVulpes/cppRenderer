#include "Renderer.h"
#include "glad/glad.h"

#include <GL/gl.h>

Renderer::Renderer()
{
    glEnable(GL_DEPTH_TEST);
}

void Renderer::Clear()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
