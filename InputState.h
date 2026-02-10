//
// Created by sjfoc on 2/8/2026.
//

#ifndef UNTITLED_INPUTSTATE_H
#define UNTITLED_INPUTSTATE_H
struct InputState
{
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
        float xOffset {0};
        float yPos {0};
        float yOffset {0};
    };
    Mouse mouse;
};

#endif //UNTITLED_INPUTSTATE_H