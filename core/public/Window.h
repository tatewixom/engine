#pragma once

#include <glad/glad.h>
#include <glfw3.h>

class Window
{
public:
  struct Dimensions
  {
    int width{};
    int height{};
  };

public:
  Window(const char* title, const char* path_to_icon, float widthRatio = 2.0f, float heightRatio = 6.0, GLFWmonitor* screenMode = nullptr, GLFWwindow* share = nullptr);

  ~Window() { glfwTerminate(); }

  //implicit conversion from Window to GLFWwindow* (carefully use)
  operator GLFWwindow* () const { return window_; }

  GLFWwindow* getWindow() { return window_; }
  Dimensions dimensions() const { return dimensions_; }
  void close() { glfwSetWindowShouldClose(window_, true); }
  void title(const char* title);
  void size(Dimensions dimensions); 

private:
  GLFWwindow* window_{};
  Dimensions dimensions_{};
};
