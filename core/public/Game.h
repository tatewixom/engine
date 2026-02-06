#pragma once

#include "Window.h"
#include "Camera.h"
#include "Mouse.h"
#include "Event.h"
#include "State.h"

class Game
{
public:
  static Game& getInstance()
  {
    static Game game{};
    return game;
  }

  void initialize();
  void run();

  Window& getWindow() noexcept { return window_; }
  Mouse& getMouse() noexcept { return mouse_; }
  Camera& getCamera() noexcept { return camera_; }

private:
  Game();

  Window window_{ "LearnOpenGL", "assets/icon/container.jpg" };
  Mouse mouse_{ window_ }; //mouse depends on window
  Camera camera_{ mouse_ }; //camera depends on mouse
  State state_{};
  Events::Bus eventBus_{};
};

