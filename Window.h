#ifndef CPPRENDERER_WINDOW_H
#define CPPRENDERER_WINDOW_H

#include <string_view>
#include "glad/glad.h"

#include "GLFW/glfw3.h"

struct WindowCoordinates {
  int width{1280};
  int height{720};
  int x_pos{0};
  int y_pos{0};
};

class Window {
public:
  Window(WindowCoordinates coordinates, std::string_view title);

  void SetMouseCallback(GLFWcursorposfun callback) const;
  void SetScrollCallback(GLFWcursorposfun callback) const;

  void SetActive() const;
  bool ShouldClose() const;
  void Close() const;

  static void PollEvents();
  void SwapBuffers() const;

  float GetAspectRatio() const;

  GLFWwindow* GetHandle() const { return window_; }
  const WindowCoordinates& GetCoordinates() const {return coordinates_; }

  void ToggleFullscreen();
  void UpdateCoordinates();

private:
  GLFWwindow* window_{};
  WindowCoordinates coordinates_{};
  bool is_fullscreen_{false};
  static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
};

#endif
