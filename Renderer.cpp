#include "Renderer.h"

#include "glad/glad.h"
#include <GL/gl.h>

Renderer::Renderer() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_CULL_FACE);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Renderer::Clear() {
  glClearColor(0.471f, 0.655f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const GameObject& object, Camera* camera) const {
  Shader* shader = object.GetShader();
  shader->Use();

  shader->SetMat4("view", camera->GetView());
  if (camera->IsProjectionDirty())
    shader->SetMat4("projection", camera->GetProjection());

  object.Draw();
}

void Renderer::Draw(const std::vector<std::unique_ptr<GameObject> >& objects, Camera* camera) const {
  for (const auto& object : objects) {
    Shader* shader = object->GetShader();
    shader->Use();

    shader->SetMat4("view", camera->GetView());
    if ( camera->IsProjectionDirty())
      shader->SetMat4("projection", camera->GetProjection());

    object->Draw();
  }
}