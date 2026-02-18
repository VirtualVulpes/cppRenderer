#include "InputManager.h"

#include "GLFW/glfw3.h"

#include "InputState.h"

InputManager::InputManager(const KeyBindings& key_bindings)
  : key_bindings_(key_bindings) {
}

InputState InputManager::GetInput(const Window& window) {
  input_.lastKeys = input_.keys;
  input_.lastMouse = input_.mouse;

  input_.keys = GetKeyInput(window);
  input_.mouse = GetMouseInput(window);

  return input_;
}

InputState::Keys InputManager::GetKeyInput(const Window& window) const {
  GLFWwindow *window_handle = window.GetHandle();

  InputState::Keys input{};

  input.forward  = IsActionPressed(Action::MoveForward, window);
  input.backward = IsActionPressed(Action::MoveBackward, window);
  input.left     = IsActionPressed(Action::MoveLeft, window);
  input.right    = IsActionPressed(Action::MoveRight, window);
  input.up       = IsActionPressed(Action::MoveUp, window);
  input.down     = IsActionPressed(Action::MoveDown, window);

  input.toggleLightDebug   = IsActionPressed(Action::ToggleLightDebug, window);
  input.toggleWireframe    = IsActionPressed(Action::ToggleWireframe, window);
  input.toggleDrawTextures = IsActionPressed(Action::ToggleDrawTextures, window);
  input.toggleFullscreen   = IsActionPressed(Action::ToggleFullscreen, window);

  input.quit = IsActionPressed(Action::Quit, window);
  return input;
}

InputState::Mouse InputManager::GetMouseInput(const Window& window) {
  GLFWwindow *window_handle = window.GetHandle();

  double x_pos{};
  double y_pos{};
  glfwGetCursorPos(window_handle, &x_pos, &y_pos);

  InputState::Mouse input{};

  input.x_pos = x_pos;
  input.y_pos = y_pos;

  return input;
}

bool InputManager::IsActionPressed(Action action, const Window& window) const {
  auto it = key_bindings_.keyboard.find(action);
  if (it == key_bindings_.keyboard.end())
      return false;

  return glfwGetKey(window.GetHandle(), it->second) == GLFW_PRESS;
}