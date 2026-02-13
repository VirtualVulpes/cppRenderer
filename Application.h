#ifndef CPPRENDERER_APPLICATION_H
#define CPPRENDERER_APPLICATION_H

#include "Renderer.h"
#include "Window.h"

class Application
{
public:
    Application();
    void Run();
    void Close();
private:
    Renderer renderer_{};
    Window window_;
    bool should_close_{false};
};


#endif