#pragma once

#include <glad/glad.h>
#include <glfw3.h>

namespace Keyboard
{
  //checks if the key is currently being pressed and will allow for repeat of input
  bool isKeyPressed(GLFWwindow* window, int key);

  //checks if the key was just released, doesn't allow for repeat input
  bool isKeyJustPressed(GLFWwindow* window, int key);

  //checks if the user has pressed the escape key to close window
  void processWindowEscape(GLFWwindow* window);
}