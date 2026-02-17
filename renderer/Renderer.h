#ifndef CPPRENDERER_RENDERER_H
#define CPPRENDERER_RENDERER_H

#include <memory>

#include "../Camera.h"
#include "../GameObject.h"
#include "../Light.h"

class Renderer {
public:
  Renderer();

  static void Clear();

  void PreDrawPass(const Camera& camera) const;
  void DrawPass() const;
  void PostDrawPass(const Camera& camera) const;

  std::vector<std::unique_ptr<GameObject>> game_objects_;
  std::vector<std::unique_ptr<Light::DirectionalLight>> directional_lights_;
  std::vector<std::unique_ptr<Light::PointLight>> point_lights_;
  std::vector<std::unique_ptr<Light::SpotLight>> spot_lights_;

  Shader unlit_shader_;
  Shader lit_shader_;
  Shader cloud_shader_;

private:
  void Draw(const Renderable& renderable, const Transform& transform) const;
};

#endif
