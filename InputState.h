#ifndef CPPRENDERER_INPUTSTATE_H
#define CPPRENDERER_INPUTSTATE_H

struct InputState {
  struct Keys {
    bool forward{false};
    bool backward{false};
    bool right{false};
    bool left{false};
    bool up{false};
    bool down{false};
    bool escape{false};
  };
  Keys keys;

  struct Mouse {
    float x_pos{0};
    float x_last{0};
    float y_pos{0};
    float y_last{0};
  };
  Mouse mouse;
};

#endif
