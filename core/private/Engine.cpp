#include "Engine.h"
#include "Window.h"
#include "Camera.h"
#include "Mouse.h"
#include "State.h"
#include "Phase.h"
#include "Ground.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <string>
#include <iostream>

namespace Nuke
{
  Engine::Engine(Window &window)
      : window_{window}
  {
    // setting glfw pointer to engine for callbacks
    glfwSetWindowUserPointer(window_, this);

    // stbi init
    stbi_set_flip_vertically_on_load(true);

    // initially setting user to view mode
    // that is, using the mouse to control the camera
    mouse_.viewMode();

    glEnable(GL_DEPTH_TEST);
  }

  void Engine::initialize()
  {
    states_.push<Ground>(states_, *this);
    /* openGL context MUST be valid by this point */
  }

  void Engine::run()
  {
    initialize();

    // vars for fps count
    Phase::Timer frameTimer{};
    int fps = 0;

    // check if window should close
    while (!glfwWindowShouldClose(window_))
    {
      // go through all linked callback events
      glfwPollEvents();

      // process events
      mouse_.update();
      spaces_.update(camera_, window_);
      camera_.update();

      // looping one time through the state's loop function
      states_.loop();
      /*
        the state is responsible for initializing,
        making use of input classes, updating, and
        rendering itself to the screen.
      */

      // one total frame has occured
      ++fps;

      // checking when a second has elapsed
      constexpr double second{1.0};
      if (frameTimer.elapsed() >= second) // assuming elapsed returns time in seconds
      {
        // set window title to fps count
        window_.title(std::to_string(fps).data());

        // reset frames and timer
        fps = 0;
        frameTimer.reset();
      }
    }
  }
}