//
// Created by sjfochs on 2/16/26.
//

#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const Transform& transform, Mesh* mesh, Texture* texture, Shader* shader, Texture* texture_s, glm::vec3 tint, float strength)
    : GameObject(transform, mesh, texture, shader, texture_s, tint)
    , strength_(strength)
{

}

void DirectionalLight::Draw() const
{
    shader_->SetVec3("dirLight.diffuse", tint_);
    shader_->SetVec3("dirLight.specular", tint_);
    shader_->SetVec3("dirLight.ambient", tint_);

    GameObject::Draw();
}
