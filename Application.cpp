#include "Application.h"

#include <format>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Camera.h"
#include "GameObject.h"
#include "Geometry.h"
#include "InputManager.h"
#include "Mesh.h"
#include "shader.h"
#include "Texture.h"
#include "WindowContext.h"

constexpr int kInitialWidth{1280};
constexpr int kInitialHeight{720};

void CreateFloorMesh(std::vector<std::unique_ptr<GameObject>>& objects, Mesh* mesh, Texture* texture, Shader* shader);

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
  Camera camera{glm::vec3(2.0f, 1.0f, 8.0f), 0.0f, -90.0f, static_cast<float>(kInitialWidth)/kInitialHeight};

  WindowContext context{&camera};
  glfwSetWindowUserPointer(window_->GetHandle(), &context);

  InputManager input_manager{};

  Mesh cubeMesh{Geometry::Cube{}};
  Mesh planeMesh{Geometry::Plane{}};

  Texture dirtTex{"textures/dirt.png"};
  Texture stoneTex{"textures/stone.png"};
  Texture grassTex{"textures/grass_block_top.png"};
  Texture whiteTex{"textures/white.png"};

  Shader unlit{"shaders/shader.vs", "shaders/unlit.fs"};
  Shader lit{"shaders/shader.vs", "shaders/lit.fs"};

  std::vector<std::unique_ptr<GameObject>> objects;
  // dirt cube
  Transform t{glm::vec3(8.0f, 0.0f, 9.0f)};
  objects.push_back(std::make_unique<GameObject>(t, &cubeMesh, &dirtTex, &lit));
  // stone cube
  t.pos = {7.0f, 1.0f, 6.0f};
  objects.push_back(std::make_unique<GameObject>(t, &cubeMesh, &stoneTex, &lit));
  // light cube
  t.pos = {8.0f, 2.0f, 4.0f};
  t.scale = {0.2f, 0.2f, 0.2f};
  auto light = std::make_unique<GameObject>(t, &cubeMesh, &whiteTex, &unlit);
  GameObject* lightPtr = light.get();
  objects.push_back(std::move(light));
  // floor plane
  CreateFloorMesh(objects, &planeMesh, &grassTex, &lit);

  glm::vec3 light_color {0.7f, 0.7f, 0.7f};

  lit.Use();
  //lit.SetVec3("tint", glm::vec3(1.0, 1.0, 1.0));
  lit.SetVec3("viewPos", camera.GetPosition());
  lit.SetVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
  lit.SetVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
  lit.SetVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
  lit.SetFloat("material.shininess", 32.0f);
  lit.SetVec3("light.position", t.pos);
  lit.SetVec3("light.ambient", glm::vec3(0.2f));
  lit.SetVec3("light.diffuse", glm::vec3(0.5f));
  lit.SetVec3("light.specular", glm::vec3(1.0f));

  unlit.Use();
  unlit.SetVec3("tint", light_color);

  float delta_time{0.0f};
  float last_frame{0.0f};

  while (!should_close_) {
    window_->PollEvents();

    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    InputState input = input_manager.GetInput(*window_);
    if (input.keys.escape)
      Close();
    camera.Update(input, delta_time);

    lightPtr->Move(glm::vec3(cos(current_frame) * 0.02, 0.0f, sin(current_frame) * 0.02));
    lit.SetVec3("light.position", lightPtr->GetPosition());

    glm::vec3 light_color;
    light_color.x = sin(current_frame * 2.0f);
    light_color.y = sin(current_frame * 0.7f);
    light_color.z = sin(current_frame * 1.3f);
    glm::vec3 ambient_color = light_color * glm::vec3(0.2f);
    glm::vec3 diffuse_color = light_color * glm::vec3(0.5f);
    lit.SetVec3("light.ambient", ambient_color);
    lit.SetVec3("light.diffuse", diffuse_color);

    renderer_->Clear();
    renderer_->Draw(objects, &camera);

    camera.ClearProjectionDirtyFlag();
    window_->SwapBuffers();
  }
}

void Application::Close() {
  should_close_ = true;
}

void CreateFloorMesh(std::vector<std::unique_ptr<GameObject>>& objects, Mesh* mesh, Texture* texture, Shader* shader) {
  int width{16};
  int length{16};

  for (int z = 0; z < length; ++z) {
    for (int x = 0; x < width; ++x) {
      Transform t {glm::vec3(x, -0.5f, z), glm::vec3(-90.0f, 0.0f, 0.0f)};
      objects.push_back(std::make_unique<GameObject>(t, mesh, texture, shader));
    }
  }
}