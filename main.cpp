#include "Application.h"

constexpr int kInitialWidth{1280};
constexpr int kInitialHeight{720};

int main()
{
  if (!glfwInit())
    throw std::runtime_error("Failed to initialize GLFW");

  constexpr WindowCoordinates coordinates{kInitialWidth, kInitialHeight};
  Window window{coordinates, "Render Window"};

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    throw std::runtime_error("Failed to initialize GLAD");

  Application app{window};
  app.Run();

  glfwTerminate();
  return 0;
}
