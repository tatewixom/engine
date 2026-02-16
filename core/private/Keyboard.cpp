#include "Keyboard.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>

namespace Nuke
{
  namespace Keyboard
  {
    bool isKeyPressed(GLFWwindow *window, int key)
    {
      return glfwGetKey(window, key) == GLFW_PRESS;
    }

    bool isKeyJustPressed(GLFWwindow *window, int key)
    {
      // stores previous key state
      static std::unordered_map<int, bool> keyStates;
      bool currentlyPressed = isKeyPressed(window, key);

      // if key was released before and is now pressed
      if (currentlyPressed && !keyStates[key])
      {
        keyStates[key] = true;
        return true;
      }

      if (!currentlyPressed)
      {
        keyStates[key] = false;
      }

      return false;
    }

    void processWindowEscape(GLFWwindow *window)
    {
      if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    }
  }
}