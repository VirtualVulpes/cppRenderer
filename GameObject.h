//
// Created by sjfochs on 2/12/26.
//

#ifndef CPPRENDERER_GAMEOBJECT_H
#define CPPRENDERER_GAMEOBJECT_H
#include "Mesh.h"
#include "shader.h"
#include <glm/glm.hpp>


class GameObject
{
    GameObject(glm::vec3 pos, Mesh mesh, Shader shader);
private:
    glm::vec3 pos_;
    Mesh mesh_;
    Shader shader_;
};


#endif //CPPRENDERER_GAMEOBJECT_H