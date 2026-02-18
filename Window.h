#ifndef CPPRENDERER_WINDOW_H
#define CPPRENDERER_WINDOW_H

#include <string_view>
#include "glad/glad.h"

#include "GLFW/glfw3.h"

class Window {
public:
  Window(int width, int height, std::string_view title);

  void SetMouseCallback(GLFWcursorposfun callback) const;
  void SetScrollCallback(GLFWcursorposfun callback) const;

  void SetActive() const;
  bool ShouldClose() const;
  void Close() const;

  static void PollEvents();
  void SwapBuffers() const;

  float GetAspectRatio() const;

  GLFWwindow* GetHandle() const { return window_; }

private:
  GLFWwindow* window_{};

  static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
};

#endif
