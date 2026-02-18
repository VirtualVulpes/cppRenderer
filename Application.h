#ifndef CPPRENDERER_APPLICATION_H
#define CPPRENDERER_APPLICATION_H

#include "ResourceHandler.h"
#include "renderer/Renderer.h"
#include "Window.h"
#include "input/InputManager.h"
#include "input/KeyBindings.h"

class Application {
public:
  Application();
  ~Application();

  void Run();
  void Close();

private:
  void CreateFloorMesh(Renderable* renderable) const;
  void CreateLight(Shader* lit, Light::LightType type, glm::vec3 color, float intensity, Transform t) const;
  void InitializeResources();
  void InitializeKeybinds();
  void HandleInput(Camera& camera, float delta_time);

  std::unique_ptr<Window> window_;

  ResourceHandler<Mesh> mesh_handler_{};
  ResourceHandler<Texture> texture_handler_{};
  ResourceHandler<Material> material_handler_{};
  ResourceHandler<Shader> shader_handler_{};

  std::unique_ptr<Renderer> renderer_{};
  bool should_close_{false};

  RenderSettings render_settings_{};

  KeyBindings key_bindings_{};
  InputManager input_manager_;
};

#endif