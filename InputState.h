//
// Created by sjfoc on 2/8/2026.
//

#ifndef UNTITLED_INPUTSTATE_H
#define UNTITLED_INPUTSTATE_H
class InputState
{
public:
    struct Keys
    {
        bool forward  { false };
        bool backward { false };
        bool right    { false };
        bool left     { false };
        bool escape   { false };
    };
    Keys keys;

    struct Mouse
    {
        float xPos {0};
        float xLast {0};
        float yPos {0};
        float yLast {0};
    };
    Mouse mouse;
};

#endif //UNTITLED_INPUTSTATE_H