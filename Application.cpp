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
#include "InputManager.h"
#include "Light.h"
#include "Renderable.h"
#include "renderer/Mesh.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"
#include "WindowContext.h"

constexpr int kInitialWidth{1280};
constexpr int kInitialHeight{720};

void CreateFloorMesh(Renderable* renderable);
void CreateLight(const Shader& lit, Light::LightType type, glm::vec3 color, float strength, Transform t);

Application::Application() {
  if (!glfwInit())
    throw std::runtime_error("Failed to initialize GLFW");

  window_ = std::make_unique<Window>(kInitialWidth, kInitialHeight, "Render Window");

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    throw std::runtime_error("Failed to initialize GLAD");

  renderer_ = std::make_unique<Renderer>();

  stbi_set_flip_vertically_on_load(true);
}

void Application::Run() {
  int width{kInitialWidth};
  int height{kInitialHeight};

  Camera camera{glm::vec3(2.0f, 1.0f, 8.0f), 0.0f, -90.0f, static_cast<float>(kInitialWidth)/kInitialHeight};

  WindowContext context{&camera};
  glfwSetWindowUserPointer(window_->GetHandle(), &context);

  InputManager input_manager{};

  Mesh cube_mesh{Geometry::Cube{}};
  Mesh plane_mesh{Geometry::Plane{}};

  Texture tile_tex{"textures/tile.png"};
  Texture tile_tex_s{"textures/tile_s.png"};
  Texture dirt_tex{"textures/dirt.png"};
  Texture dirt_tex_s{"textures/dirt_s.png"};
  Texture iron_tex{"textures/iron_block.png"};
  Texture iron_tex_s{"textures/iron_block_s.png"};
  Texture grass_tex{"textures/grass_block_top.png"};
  Texture white_tex{"textures/white.png"};
  Texture black_tex{"textures/black.png"};
  Texture noise_tex{"textures/noises.png"};
  noise_tex.SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
  noise_tex.SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

  Material default_mat{&renderer_->lit_shader_, &tile_tex, &tile_tex_s};
  Material dirt_mat{&renderer_->lit_shader_, &dirt_tex, &dirt_tex_s};
  Material iron_mat{&renderer_->lit_shader_, &iron_tex, &iron_tex_s};
  Material grass_mat{&renderer_->lit_shader_, &grass_tex, &black_tex};
  Material light_mat{&renderer_->unlit_shader_, &white_tex, &white_tex};

  Renderable dirt_rend{&cube_mesh, &dirt_mat};
  Renderable iron_rend{&cube_mesh, &iron_mat};
  Renderable grass_rend{&plane_mesh, &grass_mat};
  Renderable light_rend{&cube_mesh, &light_mat};

  // Directional
  Transform t = {{0.0, 0.0, 0.0}, {1.0f, -1.0f, 1.0f}, {0.0, 0.0, 0.0}};
  CreateLight(renderer_->lit_shader_, Light::kDirectional, Color::White, 1, t);
  // Blue Point
  t = {{5.0, 3.0, 5.0}, {0.0f, 0.0f, 0.0f}, {0.2, 0.2, 0.2}};
  CreateLight(renderer_->lit_shader_, Light::kPoint, Color::Blue, 1.3, t);
  // White Point
  t = {{12.0, 3.0, 13}, {0.0f, 0.0f, 0.0f}, {0.2, 0.2, 0.2}};
  CreateLight(renderer_->lit_shader_, Light::kPoint, Color::White, 1.0, t);
  // Red Point
  t = {{11.0, 1.0, 4.0}, {0.0f, 0.0f, 0.0f}, {0.2, 0.2, 0.2}};
  CreateLight(renderer_->lit_shader_, Light::kPoint, Color::Red, 0.9, t);
  // Green Point
  t = {{7.0, 2.0, 10.0}, {0.0f, 0.0f, 0.0f}, {0.2, 0.2, 0.2}};
  CreateLight(renderer_->lit_shader_, Light::kPoint, Color::Green, 0.8, t);
  // White Spot
  t = {{8.0, 4.0, 8.0}, {-0.2f, -1.0f, -0.6f}, {0.2, 0.2, 0.2}};
  CreateLight(renderer_->lit_shader_, Light::kSpot, Color::White, 0.8, t);

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

    InputState input = input_manager.GetInput(*window_);
    if (input.keys.escape)
      Close();

    camera.Update(input, delta_time);
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
    noise_tex.Bind(2);

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

void Application::CreateLight(const Shader& lit, Light::LightType type, glm::vec3 color, float intensity, Transform t) const {
  static int numPoints{0};
  lit.Use();

  switch (type) {
    case Light::kDirectional:
      lit.SetVec3("dirLight.direction", t.rotation);
      lit.SetVec3("dirLight.ambient", color * intensity * 0.02f);
      lit.SetVec3("dirLight.diffuse", color * intensity * 0.1f);
      lit.SetVec3("dirLight.specular", color * intensity * 0.2f);
      renderer_->directional_lights_.push_back(std::make_unique<Light::DirectionalLight>(t.rotation, color, intensity));
      return;
    case Light::kPoint:
      lit.SetVec3(std::format("pointLights[{}].position", numPoints), t.pos);
      lit.SetFloat(std::format("pointLights[{}].constant", numPoints), 1.0f);
      lit.SetFloat(std::format("pointLights[{}].linear", numPoints), 0.09f);
      lit.SetFloat(std::format("pointLights[{}].quadratic", numPoints), 0.032f);
      lit.SetVec3(std::format("pointLights[{}].ambient", numPoints), color * intensity * 0.2f);
      lit.SetVec3(std::format("pointLights[{}].diffuse", numPoints), color * intensity);
      lit.SetVec3(std::format("pointLights[{}].specular", numPoints), color * intensity);
      numPoints += 1;
      t.scale *= intensity;
      renderer_->point_lights_.push_back(std::make_unique<Light::PointLight>(t, color, intensity, 5.0f));
      return;
    case Light::kSpot:
      lit.SetVec3("spotLight.position", t.pos);
      lit.SetVec3("spotLight.direction", t.rotation);
      lit.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
      lit.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
      lit.SetVec3("spotLight.ambient", color * intensity * 0.2f);
      lit.SetVec3("spotLight.diffuse", color * intensity);
      lit.SetVec3("spotLight.specular", color * intensity);
      t.scale *= intensity;
      renderer_->spot_lights_.push_back(std::make_unique<Light::SpotLight>(t, color, intensity));
      return;
  }
}
