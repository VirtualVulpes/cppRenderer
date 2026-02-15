#ifndef CPPRENDERER_RENDERER_H
#define CPPRENDERER_RENDERER_H

#include <memory>

#include "Camera.h"
#include "GameObject.h"

class Renderer {
public:
  Renderer();

  void Clear();
  void Draw(const GameObject& object, Camera* camera) const;
  void Draw(const std::vector<std::unique_ptr<GameObject>>& objects, Camera* camera) const;
  void SetCamera(Camera& camera) const;
  void CreateFrameBuffer();
};

#endif
