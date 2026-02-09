//
// Created by sjfochs on 2/9/26.
//

#ifndef CPPRENDERER_RENDERER_H
#define CPPRENDERER_RENDERER_H
#include "GLFW/glfw3.h"


class Renderer
{
public:
    void createWindow(unsigned int width, unsigned int height);
    unsigned int getWidth() { return m_scr_width; }
    unsigned int getHeight() { return m_scr_height; }
    GLFWwindow* getWindow();
private:
    unsigned int m_scr_width;
    unsigned int m_scr_height;
    GLFWwindow* m_window;

    static void framebufferSizeCallback(GLFWwindow* window, const int width, const int height);
};


#endif //CPPRENDERER_RENDERER_H