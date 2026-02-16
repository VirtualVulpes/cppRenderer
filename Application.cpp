#include "Application.h"

#include <format>
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>

#include "stb_image.h"

#include "Camera.h"
#include "Framebuffer.h"
#include "GameObject.h"
#include "Geometry.h"
#include "InputManager.h"
#include "Mesh.h"
#include "shader.h"
#include "Texture.h"
#include "WindowContext.h"

constexpr int kInitialWidth{1280};
constexpr int kInitialHeight{720};

enum LightType {
  kDirectional,
  kPoint,
  kSpot
};

void CreateFloorMesh(std::vector<std::unique_ptr<GameObject>>& objects, Mesh* mesh, Texture* texture, Shader* shader, Texture* texture_s);
std::unique_ptr<GameObject> CreateLight(const Shader& lit, LightType type, glm::vec3 color, float strength, Transform t, Mesh* m, Texture* tex, Shader* unlit);

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

  std::vector<std::unique_ptr<GameObject>> objects;

  Shader unlit{"shaders/shader.vs", "shaders/unlit.fs"};

  Shader lit{"shaders/shader.vs", "shaders/lit.fs"};
  lit.Use();

  lit.SetVec3("viewPos", camera.GetPosition());
  lit.SetFloat("material.shininess", 32.0f);
  lit.SetInt("material.diffuse", 0);
  lit.SetInt("material.specular", 1);

  // Directional
  Transform t = {{0.0, 0.0, 0.0}, {1.0f, -1.0f, 1.0f}, {0.0, 0.0, 0.0}};
  auto light = CreateLight(lit, kDirectional, glm::vec3(1.0, 1.0, 1.0), 1, t, &cube_mesh, &white_tex, &unlit);
  objects.push_back(std::move(light));
  // Blue Point
  t = {{5.0, 3.0, 5.0}, {0.0f, 0.0f, 0.0f}, {0.2, 0.2, 0.2}};
  light = CreateLight(lit, kPoint, glm::vec3(0.0, 0.0, 1.0), 1.3, t, &cube_mesh, &white_tex, &unlit);
  GameObject* lightPtr = light.get();
  objects.push_back(std::move(light));
  // White Point
  t = {{12.0, 3.0, 13}, {0.0f, 0.0f, 0.0f}, {0.2, 0.2, 0.2}};
  light = CreateLight(lit, kPoint, glm::vec3(1.0, 1.0, 1.0), 1.0, t, &cube_mesh, &white_tex, &unlit);
  objects.push_back(std::move(light));
  // Red Point
  t = {{11.0, 1.0, 4.0}, {0.0f, 0.0f, 0.0f}, {0.2, 0.2, 0.2}};
  light = CreateLight(lit, kPoint, glm::vec3(1.0, 0.0, 0.0), 0.9, t, &cube_mesh, &white_tex, &unlit);
  objects.push_back(std::move(light));
  // Green Point
  t = {{7.0, 2.0, 10.0}, {0.0f, 0.0f, 0.0f}, {0.2, 0.2, 0.2}};
  light = CreateLight(lit, kPoint, glm::vec3(0.0, 1.0, 0.0), 0.8, t, &cube_mesh, &white_tex, &unlit);
  objects.push_back(std::move(light));
  // White Spot
  t = {{8.0, 4.0, 8.0}, {-0.2f, -1.0f, -0.6f}, {0.2, 0.2, 0.2}};
  light = CreateLight(lit, kSpot, glm::vec3(1.0, 1.0, 1.0), 1.6, t, &cube_mesh, &white_tex, &unlit);
  objects.push_back(std::move(light));

  Shader clouds{"shaders/clouds.vs", "shaders/clouds.fs"};
  clouds.Use();
  clouds.SetInt("screenTexture", 0);
  clouds.SetInt("depthTexture", 1);
  clouds.SetInt("noiseTexture", 2);
  clouds.SetVec2("zPlanes", camera.GetZPlanes());
  clouds.SetMat4("projMatrix", camera.GetProjection());

  // dirt cube
  t = {glm::vec3(8.0f, 0.0f, 9.0f)};
  objects.push_back(std::make_unique<GameObject>(t, &cube_mesh, &dirt_tex, &lit, &dirt_tex_s));
  // stone cube
  t.pos = {7.0f, 1.0f, 6.0f};
  objects.push_back(std::make_unique<GameObject>(t, &cube_mesh, &iron_tex, &lit, &iron_tex_s));
  // light cube
  t.pos = {8.0f, 2.0f, 4.0f};
  t.scale = {0.2f, 0.2f, 0.2f};
  // floor plane
  CreateFloorMesh(objects, &plane_mesh, &grass_tex, &lit, &black_tex );

  float delta_time{0.0f};
  float last_frame{0.0f};

  Mesh screen_quad{Geometry::Quad{}};
  Framebuffer msaa_framebuffer{width, height, true};
  Framebuffer framebuffer{width, height, false};

  while (!should_close_) {
    window_->PollEvents();

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
      clouds.SetVec2("screenSize", glm::vec2(width, height));
      clouds.SetMat4("projMatrix", camera.GetProjection());
    }

    lit.Use();
    lightPtr->Move(glm::vec3(cos(current_frame) * 0.02, 0.0f, sin(current_frame) * 0.02));
    lit.SetVec3("pointLights[0].position", lightPtr->GetPosition());

    msaa_framebuffer.Bind();
    renderer_->Clear();

    renderer_->Draw(objects, &camera);

    Framebuffer::Blit(msaa_framebuffer.GetId(), framebuffer.GetId(), width, height);

    clouds.Use();
    clouds.SetMat4("viewMatrix", camera.GetView());
    clouds.SetVec3("cameraPos", camera.GetPosition());

    Texture::Bind(0, framebuffer.GetColorAttachment());
    Texture::Bind(1, framebuffer.GetDepthAttachment());
    noise_tex.Bind(2);

    screen_quad.Draw();

    camera.ClearProjectionDirtyFlag();
    window_->SwapBuffers();
  }
}

