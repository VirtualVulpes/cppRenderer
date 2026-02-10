//
// Created by sjfochs on 2/9/26.
//

#ifndef CPPRENDERER_RENDERER_H
#define CPPRENDERER_RENDERER_H
#include <string_view>

#include "glad/glad.h"
#include "GLFW/glfw3.h"


class Window
{
public:
    Window(int width, int height, std::string_view title);

    void setMouseCallback(GLFWcursorposfun callback);
    void setScrollCallback(GLFWcursorposfun callback);
    bool shouldClose();

    int getWidth() const  { return m_width;  }
    int getHeight() const { return m_height; }

    GLFWwindow* nativeHandle() const { return m_window; }

private:
    int m_width;
    int m_height;
    GLFWwindow*  m_window;

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};

#endif //CPPRENDERER_RENDERER_H