#include "GameObject.h"

#include <iostream>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

GameObject::GameObject(const Transform& transform, Mesh* mesh, Texture* texture, Shader* shader, Texture* texture_s)
  : transform_(transform)
  , mesh_(mesh)
  , texture_(texture)
  , shader_(shader)
  , texture_s_(texture_s) {
}

void GameObject::Draw() const {
  shader_->Use();
  shader_->SetMat4("model", ComputeModelMatrix());
  if (texture_s_ != nullptr) {
    texture_s_->Bind(1);
  }
  texture_->Bind(0);
  mesh_->Draw();
}

glm::mat4 GameObject::ComputeModelMatrix() const {
  return glm::translate(glm::mat4(1.0f), transform_.pos) *
         glm::toMat4(glm::quat(glm::radians(transform_.rotation))) *
         glm::scale(glm::mat4(1.0f), transform_.scale);
}

void GameObject::Move(const glm::vec3& delta) {
  transform_.Translate(delta);
}

void GameObject::Rotate(glm::vec3 rotation) {
  transform_.rotation += rotation;
}