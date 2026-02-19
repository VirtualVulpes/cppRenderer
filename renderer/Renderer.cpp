#include "Renderer.h"

#include "glad/glad.h"
#include <GL/gl.h>

#include "../Color.h"
#include "../RenderContext.h"

Renderer::Renderer(RenderContext context, Renderable light_debug, const RenderSettings& settings)
  : context_(context)
  , light_debug_(light_debug)
  , settings_(settings) {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
}

void Renderer::Clear() {
  glClearColor(Color::SkyBlue.r, Color::SkyBlue.g, Color::SkyBlue.b, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::PreDrawPass(Camera& camera) {
  Shader* lit = context_.shaders.GetPointer("lit");
  Shader* unlit = context_.shaders.GetPointer("unlit");
  Shader* clouds = context_.shaders.GetPointer("clouds");

  lit->Use();
  lit->SetVec3("viewPos", camera.GetPosition());
  lit->SetMat4("view", camera.GetViewMatrix());
  unlit->Use();
  unlit->SetMat4("view", camera.GetViewMatrix());
  if (camera.IsProjectionDirty()) {
    lit->Use();
    lit->SetMat4("projection", camera.GetProjectionMatrix());
    unlit->Use();
    unlit->SetMat4("projection", camera.GetProjectionMatrix());
    clouds->Use();
    clouds->SetMat4("projMatrix", camera.GetProjectionMatrix());
    clouds->SetVec2("zPlanes", camera.GetZPlanes());
    camera.ClearProjectionDirtyFlag();
  }
}

void Renderer::DrawPass() {
  if (settings_.drawWireframe)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Opaque Drawing
  for ( const auto& object : game_objects_) {
    if (object->renderable->type == RenderType::Opaque)
      Draw(*object->renderable, object->transform);
  }

  // Cutout Drawing
  DisableBackCull();
  glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

  for ( const auto& object : game_objects_) {
    if (object->renderable->type == RenderType::Cutout)
      Draw(*object->renderable, object->transform);
  }

  EnableBackCull();
  glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);

  if (settings_.debug.drawLights) {
    Shader* unlit = context_.shaders.GetPointer("unlit");
    unlit->Use();

    for (const auto& light : point_lights_) {
      unlit->SetVec3("tint", light->color);
      Draw(light_debug_, light->transform);
    }

    for (const auto& light : spot_lights_) {
      unlit->SetVec3("tint", light->color);
      Draw(light_debug_, light->transform);
    }
  }

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::PostDrawPass(const Camera& camera) {
  Shader* clouds = context_.shaders.GetPointer("clouds");

  clouds->Use();
  clouds->SetMat4("viewMatrix", camera.GetViewMatrix());
  clouds->SetVec3("cameraPos", camera.GetPosition());

  glDepthMask(GL_FALSE);
  Mesh screen_quad{Geometry::Quad{}};
  screen_quad.Draw();
  glDepthMask(GL_TRUE);
}

void Renderer::TransparentDrawPass() {
  if (settings_.drawWireframe)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  for (const auto& object : game_objects_) {
    if (object->renderable->type == RenderType::Transparent)
      Draw(*object->renderable, object->transform);
  }

  glDisable(GL_BLEND);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::Draw(const Renderable& renderable, const Transform& transform) const {
  Material* material = context_.materials.GetPointer(renderable.material);
  Shader* shader = context_.shaders.GetPointer(material->shader);
  shader->Use();
  shader->SetMat4("model", transform.GetModelMatrix());

  if (settings_.drawTextures) {
    Texture::Bind(0, material->diffuse_texture);
    Texture::Bind(1, material->specular_texture);
  } else {
    Texture::Bind(0, context_.textures.GetId("tile"));
    Texture::Bind(1, context_.textures.GetId("tile_s"));
  }

  context_.meshes.GetPointer(renderable.mesh)->Draw();
}