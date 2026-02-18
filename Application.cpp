#include "Application.h"

#include <format>
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>

#include "stb_image.h"

#include "Camera.h"
#include "Color.h"
#include "renderer/Framebuffer.h"
#include "GameObject.h"
#include "Geometry.h"
#include "input/InputManager.h"
#include "Light.h"
#include "Renderable.h"
#include "renderer/Mesh.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"
#include "WindowContext.h"

void CreateFloorMesh(Renderable* renderable);
void CreateLight(const Shader& lit, Light::LightType type, glm::vec3 color, float strength, Transform t);

Application::Application(Window& window)
  : window_(&window)
  , input_manager_(key_bindings_) {

  InitializeResources();
  InitializeKeybinds();

  RenderContext context{mesh_handler_, texture_handler_, material_handler_, shader_handler_};
  Renderable light_rend{mesh_handler_.GetId("cube"), material_handler_.GetId("light")};
  renderer_ = std::make_unique<Renderer>(context, light_rend, render_settings_);

  stbi_set_flip_vertically_on_load(true);
}

void Application::Run() {
  int width{};
  int height{};
  glfwGetFramebufferSize(window_->GetHandle(), &width, &height);

  Camera camera{glm::vec3(2.0f, 1.0f, 8.0f), 0.0f, -90.0f, static_cast<float>(width)/height};

  WindowContext context{&camera};
  glfwSetWindowUserPointer(window_->GetHandle(), &context);

  Renderable dirt_rend{mesh_handler_.GetId("cube"), material_handler_.GetId("dirt")};
  Renderable iron_rend{mesh_handler_.GetId("cube"), material_handler_.GetId("iron")};
  Renderable grass_rend{mesh_handler_.GetId("plane"), material_handler_.GetId("grass")};

  Transform t = {{0.0, 0.0, 0.0}, {1.0f, -1.0f, 1.0f}, {0.0, 0.0, 0.0}};
  CreateLight(shader_handler_.GetPointer("lit"), Light::kDirectional, Color::White, 1, t);
  t = {{5.0, 3.0, 5.0}, {0.0f, 0.0f, 0.0f}, {0.2, 0.2, 0.2}};
  CreateLight(shader_handler_.GetPointer("lit"), Light::kPoint, Color::Blue, 1.6 , t);
  t = {{12.0, 3.0, 13}, {0.0f, 0.0f, 0.0f}, {0.2, 0.2, 0.2}};
  CreateLight(shader_handler_.GetPointer("lit"), Light::kPoint, Color::White, 1.0, t);
  t = {{11.0, 1.0, 4.0}, {0.0f, 0.0f, 0.0f}, {0.2, 0.2, 0.2}};
  CreateLight(shader_handler_.GetPointer("lit"), Light::kPoint, Color::Red, 1.2, t);
  t = {{7.0, 2.0, 10.0}, {0.0f, 0.0f, 0.0f}, {0.2, 0.2, 0.2}};
  CreateLight(shader_handler_.GetPointer("lit"), Light::kPoint, Color::Green, 0.6, t);
  t = {{8.0, 4.0, 8.0}, {-0.2f, -1.0f, -0.6f}, {0.2, 0.2, 0.2}};
  CreateLight(shader_handler_.GetPointer("lit"), Light::kSpot, Color::White, 0.8, t);

  // dirt cube
  t = {glm::vec3(8.0f, 0.0f, 9.0f)};
  renderer_->game_objects_.push_back(std::make_unique<GameObject>(&dirt_rend, t));
  // iron cube
  t.pos = {7.0f, 1.0f, 6.0f};
  renderer_->game_objects_.push_back(std::make_unique<GameObject>(&iron_rend, t));
  // floor plane
  CreateFloorMesh(&grass_rend);

  float delta_time{0.0f};
  float last_frame{0.0f};

  Mesh screen_quad{Geometry::Quad{}};
  Framebuffer msaa_framebuffer{width, height, true};
  Framebuffer framebuffer{width, height, false};

  while (!should_close_) {
    Window::PollEvents();

    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    HandleInput(camera, delta_time);

    if (camera.IsProjectionDirty()) {
      glfwGetFramebufferSize(window_->GetHandle(), &width, &height);
      framebuffer.Resize(width, height);
      msaa_framebuffer.Resize(width, height);
    }

    msaa_framebuffer.Bind();
    Renderer::Clear();
    renderer_->PreDrawPass(camera);
    renderer_->DrawPass();
    Framebuffer::Blit(msaa_framebuffer.GetId(), framebuffer.GetId(), width, height);
    renderer_->PostDrawPass(camera);

    Texture::Bind(0, framebuffer.GetColorAttachment());
    Texture::Bind(1, framebuffer.GetDepthAttachment());
    texture_handler_.Get("noises")->Bind(2);

    screen_quad.Draw();

    window_->SwapBuffers();
  }
}

void Application::Close() {
  should_close_ = true;
}

void Application::CreateFloorMesh(Renderable* renderable) const {
  int width{16};
  int length{16};

  for (int z = 0; z < length; ++z) {
    for (int x = 0; x < width; ++x) {
      Transform t {glm::vec3(x, -0.5f, z), glm::vec3(-90.0f, 0.0f, 0.0f)};
      renderer_->game_objects_.push_back(std::make_unique<GameObject>(renderable, t));
    }
  }
}

