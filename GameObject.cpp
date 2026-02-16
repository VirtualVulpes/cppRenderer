#include "GameObject.h"

#include <iostream>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

GameObject::GameObject(const Transform& transform, Mesh* mesh, Texture* texture, Shader* shader, Texture* texture_s, glm::vec3 tint)
  : transform_(transform)
  , mesh_(mesh)
  , texture_(texture)
  , shader_(shader)
  , texture_s_(texture_s)
  , tint_(tint) {
}

void GameObject::Draw() const {
  shader_->SetMat4("model", ComputeModelMatrix());
  shader_->SetVec3("tint", tint_);
  if (Texture::kDrawTextures) {
    texture_->Bind(0);
    if (texture_s_ != nullptr) {
      texture_s_->Bind(1);
    }
  } else {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, 1); // bind tile texture
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, 2); // bind tile specular
  }


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