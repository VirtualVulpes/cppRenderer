#ifndef CPPRENDERER_GAMEOBJECT_H
#define CPPRENDERER_GAMEOBJECT_H

#include "Transform.h"
#include "Mesh.h"
#include "Texture.h"
#include "shader.h"

class Texture;

class GameObject {
public:
  GameObject(const Transform& transform, Mesh* mesh, Texture* texture, Shader* shader);

  void Draw() const;
  void Rotate(glm::vec3 rotation);

private:
  Transform transform_;
  Mesh* mesh_;
  Texture* texture_;
  Shader* shader_;

  glm::mat4 ComputeModelMatrix() const;
};

#endif
