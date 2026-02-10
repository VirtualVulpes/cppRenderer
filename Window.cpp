//
// Created by sjfochs on 2/9/26.
//

#include "Window.h"
#include <iostream>

Window::Window(int width, int height, std::string_view title)
    : m_width(width)
    , m_height(height)
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    m_window = glfwCreateWindow(m_width, m_height, title.data(), nullptr, nullptr);
    if (m_window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Window::~Window()
{
    glfwTerminate();
}

void Window::setActive()
{
    glfwMakeContextCurrent(m_window);
}

void Window::close()
{
    glfwSetWindowShouldClose(m_window, true);
}

void Window::framebufferSizeCallback(GLFWwindow* window, const int width, const int height)
{
    glViewport(0, 0, width, height);
}

void Window::setMouseCallback(GLFWcursorposfun callback)
{
    glfwSetCursorPosCallback(m_window, callback);
}

void Window::setScrollCallback(GLFWcursorposfun callback)
{
    glfwSetScrollCallback(m_window, callback);
}

bool Window::shouldClose()
{
    return glfwWindowShouldClose(m_window);
}
