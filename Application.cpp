#include "Application.h"

#include <format>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Camera.h"
#include "Color.h"
#include "renderer/Framebuffer.h"
#include "GameObject.h"
#include "Geometry.h"
#include "input/InputManager.h"
#include "Light.h"
#include "Renderable.h"
#include "TextureLoader.h"
#include "renderer/Mesh.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"
#include "WindowContext.h"

void CreateFloorMesh(Renderable* renderable);
void CreateLight(const Shader& lit, Light::LightType type, glm::vec3 color, float strength, Transform t);

Application::Application()
  : window_(std::make_unique<Window>(WindowCoordinates{1280, 720, 200, 200}, "Render Window"))
  , input_manager_(key_bindings_) {

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    throw std::runtime_error("Failed to initialize GLAD");

  InitializeResources();
  InitializeKeybinds();

  RenderContext context{mesh_handler_, texture_handler_, material_handler_, shader_handler_};
  Renderable light_rend{mesh_handler_.GetId("cube"), material_handler_.GetId("light")};
  renderer_ = std::make_unique<Renderer>(context, light_rend, render_settings_);

  stbi_set_flip_vertically_on_load(true);
}

Application::~Application()
{
  renderer_.reset();
  shader_handler_.Clear();
  material_handler_.Clear();
  mesh_handler_.Clear();
  texture_handler_.Clear();
  window_.reset();
  glfwTerminate();
}

void Application::Run() {
  int width{};
  int height{};
  glfwGetFramebufferSize(window_->GetHandle(), &width, &height);

  Camera camera{glm::vec3(2.0f, 1.0f, 8.0f), 0.0f, -90.0f, static_cast<float>(width)/height};

  WindowContext context{&camera};
  glfwSetWindowUserPointer(window_->GetHandle(), &context);

  Transform t = {{0.0, 0.0, 0.0}, {1.0f, -1.0f, 1.0f}, {0.0, 0.0, 0.0}};
  CreateLight(shader_handler_.GetPointer("lit"), Light::Directional, Color::White, 1, t);
  t = {{5.0, 3.0, 5.0}, {0.0f, 0.0f, 0.0f}, {0.2, 0.2, 0.2}};
  CreateLight(shader_handler_.GetPointer("lit"), Light::Point, Color::Blue, 1.6 , t);
  t = {{12.0, 3.0, 13}, {0.0f, 0.0f, 0.0f}, {0.2, 0.2, 0.2}};
  CreateLight(shader_handler_.GetPointer("lit"), Light::Point, Color::White, 1.0, t);
  t = {{11.0, 1.0, 4.0}, {0.0f, 0.0f, 0.0f}, {0.2, 0.2, 0.2}};
  CreateLight(shader_handler_.GetPointer("lit"), Light::Point, Color::Red, 1.2, t);
  t = {{7.0, 2.0, 10.0}, {0.0f, 0.0f, 0.0f}, {0.2, 0.2, 0.2}};
  CreateLight(shader_handler_.GetPointer("lit"), Light::Point, Color::Green, 0.6, t);
  t = {{8.0, 4.0, 8.0}, {-0.2f, -1.0f, -0.6f}, {0.2, 0.2, 0.2}};
  CreateLight(shader_handler_.GetPointer("lit"), Light::Spot, Color::White, 0.8, t);

  Renderable dirt_rend{mesh_handler_.GetId("cube"), material_handler_.GetId("dirt")};
  Renderable iron_rend{mesh_handler_.GetId("cube"), material_handler_.GetId("iron")};
  Renderable grass_rend{mesh_handler_.GetId("plane"), material_handler_.GetId("grass")};
  Renderable short_grass_rend{mesh_handler_.GetId("cross_plane"), material_handler_.GetId("short_grass"), RenderType::Cutout};

  // dirt cube
  t = {glm::vec3(8.0f, 0.0f, 9.0f)};
  renderer_->game_objects_.push_back(std::make_unique<GameObject>(&dirt_rend, t));
  // iron cube
  t.pos = {7.0f, 1.0f, 6.0f};
  renderer_->game_objects_.push_back(std::make_unique<GameObject>(&iron_rend, t));
  // floor plane
  CreateFloorMesh(&grass_rend);
  // short grass
  t.pos = {7.0f, 0.0f, 7.0f};
  renderer_->game_objects_.push_back(std::make_unique<GameObject>(&short_grass_rend, t));
  t.pos = {4.0f, 0.0f, 4.0f};
  renderer_->game_objects_.push_back(std::make_unique<GameObject>(&short_grass_rend, t));
  t.pos = {12.0f, 0.0f, 5.0f};
  renderer_->game_objects_.push_back(std::make_unique<GameObject>(&short_grass_rend, t));

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
    texture_handler_.GetPointer("noises")->Bind(2);

    screen_quad.Draw();

    window_->SwapBuffers();
  }
}

