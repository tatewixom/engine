#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Callback
{
  void framebuffer(GLFWwindow* window, int width, int height);
  void mouse(GLFWwindow* window, double xposIn, double yposIn);
  void scroll(GLFWwindow* window, double xoffset, double yoffset);
  void refresh(GLFWwindow* window);
  void mouseButton(GLFWwindow* window, int button, int action, int mods);
}
