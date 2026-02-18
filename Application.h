#ifndef CPPRENDERER_APPLICATION_H
#define CPPRENDERER_APPLICATION_H

#include "ResourceHandler.h"
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
  void CreateLight(Shader* lit, Light::LightType type, glm::vec3 color, float intensity, Transform t) const;
  void InitializeResources();

  std::unique_ptr<Window> window_{};

  ResourceHandler<Mesh> mesh_handler_{};
  TextureHandler texture_handler_{};
  ResourceHandler<Material> material_handler_{};
  ResourceHandler<Shader> shader_handler_{};

  std::unique_ptr<Renderer> renderer_{};
  bool should_close_{false};

  RenderSettings render_settings_{};
};

#endif