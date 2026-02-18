#ifndef CPPRENDERER_KEYBINDINGS_H
#define CPPRENDERER_KEYBINDINGS_H
#include <unordered_map>

#include "Action.h"

using Key = int;

struct KeyBindings {
  std::unordered_map<Action, Key> keyboard;
};

#endif