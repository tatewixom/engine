#include "Callback.h"
#include "Engine.h"
#include "Mouse.h"
#include "Window.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Nuke
{
  namespace Callback
  {
    /*
      IDEOLOGY:

      these are all callback functions defined but maintained by GLFW.
      they are called when glfwPollEvents() has been called. they must be
      linked using GLFW callback function-linkers.

      the main approach I'm using is to limit these functions to simply
      assign variable to input classes. It makes it more clear as to
      what behavior is happening by just looking at the behavior
      associated with the object of the class.

      for example, the camera starts jittering out of nowhere and you
      are investgating. Would you naturally look into the Mouse class,
      or go to this callback file and look for the mouse callback
      associated with positioning. The former is obviously more
      favorable.

      it also makes behavior more predictable as you can use an update
      function in your loop as to see the exact point you used the
      information assigned from the mouse callback function to the
      Mouse object.
    */

    void framebuffer(GLFWwindow *window, int width, int height)
    {
      Engine *engine{static_cast<Engine *>(glfwGetWindowUserPointer(window))};
      engine->getWindow().size(Window::Dimensions{width, height});
    }

    void mouse(GLFWwindow *window, double xposIn, double yposIn)
    {
      Engine *engine{static_cast<Engine *>(glfwGetWindowUserPointer(window))};
      engine->getMouse().position(Mouse::Position{xposIn, yposIn});
    }

    void scroll(GLFWwindow *window, double xoffset, double yoffset)
    {
      Engine *engine{static_cast<Engine *>(glfwGetWindowUserPointer(window))};
      engine->getMouse().soffset(Mouse::Position{xoffset, yoffset});
    }

    void refresh(GLFWwindow *window)
    {
      // nothing
    }

    void mouseButton(GLFWwindow *window, int button, int action, int mods)
    {
      Engine *engine{static_cast<Engine *>(glfwGetWindowUserPointer(window))};
      engine->getMouse().button(Mouse::Button{button, action});
    }
  }
}