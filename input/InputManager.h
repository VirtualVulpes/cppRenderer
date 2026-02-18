#ifndef CPPRENDERER_INPUTMANAGER_H
#define CPPRENDERER_INPUTMANAGER_H

#include "InputState.h"
#include "KeyBindings.h"
#include "../Window.h"

class InputManager {
public:
  InputManager(const KeyBindings& key_bindings);

  InputState GetInput(const Window& window);
  InputState::Keys GetKeyInput(const Window& window) const;
  InputState::Mouse GetMouseInput(const Window& window);

  bool IsActionPressed(Action action, const Window& window) const;
  bool IsJustPressed(bool InputState::Keys::*key) { return input_.keys.*key && !(input_.lastKeys.*key); };
  bool IsJustReleased(bool InputState::Keys::*key) { return !(input_.keys.*key) && input_.lastKeys.*key; };

private:
  InputState input_{};
  const KeyBindings& key_bindings_;
};

#endif
