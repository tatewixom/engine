#pragma once

#include "Window.h"
#include "Camera.h"
#include "Mouse.h"
#include "State.h"
#include "Spaces.h"

class Engine
{
public:
  Engine(Window& window);

  void initialize();
  void run();

  Window& getWindow() noexcept { return window_; }
  Mouse& getMouse() noexcept { return mouse_; }
  Camera& getCamera() noexcept { return camera_; }
  Spaces& getSpaces() noexcept { return spaces_; }

private:
  Window& window_;
  Mouse mouse_{ window_ }; //mouse depends on window
  Camera camera_{ mouse_ }; //camera depends on mouse
  State state_{};
  Spaces spaces_{};
};

