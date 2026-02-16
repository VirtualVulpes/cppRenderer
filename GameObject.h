#ifndef CPPRENDERER_GAMEOBJECT_H
#define CPPRENDERER_GAMEOBJECT_H

#include "Camera.h"
#include "Transform.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"

class GameObject {
public:
  GameObject(const Transform& transform, Mesh* mesh, Texture* texture, Shader* shader, Texture* texture_s, glm::vec3 tint = {1.0, 1.0, 1.0});

  virtual void Draw() const;
  void Rotate(glm::vec3 rotation);
  Shader* GetShader() const { return shader_; }
  const glm::vec3& GetPosition() const { return transform_.pos; }
  void Move(const glm::vec3& delta);

protected:
  Shader* shader_{};
  glm::vec3 tint_;

private:
  Transform transform_;
  Mesh* mesh_{};
  Texture* texture_{};
  Texture* texture_s_{};

  glm::mat4 ComputeModelMatrix() const;
};

#endif
