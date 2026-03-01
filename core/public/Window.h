#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Nuke
{
  class Window
  {
  public:
    struct Dimensions
    {
      int width{};
      int height{};

      friend bool operator==(const Dimensions& d1, const Dimensions& d2)
      {
        return (d1.width == d2.width) && (d1.height == d2.height);
      }
    };

  public:
    Window(const char* title, const char* path_to_icon, float widthRatio = 2.0f, float heightRatio = 3.0);

    ~Window() { glfwTerminate(); }

    // implicit conversion from Window to GLFWwindow* (carefully use)
    operator GLFWwindow* () const noexcept { return window_; }

    GLFWwindow* getWindow() const noexcept { return window_; }
    Dimensions dimensions() const noexcept { return dimensions_; }
    void close() const { glfwSetWindowShouldClose(window_, true); }
    void title(const char* title) const;
    void size(Dimensions dimensions) noexcept;

    void vsync(bool val) const;

    void refresh() const noexcept { glViewport(0, 0, dimensions_.width, dimensions_.width); }
  private:
    GLFWwindow* window_{};
    Dimensions dimensions_{};
  };
}
