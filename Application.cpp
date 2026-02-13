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

Application::Application() {
  if (!glfwInit())
    throw std::runtime_error("Failed to initialize GLFW");

  window_ = std::make_unique<Window>(1280, 720, "Render Window");

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    throw std::runtime_error("Failed to initialize GLAD");

  renderer_ = std::make_unique<Renderer>();

  stbi_set_flip_vertically_on_load(true);
}

void Application::Run() {
  Camera camera{glm::vec3(0.0f, 0.0f, 5.0f), 0.0f, 180.0f};
  InputManager input_manager{};

  Transform t{};

  Mesh cubeMesh{Geometry::Cube::vertices, Geometry::Cube::indices};
  Mesh planeMesh{Geometry::Plane::vertices, Geometry::Plane::indices};

  Texture dirtTex{};
  dirtTex.Bind();
  dirtTex.LoadFromFile("textures/dirt.png");

  Texture stoneTex{};
  stoneTex.Bind();
  stoneTex.LoadFromFile("textures/stone.png");

  Shader shader("shaders/shader.vs", "shaders/shader.fs");
  shader.Use();
  shader.SetMat4("projection", camera.GetProjection(window_->GetAspectRatio()));

  WindowContext context{&camera, &shader};
  glfwSetWindowUserPointer(window_->GetHandle(), &context);

  GameObject cube{t, &cubeMesh, &dirtTex, &shader};
  t.pos = glm::vec3(0.0f, 2.0f, 0.0f);
  GameObject plane{t, &planeMesh, &stoneTex, &shader};

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
    shader.SetMat4("view", camera.GetView());

    renderer_->Clear();

    cube.Draw();
    plane.Draw();

    window_->SwapBuffers();
  }
}

void Application::Close() {
  should_close_ = true;
}
