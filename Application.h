#ifndef CPPRENDERER_APPLICATION_H
#define CPPRENDERER_APPLICATION_H

#include <memory>

#include "Renderer.h"
#include "Window.h"

class Application {
public:
  Application();

  void Run();
  void Close();

private:
  std::unique_ptr<Window> window_;
  std::unique_ptr<Renderer> renderer_;
  bool should_close_{false};
};

#endif
