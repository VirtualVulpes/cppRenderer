//
// Created by sjfochs on 2/10/26.
//

#ifndef CPPRENDERER_INPUTMANAGER_H
#define CPPRENDERER_INPUTMANAGER_H
#include "InputState.h"
#include "Window.h"


class InputManager
{
public:
    InputManager();
    InputState getInput(const Window& window);
    InputState::Keys getKeyInput(const Window& window);
    InputState::Mouse getMouseInput(const Window& window);
private:
    InputState m_prevInput {};
};


#endif //CPPRENDERER_INPUTMANAGER_H