void Application::Close() {
  should_close_ = true;
}

void CreateFloorMesh(std::vector<std::unique_ptr<GameObject>>& objects, Mesh* mesh, Texture* texture, Shader* shader, Texture* texture_s) {
  int width{16};
  int length{16};

  for (int z = 0; z < length; ++z) {
    for (int x = 0; x < width; ++x) {
      Transform t {glm::vec3(x, -0.5f, z), glm::vec3(-90.0f, 0.0f, 0.0f)};
      objects.push_back(std::make_unique<GameObject>(t, mesh, texture, shader, texture_s));
    }
  }
}

std::unique_ptr<GameObject> CreateLight(const Shader& lit, LightType type, glm::vec3 color, float strength, Transform t, Mesh* m, Texture* tex, Shader* unlit) {
  static int numPoints{0};

  switch (type) {
    case kDirectional:
      lit.SetVec3("dirLight.direction", t.rotation);
      lit.SetVec3("dirLight.ambient", color * strength * 0.02f);
      lit.SetVec3("dirLight.diffuse", color * strength * 0.1f);
      lit.SetVec3("dirLight.specular", color * strength * 0.2f);
      t.scale *= strength;
      return std::make_unique<GameObject>(t, m, tex, unlit, tex, color);
    case kPoint:
      lit.SetVec3(std::format("pointLights[{}].position", numPoints), t.pos);
      lit.SetFloat(std::format("pointLights[{}].constant", numPoints), 1.0f);
      lit.SetFloat(std::format("pointLights[{}].linear", numPoints), 0.09f);
      lit.SetFloat(std::format("pointLights[{}].quadratic", numPoints), 0.032f);
      lit.SetVec3(std::format("pointLights[{}].ambient", numPoints), color * strength * 0.2f);
      lit.SetVec3(std::format("pointLights[{}].diffuse", numPoints), color * strength);
      lit.SetVec3(std::format("pointLights[{}].specular", numPoints), color * strength);
      numPoints += 1;
      t.scale *= strength;
      return std::make_unique<GameObject>(t, m, tex, unlit, tex, color);
    case kSpot:
      lit.SetVec3("spotLight.position", t.pos);
      lit.SetVec3("spotLight.direction", t.rotation);
      lit.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
      lit.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
      lit.SetVec3("spotLight.ambient", color * strength * 0.2f);
      lit.SetVec3("spotLight.diffuse", color * strength);
      lit.SetVec3("spotLight.specular", color * strength);
      t.scale *= strength;
      return std::make_unique<GameObject>(t, m, tex, unlit, tex, color);
  }
}