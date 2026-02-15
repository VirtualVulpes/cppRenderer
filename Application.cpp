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

void CreateFloorMesh(std::vector<std::unique_ptr<GameObject>>& objects, Mesh* mesh, Texture* texture, Shader* shader, Texture* texture_s);

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

  Mesh cube_mesh{Geometry::Cube{}};
  Mesh plane_mesh{Geometry::Plane{}};

  Texture dirt_tex{"textures/dirt.png"};
  Texture dirt_tex_s{"textures/dirt_s.png"};
  Texture iron_tex{"textures/iron_block.png"};
  Texture iron_tex_s{"textures/iron_block_s.png"};
  Texture grass_tex{"textures/grass_block_top.png"};
  Texture white_tex{"textures/white.png"};

  Shader unlit{"shaders/shader.vs", "shaders/unlit.fs"};
  Shader lit{"shaders/shader.vs", "shaders/lit.fs"};

  std::vector<std::unique_ptr<GameObject>> objects;
  // dirt cube
  Transform t{glm::vec3(8.0f, 0.0f, 9.0f)};
  objects.push_back(std::make_unique<GameObject>(t, &cube_mesh, &dirt_tex, &lit, &dirt_tex_s));
  // stone cube
  t.pos = {7.0f, 1.0f, 6.0f};
  objects.push_back(std::make_unique<GameObject>(t, &cube_mesh, &iron_tex, &lit, &iron_tex_s));
  // light cube
  t.pos = {8.0f, 2.0f, 4.0f};
  t.scale = {0.2f, 0.2f, 0.2f};
  auto light = std::make_unique<GameObject>(t, &cube_mesh, &white_tex, &unlit, &white_tex);
  GameObject* lightPtr = light.get();
  objects.push_back(std::move(light));
  // floor plane
  CreateFloorMesh(objects, &plane_mesh, &grass_tex, &lit, &grass_tex );

  glm::vec3 light_color {0.7f, 0.7f, 0.7f};

  lit.Use();
  lit.SetVec3("viewPos", camera.GetPosition());

  lit.SetFloat("material.shininess", 32.0f);

  // Direction light
  lit.SetVec3("dirLight.direction", glm::vec3(1.0f, -1.0f, 1.0f));
  lit.SetVec3("dirLight.ambient", glm::vec3(0.02f));
  lit.SetVec3("dirLight.diffuse", glm::vec3(0.1f));
  lit.SetVec3("dirLight.specular", glm::vec3(0.2f));

  // Point light
  lit.SetVec3("pointLights[0].position", glm::vec3(3.0f));
  lit.SetFloat("pointLights[0].constant", 1.0f);
  lit.SetFloat("pointLights[0].linear", 0.09f);
  lit.SetFloat("pointLights[0].quadratic", 0.032f);
  lit.SetVec3("pointLights[0].ambient", glm::vec3(0.2f));
  lit.SetVec3("pointLights[0].diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
  lit.SetVec3("pointLights[0].specular", glm::vec3(0.0f, 0.0f, 1.0f));

  lit.SetVec3("pointLights[1].position", glm::vec3(15.0f, 3.0f, 15.0f));
  lit.SetFloat("pointLights[1].constant", 1.0f);
  lit.SetFloat("pointLights[1].linear", 0.09f);
  lit.SetFloat("pointLights[1].quadratic", 0.032f);
  lit.SetVec3("pointLights[1].ambient", glm::vec3(0.2f));
  lit.SetVec3("pointLights[1].diffuse", glm::vec3(0.2f));
  lit.SetVec3("pointLights[1].specular", glm::vec3(1.0f));

  lit.SetVec3("pointLights[2].position", glm::vec3(15.0f, 2.0f, 0.0f));
  lit.SetFloat("pointLights[2].constant", 1.0f);
  lit.SetFloat("pointLights[2].linear", 0.09f);
  lit.SetFloat("pointLights[2].quadratic", 0.032f);
  lit.SetVec3("pointLights[2].ambient", glm::vec3(0.2f));
  lit.SetVec3("pointLights[2].diffuse", glm::vec3(0.9f, 0.1f, 0.2f));
  lit.SetVec3("pointLights[2].specular", glm::vec3(1.0f, 0.0f, 0.0f));

  lit.SetVec3("pointLights[3].position", glm::vec3(0.0f, 4.0f, 15.0f));
  lit.SetFloat("pointLights[3].constant", 1.0f);
  lit.SetFloat("pointLights[3].linear", 0.09f);
  lit.SetFloat("pointLights[3].quadratic", 0.032f);
  lit.SetVec3("pointLights[3].ambient", glm::vec3(0.2f));
  lit.SetVec3("pointLights[3].diffuse", glm::vec3(0.1f, 0.9f, 0.2f));
  lit.SetVec3("pointLights[3].specular", glm::vec3(0.0f, 1.0f, 0.0f));

  // Spot light
  lit.SetVec3("spotLight.position", {8.0, 4.0, 8.0});
  lit.SetVec3("spotLight.direction", glm::vec3(-0.2f, -1.0f, -0.6f));
  lit.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
  lit.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
  lit.SetVec3("spotLight.ambient", glm::vec3(0.2f));
  lit.SetVec3("spotLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
  lit.SetVec3("spotLight.specular", glm::vec3(1.0f));

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
    lit.SetVec3("light.direction", glm::vec3(0.0f, -1.0f, 0.0f));

    renderer_->Clear();
    renderer_->Draw(objects, &camera);

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