void Application::Close() {
  should_close_ = true;
}

void Application::CreateFloorMesh(Renderable* renderable) const {
  constexpr int length{16};
  constexpr int width{16};

  for (int z = 0; z < length; ++z) {
    for (int x = 0; x < width; ++x) {
      Transform t {glm::vec3(x, -0.5f, z), glm::vec3(-90.0f, 0.0f, 0.0f)};
      renderer_->game_objects_.push_back(std::make_unique<GameObject>(renderable, t));
    }
  }
}

void Application::CreateLight(Shader* lit, Light::LightType type, glm::vec3 color, float intensity, Transform t) const {
  static int num_dir_lights{0};
  static int num_point_lights{0};
  static int num_spot_lights{0};

  lit->Use();

  switch (type) {
    case Light::Directional:
      lit->SetVec3(std::format("dirLights[{}].direction", num_dir_lights), t.rotation);
      lit->SetVec3(std::format("dirLights[{}].ambient", num_dir_lights), color * intensity * 0.02f);
      lit->SetVec3(std::format("dirLights[{}].diffuse", num_dir_lights), color * intensity * 0.1f);
      lit->SetVec3(std::format("dirLights[{}].specular", num_dir_lights), color * intensity * 0.2f);
      num_dir_lights++;
      lit->SetInt("num_dir_lights", num_dir_lights);
      renderer_->directional_lights_.push_back(std::make_unique<Light::DirectionalLight>(t.rotation, color, intensity));
      return;
    case Light::Point:
      lit->SetVec3(std::format("pointLights[{}].position", num_point_lights), t.pos);
      lit->SetFloat(std::format("pointLights[{}].constant", num_point_lights), 1.0f);
      lit->SetFloat(std::format("pointLights[{}].linear", num_point_lights), 0.09f);
      lit->SetFloat(std::format("pointLights[{}].quadratic", num_point_lights), 0.032f);
      lit->SetVec3(std::format("pointLights[{}].ambient", num_point_lights), color * intensity * 0.2f);
      lit->SetVec3(std::format("pointLights[{}].diffuse", num_point_lights), color * intensity);
      lit->SetVec3(std::format("pointLights[{}].specular", num_point_lights), color * intensity);
      num_point_lights++;
      lit->SetInt("num_point_lights", num_point_lights);
      t.scale *= intensity;
      renderer_->point_lights_.push_back(std::make_unique<Light::PointLight>(t, color, intensity, 5.0f));
      return;
    case Light::Spot:
      lit->SetVec3(std::format("spotLights[{}].position", num_spot_lights), t.pos);
      lit->SetVec3(std::format("spotLights[{}].direction", num_spot_lights), t.rotation);
      lit->SetFloat(std::format("spotLights[{}].cutOff", num_spot_lights), glm::cos(glm::radians(12.5f)));
      lit->SetFloat(std::format("spotLights[{}].outerCutOff", num_spot_lights), glm::cos(glm::radians(17.5f)));
      lit->SetVec3(std::format("spotLights[{}].ambient", num_spot_lights), color * intensity * 0.2f);
      lit->SetVec3(std::format("spotLights[{}].diffuse", num_spot_lights), color * intensity);
      lit->SetVec3(std::format("spotLights[{}].specular", num_spot_lights), color * intensity);
      num_spot_lights++;
      lit->SetInt("num_spot_lights", num_spot_lights);
      t.scale *= intensity;
      renderer_->spot_lights_.push_back(std::make_unique<Light::SpotLight>(t, color, intensity));
      return;
  }
}

