#include "Window.h"

#include <iostream>

#include "WindowContext.h"

Window::Window(int width, int height, std::string_view title)
{
  // glfw: initialize and configure
  // ------------------------------
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  // glfw window creation
  // --------------------
  window_ = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
  if (window_ == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return;
  }
  glfwMakeContextCurrent(window_);
  glfwSetFramebufferSizeCallback(window_, FramebufferSizeCallback);

  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Window::~Window() {
  glfwTerminate();
}

void Window::SetActive() const {
  glfwMakeContextCurrent(window_);
}

void Window::Close() const {
  glfwSetWindowShouldClose(window_, true);
}

float Window::GetAspectRatio() const {
  int width;
  int height;

  glfwGetFramebufferSize(window_, &width, &height);

  return static_cast<float>(width) / height;
}

void Window::FramebufferSizeCallback(GLFWwindow *window, const int width, const int height) {
  const auto* context = static_cast<WindowContext*>(glfwGetWindowUserPointer(window));
  if (!context) return;

  glViewport(0, 0, width, height);
  context->camera->SetAspectRatio(static_cast<float>(width) / height);
  context->camera->MarkProjectionDirty();
}

void Window::SetMouseCallback(GLFWcursorposfun callback) const {
  glfwSetCursorPosCallback(window_, callback);
}

void Window::SetScrollCallback(GLFWcursorposfun callback) const {
  glfwSetScrollCallback(window_, callback);
}

bool Window::ShouldClose() const {
  return glfwWindowShouldClose(window_);
}

void Window::PollEvents() {
  glfwPollEvents();
}

void Window::SwapBuffers() const {
  glfwSwapBuffers(window_);
}
