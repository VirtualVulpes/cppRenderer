#ifndef CPPRENDERER_INPUTMANAGER_H
#define CPPRENDERER_INPUTMANAGER_H

#include "InputState.h"
#include "Window.h"

class InputManager {
public:
  InputState GetInput(const Window &window);
  InputState::Keys GetKeyInput(const Window &window);
  InputState::Mouse GetMouseInput(const Window &window);

private:
  InputState::Mouse prev_mouse_{};
};

#endif