void Application::InitializeResources() {
  mesh_handler_.Create("cube", Geometry::Cube{});
  mesh_handler_.Create("plane", Geometry::Plane{});
  mesh_handler_.Create("cross_plane", Geometry::CrossPlane{});

  TextureLoader::LoadFromFolder("textures/", texture_handler_);
  const Texture* noises = texture_handler_.GetPointer("noises");
  noises->Bind(0);
  noises->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
  noises->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
  noises->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  noises->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  shader_handler_.Create("lit", "shaders/shader.vs", "shaders/lit.fs");
  shader_handler_.Create("unlit", "shaders/shader.vs", "shaders/unlit.fs");
  shader_handler_.Create("clouds", "shaders/clouds.vs", "shaders/clouds.fs");

  const Shader* lit = shader_handler_.GetPointer("lit");
  lit->Use();
  lit->SetInt("material.diffuse", 0);
  lit->SetInt("material.specular", 1);
  lit->SetFloat("material.shininess", 32.0f);

  const Shader* unlit = shader_handler_.GetPointer("unlit");
  unlit->Use();
  unlit->SetInt("material.diffuse", 0);

  const Shader* clouds = shader_handler_.GetPointer("clouds");
  clouds->Use();
  clouds->SetInt("screenTexture", 0);
  clouds->SetInt("depthTexture", 1);
  clouds->SetInt("noiseTexture", 2);

  material_handler_.Create("default", shader_handler_.GetId("lit"), texture_handler_.GetId("tile"), texture_handler_.GetId("tile_s"));
  material_handler_.Create("dirt", shader_handler_.GetId("lit"), texture_handler_.GetId("dirt"), texture_handler_.GetId("dirt_s"));
  material_handler_.Create("iron", shader_handler_.GetId("lit"), texture_handler_.GetId("iron_block"), texture_handler_.GetId("iron_block_s"));
  material_handler_.Create("grass", shader_handler_.GetId("lit"), texture_handler_.GetId("grass_block_top"), texture_handler_.GetId("black"));
  material_handler_.Create("short_grass", shader_handler_.GetId("lit"), texture_handler_.GetId("short_grass"), texture_handler_.GetId("black"));
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
  key_bindings_.keyboard.emplace(Action::ToggleFullscreen, GLFW_KEY_F11);

  key_bindings_.keyboard.emplace(Action::Quit, GLFW_KEY_ESCAPE);
}

void Application::HandleInput(Camera& camera, float delta_time) {
  const InputState input = input_manager_.GetInput(*window_);
  if (input_manager_.IsJustPressed(&InputState::Keys::quit))
    Close();
  if (input_manager_.IsJustPressed(&InputState::Keys::toggleLightDebug))
    render_settings_.debug.drawLights = !render_settings_.debug.drawLights;
  if (input_manager_.IsJustPressed(&InputState::Keys::toggleWireframe))
    render_settings_.drawWireframe = !render_settings_.drawWireframe;
  if (input_manager_.IsJustPressed(&InputState::Keys::toggleDrawTextures))
    render_settings_.drawTextures = !render_settings_.drawTextures;
  if (input_manager_.IsJustPressed(&InputState::Keys::toggleFullscreen))
    window_->ToggleFullscreen();

  camera.Update(input, delta_time);
}