#ifndef CPPRENDERER_RENDERER_H
#define CPPRENDERER_RENDERER_H

#include <memory>

#include "../Camera.h"
#include "../GameObject.h"
#include "../Light.h"
#include "../RenderContext.h"

class Renderer {
public:
  Renderer(RenderContext context, Renderable renderables);

  static void Clear();

  void PreDrawPass(Camera& camera) const;
  void DrawPass() const;
  void PostDrawPass(const Camera& camera) const;

  std::vector<std::unique_ptr<GameObject>> game_objects_;
  std::vector<std::unique_ptr<Light::DirectionalLight>> directional_lights_;
  std::vector<std::unique_ptr<Light::PointLight>> point_lights_;
  std::vector<std::unique_ptr<Light::SpotLight>> spot_lights_;

private:
  void Draw(const Renderable& renderable, const Transform& transform) const;
  RenderContext context_;
  Renderable light_debug_;
};

#endif
