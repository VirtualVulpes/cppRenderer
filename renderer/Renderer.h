#ifndef CPPRENDERER_RENDERER_H
#define CPPRENDERER_RENDERER_H

#include <memory>
#include <vector>

#include "../Camera.h"
#include "../GameObject.h"
#include "../Light.h"
#include "../RenderContext.h"
#include "../Settings.h"

class Renderer {
public:
  Renderer(RenderContext context, Renderable renderables, const RenderSettings& settings);

  static void Clear();

  void PreDrawPass(Camera& camera);
  void DrawPass();
  void PostDrawPass(const Camera& camera);

  void EnableBackCull() { glEnable(GL_CULL_FACE); }
  void DisableBackCull() { glDisable(GL_CULL_FACE); }

  std::vector<std::unique_ptr<GameObject>> game_objects_;
  std::vector<std::unique_ptr<Light::DirectionalLight>> directional_lights_;
  std::vector<std::unique_ptr<Light::PointLight>> point_lights_;
  std::vector<std::unique_ptr<Light::SpotLight>> spot_lights_;

private:
  void Draw(const Renderable& renderable, const Transform& transform) const;
  RenderContext context_;
  Renderable light_debug_;
  const RenderSettings& settings_;
};

#endif
