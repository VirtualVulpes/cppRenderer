#include "Application.h"

int main()
{
  if (!glfwInit())
    throw std::runtime_error("Failed to initialize GLFW");

  Application app{};
  app.Run();

  glfwTerminate();
  return 0;
}
