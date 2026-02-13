#include <iostream>
#include <ostream>

#include "Application.h"

int main()
{
  // glad: load all OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  Application application{};
  application.Run();

  return 0;
}
