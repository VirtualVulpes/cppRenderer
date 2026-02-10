//
// Created by sjfochs on 2/10/26.
//

#include "InputManager.h"

#include "InputState.h"
#include "GLFW/glfw3.h"

InputManager::InputManager()
{
}

InputState InputManager::getInput(const Window& window)
{
    InputState input {};
    input.keys = getKeyInput(window);
    input.mouse = getMouseInput(window);

    return input;
}

InputState::Keys InputManager::getKeyInput(const Window& window)
{
    GLFWwindow* windowHandle = window.nativeHandle();

    InputState::Keys input {};
    input.forward  = glfwGetKey(windowHandle, GLFW_KEY_W) == GLFW_PRESS;
    input.backward = glfwGetKey(windowHandle, GLFW_KEY_S) == GLFW_PRESS;
    input.left     = glfwGetKey(windowHandle, GLFW_KEY_A) == GLFW_PRESS;
    input.right    = glfwGetKey(windowHandle, GLFW_KEY_D) == GLFW_PRESS;
    input.escape   = glfwGetKey(windowHandle, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    return input;
}

InputState::Mouse InputManager::getMouseInput(const Window& window)
{
    GLFWwindow* windowHandle = window.nativeHandle();

    double xPos {};
    double yPos {};
    glfwGetCursorPos(windowHandle, &xPos, &yPos);

    InputState::Mouse input {};

    input.xLast = m_prevInput.mouse.xPos;
    input.yLast = m_prevInput.mouse.yPos;
    input.xPos = xPos;
    input.yPos = yPos;
    m_prevInput.mouse.xPos = xPos;
    m_prevInput.mouse.yPos = yPos;

    return input;
}