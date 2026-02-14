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

void Renderer::Draw(const GameObject& object) const {
  object.Draw();
}

void Renderer::Draw(const std::vector<std::unique_ptr<GameObject> >& objects) const {
  for (const auto& obj : objects)
    obj->Draw();
}

void Renderer::SetCamera(Camera& camera) const {
  active_shader_->SetMat4("view", camera.GetView());

  if (camera.IsProjectionDirty()) {
    active_shader_->SetMat4("projection", camera.GetProjection());
    camera.ClearProjectionDirtyFlag();
  }
}

void Renderer::UseShader(Shader* shader) {
  active_shader_ = shader;
  active_shader_->Use();
}
