//
// Created by sjfochs on 2/9/26.
//

#ifndef CPPRENDERER_RENDERER_H
#define CPPRENDERER_RENDERER_H
#include <string_view>

#include "glad/glad.h"
#include "GLFW/glfw3.h"


class Window {
public:
  Window(int width, int height, std::string_view title);
  ~Window();

  void SetMouseCallback(GLFWcursorposfun callback);
  void SetScrollCallback(GLFWcursorposfun callback);

  void SetActive();
  bool ShouldClose();
  void Close();

  void PollEvents();
  void SwapBuffers();

  float GetAspectRatio() const;

  GLFWwindow *GetNativeHandle() const { return window_; }

private:
  GLFWwindow *window_;

  static void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
};

#endif //CPPRENDERER_RENDERER_H
