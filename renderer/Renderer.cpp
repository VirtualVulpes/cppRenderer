#include "Renderer.h"

#include "glad/glad.h"
#include <GL/gl.h>

#include "../Color.h"
#include "../RenderContext.h"

constexpr bool kDrawTextures{false};

Renderer::Renderer(RenderContext context)
  : unlit_shader_("shaders/shader.vs", "shaders/unlit.fs")
  , lit_shader_("shaders/shader.vs", "shaders/lit.fs")
  , cloud_shader_("shaders/clouds.vs", "shaders/clouds.fs")
  , context_(context) {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_CULL_FACE);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  lit_shader_.Use();
  lit_shader_.SetInt("material.diffuse", 0);
  lit_shader_.SetInt("material.specular", 1);
  lit_shader_.SetFloat("material.shininess", 32.0f);

  cloud_shader_.Use();
  cloud_shader_.SetInt("screenTexture", 0);
  cloud_shader_.SetInt("depthTexture", 1);
  cloud_shader_.SetInt("noiseTexture", 2);
}

void Renderer::Clear() {
  glClearColor(Color::SkyBlue.r, Color::SkyBlue.g, Color::SkyBlue.b, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::PreDrawPass(Camera& camera) const {
  lit_shader_.Use();
  lit_shader_.SetVec3("viewPos", camera.GetPosition());
  lit_shader_.SetMat4("view", camera.GetViewMatrix());
  if (camera.IsProjectionDirty()) {
    lit_shader_.SetMat4("projection", camera.GetProjectionMatrix());
    cloud_shader_.Use();
    cloud_shader_.SetMat4("projMatrix", camera.GetProjectionMatrix());
    cloud_shader_.SetVec2("zPlanes", camera.GetZPlanes());
    camera.ClearProjectionDirtyFlag();
  }
}

void Renderer::DrawPass() const {
  for ( const auto& object : game_objects_) {
    Draw(*object->renderable, object->transform);
  }
}

void Renderer::PostDrawPass(const Camera& camera) const {
  cloud_shader_.Use();
  cloud_shader_.SetMat4("viewMatrix", camera.GetViewMatrix());
  cloud_shader_.SetVec3("cameraPos", camera.GetPosition());
}

void Renderer::Draw(const Renderable& renderable, const Transform& transform) const {
  Material* material = renderable.material;
  material->shader->Use();
  material->shader->SetMat4("model", transform.GetModelMatrix());

  if (kDrawTextures && material->diffuse_texture && material->specular_texture) {
    material->diffuse_texture->Bind(0);
    material->specular_texture->Bind(1);
  } else {
    // bind tile texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, context_.textures.GetId("tile"));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, context_.textures.GetId("tile_s"));
  }

  renderable.mesh->Draw();
}