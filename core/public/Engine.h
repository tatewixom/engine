#pragma once

#include "Window.h"
#include "Camera.h"
#include "Mouse.h"
#include "State.h"
#include "Spaces.h"
#include "Keyboard.h"

namespace Nuke
{
  class Engine
  {
  public:
    Engine(Window& window);

    void initialize();
    void run();

    Window& getWindow() noexcept { return window_; }
    Keyboard& getKeyboard() noexcept { return keyboard_; }
    Mouse& getMouse() noexcept { return mouse_; }
    Camera& getCamera() noexcept { return camera_; }
    States& getStates() noexcept { return states_; }
    Spaces& getSpaces() noexcept { return spaces_; }

  private:
    Window& window_;
    Keyboard keyboard_{ window_ }; // keyboard depends on window
    Mouse mouse_{ window_ };  // mouse depends on window
    Camera camera_{ mouse_ }; // camera depends on mouse
    States states_{};
    Spaces spaces_{};
  };
}
