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

void Window::SetActive() {
  glfwMakeContextCurrent(window_);
}

void Window::Close() {
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

  context->shader->Use();
  context->shader->SetMat4("projection", context->camera->GetProjection(static_cast<float>(width) / height));

  glViewport(0, 0, width, height);
}

void Window::SetMouseCallback(GLFWcursorposfun callback) {
  glfwSetCursorPosCallback(window_, callback);
}

void Window::SetScrollCallback(GLFWcursorposfun callback) {
  glfwSetScrollCallback(window_, callback);
}

bool Window::ShouldClose() {
  return glfwWindowShouldClose(window_);
}

void Window::PollEvents() {
  glfwPollEvents();
}

void Window::SwapBuffers() {
  glfwSwapBuffers(window_);
}
