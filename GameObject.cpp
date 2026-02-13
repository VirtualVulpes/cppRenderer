//
// Created by sjfochs on 2/12/26.
//

#include "GameObject.h"

GameObject::GameObject(glm::vec3 pos, Mesh mesh, Shader shader)
    : pos_(pos)
    , mesh_(mesh)
    , shader_(shader)
{
}