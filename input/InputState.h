#ifndef CPPRENDERER_INPUTSTATE_H
#define CPPRENDERER_INPUTSTATE_H

struct InputState {
  struct Keys {
    // Movement
    bool forward;
    bool backward;
    bool right;
    bool left;
    bool up;
    bool down;

    //Settings
    bool toggleWireframe;
    bool toggleLightDebug;
    bool toggleDrawTextures;
    bool toggleFullscreen;

    bool quit;
  };
  Keys keys;
  Keys lastKeys;

  struct Mouse {
    float x_pos{0};
    float y_pos{0};
  };
  Mouse mouse;
  Mouse lastMouse;
};

#endif
