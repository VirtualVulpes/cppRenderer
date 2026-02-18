#ifndef CPPRENDERER_INPUTSTATE_H
#define CPPRENDERER_INPUTSTATE_H

struct InputState {
  struct Keys {
    // Movement
    bool forward{false};
    bool backward{false};
    bool right{false};
    bool left{false};
    bool up{false};
    bool down{false};

    //Settings
    bool toggleWireframe{false};
    bool toggleLightDebug{false};
    bool toogleDrawTextures{false};

    bool quit{false};
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
