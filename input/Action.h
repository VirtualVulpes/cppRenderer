#ifndef CPPRENDERER_ACTION_H
#define CPPRENDERER_ACTION_H

enum class Action {
  // Movement
  MoveForward,
  MoveBackward,
  MoveLeft,
  MoveRight,
  MoveUp,
  MoveDown,

  // Settings
  ToggleWireframe,
  ToggleLightDebug,
  ToggleDrawTextures,
  ToggleFullscreen,

  Quit
};

#endif