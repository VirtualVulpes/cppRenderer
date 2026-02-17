#ifndef CPPRENDERER_APPLICATION_H
#define CPPRENDERER_APPLICATION_H

#include <memory>

#include "TextureHandler.h"
#include "renderer/Renderer.h"
#include "Window.h"

class Application {
public:
  Application();

  void Run();
  void Close();

private:
  void CreateFloorMesh(Renderable* renderable) const;
  void CreateLight(const Shader& lit, Light::LightType type, glm::vec3 color, float intensity, Transform t) const;

  std::unique_ptr<Window> window_;
  TextureHandler texture_handler_;
  std::unique_ptr<Renderer> renderer_;
  bool should_close_{false};
};

#endif