void Application::CreateLight(Shader* lit, Light::LightType type, glm::vec3 color, float intensity, Transform t) const {
  static int numPoints{0};
  lit->Use();

  switch (type) {
    case Light::kDirectional:
      lit->SetVec3("dirLight.direction", t.rotation);
      lit->SetVec3("dirLight.ambient", color * intensity * 0.02f);
      lit->SetVec3("dirLight.diffuse", color * intensity * 0.1f);
      lit->SetVec3("dirLight.specular", color * intensity * 0.2f);
      renderer_->directional_lights_.push_back(std::make_unique<Light::DirectionalLight>(t.rotation, color, intensity));
      return;
    case Light::kPoint:
      lit->SetVec3(std::format("pointLights[{}].position", numPoints), t.pos);
      lit->SetFloat(std::format("pointLights[{}].constant", numPoints), 1.0f);
      lit->SetFloat(std::format("pointLights[{}].linear", numPoints), 0.09f);
      lit->SetFloat(std::format("pointLights[{}].quadratic", numPoints), 0.032f);
      lit->SetVec3(std::format("pointLights[{}].ambient", numPoints), color * intensity * 0.2f);
      lit->SetVec3(std::format("pointLights[{}].diffuse", numPoints), color * intensity);
      lit->SetVec3(std::format("pointLights[{}].specular", numPoints), color * intensity);
      numPoints += 1;
      t.scale *= intensity;
      renderer_->point_lights_.push_back(std::make_unique<Light::PointLight>(t, color, intensity, 5.0f));
      return;
    case Light::kSpot:
      lit->SetVec3("spotLight.position", t.pos);
      lit->SetVec3("spotLight.direction", t.rotation);
      lit->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
      lit->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
      lit->SetVec3("spotLight.ambient", color * intensity * 0.2f);
      lit->SetVec3("spotLight.diffuse", color * intensity);
      lit->SetVec3("spotLight.specular", color * intensity);
      t.scale *= intensity;
      renderer_->spot_lights_.push_back(std::make_unique<Light::SpotLight>(t, color, intensity));
      return;
  }
}

void Application::InitializeResources() {
  mesh_handler_.Create("cube", Geometry::Cube{});
  mesh_handler_.Create("plane", Geometry::Plane{});

  texture_handler_.LoadFromFolder("textures/");
  texture_handler_.Get("noises")->Bind(0);
  texture_handler_.Get("noises")->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
  texture_handler_.Get("noises")->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
  texture_handler_.Get("noises")->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  texture_handler_.Get("noises")->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  shader_handler_.Create("lit", "shaders/shader.vs", "shaders/lit.fs");
  shader_handler_.Create("unlit", "shaders/shader.vs", "shaders/unlit.fs");
  shader_handler_.Create("clouds", "shaders/clouds.vs", "shaders/clouds.fs");

  Shader* lit = shader_handler_.GetPointer("lit");
  lit->Use();
  lit->SetInt("material.diffuse", 0);
  lit->SetInt("material.specular", 1);
  lit->SetFloat("material.shininess", 32.0f);

  Shader* unlit = shader_handler_.GetPointer("unlit");
  unlit->Use();
  unlit->SetInt("material.diffuse", 0);

  Shader* clouds = shader_handler_.GetPointer("clouds");
  clouds->Use();
  clouds->SetInt("screenTexture", 0);
  clouds->SetInt("depthTexture", 1);
  clouds->SetInt("noiseTexture", 2);

  material_handler_.Create("default", shader_handler_.GetId("lit"), texture_handler_.GetId("tile"), texture_handler_.GetId("tile_s"));
  material_handler_.Create("dirt", shader_handler_.GetId("lit"), texture_handler_.GetId("dirt"), texture_handler_.GetId("dirt_s"));
  material_handler_.Create("iron", shader_handler_.GetId("lit"), texture_handler_.GetId("iron_block"), texture_handler_.GetId("iron_block_s"));
  material_handler_.Create("grass", shader_handler_.GetId("lit"), texture_handler_.GetId("grass_block_top"), texture_handler_.GetId("black"));
  material_handler_.Create("light", shader_handler_.GetId("unlit"), texture_handler_.GetId("white"), texture_handler_.GetId("white"));
}

void Application::InitializeKeybinds() {
  key_bindings_.keyboard.emplace(Action::MoveForward, GLFW_KEY_W);
  key_bindings_.keyboard.emplace(Action::MoveBackward, GLFW_KEY_S);
  key_bindings_.keyboard.emplace(Action::MoveRight, GLFW_KEY_D);
  key_bindings_.keyboard.emplace(Action::MoveLeft, GLFW_KEY_A);
  key_bindings_.keyboard.emplace(Action::MoveUp, GLFW_KEY_SPACE);
  key_bindings_.keyboard.emplace(Action::MoveDown, GLFW_KEY_LEFT_SHIFT);

  key_bindings_.keyboard.emplace(Action::ToggleWireframe, GLFW_KEY_Z);
  key_bindings_.keyboard.emplace(Action::ToggleLightDebug, GLFW_KEY_X);
  key_bindings_.keyboard.emplace(Action::ToggleDrawTextures, GLFW_KEY_C);

  key_bindings_.keyboard.emplace(Action::Quit, GLFW_KEY_ESCAPE);
}

void Application::HandleInput(Camera& camera, float delta_time) {
  InputState input = input_manager_.GetInput(*window_);
  if (input_manager_.IsJustPressed(&InputState::Keys::quit))
    Close();
  if (input_manager_.IsJustPressed(&InputState::Keys::toggleLightDebug))
    render_settings_.debug.drawLights = !render_settings_.debug.drawLights;
  if (input_manager_.IsJustPressed(&InputState::Keys::toggleWireframe))
    render_settings_.drawWireframe = !render_settings_.drawWireframe;
  if (input_manager_.IsJustPressed(&InputState::Keys::toogleDrawTextures))
    render_settings_.drawTextures = !render_settings_.drawTextures;

  camera.Update(input, delta_time);
}