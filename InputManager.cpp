//
// Created by sjfochs on 2/10/26.
//

#include "InputManager.h"

#include "InputState.h"
#include "GLFW/glfw3.h"

InputState InputManager::GetInput(const Window &window) {
  InputState input{};
  input.keys = GetKeyInput(window);
  input.mouse = GetMouseInput(window);

  return input;
}

InputState::Keys InputManager::GetKeyInput(const Window &window) {
  GLFWwindow *window_handle = window.GetNativeHandle();

  InputState::Keys input{};
  input.forward  = glfwGetKey(window_handle, GLFW_KEY_W) == GLFW_PRESS;
  input.backward = glfwGetKey(window_handle, GLFW_KEY_S) == GLFW_PRESS;
  input.left     = glfwGetKey(window_handle, GLFW_KEY_A) == GLFW_PRESS;
  input.right    = glfwGetKey(window_handle, GLFW_KEY_D) == GLFW_PRESS;
  input.escape   = glfwGetKey(window_handle, GLFW_KEY_ESCAPE) == GLFW_PRESS;
  return input;
}

InputState::Mouse InputManager::GetMouseInput(const Window &window) {
  GLFWwindow *window_handle = window.GetNativeHandle();

  double x_pos{};
  double y_pos{};
  glfwGetCursorPos(window_handle, &x_pos, &y_pos);

  InputState::Mouse input{};

  input.x_last = prev_mouse_.x_pos;
  input.y_last = prev_mouse_.y_pos;
  input.x_pos  = x_pos;
  input.y_pos  = y_pos;

  prev_mouse_.x_pos = x_pos;
  prev_mouse_.y_pos = y_pos;

  return input;
}
