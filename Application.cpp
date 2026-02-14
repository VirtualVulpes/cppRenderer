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
  InputManager input_manager{};

  Transform t{glm::vec3(8.0f, 0.0f, 8.0f)};

  Mesh cubeMesh{Geometry::Cube{}};
  Mesh planeMesh{Geometry::Plane{}};

  Texture dirtTex{"textures/dirt.png"};
  Texture grassTex{"textures/grass_block_top.png"};

  Shader shader{"shaders/shader.vs", "shaders/shader.fs"};
  renderer_->UseShader(&shader);

  WindowContext context{&camera};
  glfwSetWindowUserPointer(window_->GetHandle(), &context);

  std::vector<std::unique_ptr<GameObject>> objects;
  objects.push_back(std::make_unique<GameObject>(t, &cubeMesh, &dirtTex, &shader));
  CreateFloorMesh(objects, &planeMesh, &grassTex, &shader);

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

    renderer_->SetCamera(camera);

    renderer_->Clear();
    renderer_->Draw(objects);

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