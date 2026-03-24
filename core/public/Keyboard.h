#pragma once

#include "Containers.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Nuke
{
  class Window;

  class Keyboard
  {
  public:
    enum class Keys : int
    {
      none,
      A,
      B,
      C,
      D,
      E,
      F,
      G,
      H,
      I,
      J,
      K,
      L,
      M,
      N,
      O,
      P,
      Q,
      R,
      S,
      T,
      U,
      V,
      W,
      X,
      Y,
      Z,
      SPACE,
      LEFT_SHIFT,
      RIGHT_SHIFT,
      UP,
      DOWN,
      LEFT,
      RIGHT,
      ZERO,
      ONE,
      TWO,
      THREE,
      FOUR,
      FIVE,
      SIX,
      SEVEN,
      EIGHT,
      NINE,
      ESCAPE,
      max,
    };

    enum class Action : int
    {
      none,
      PRESSED,
      RELEASED,
      REPEATING,
      max,
    };

    struct Key
    {
      Keys key{};
      Action action{};

      Key() = default;
      Key(int code, int action);
    };

  public:
    Keyboard(Window& window);

    void action(Key key);
    bool is(Keys key);
    bool now(Keys key);
  private:
    Window& window_;
    Itemizer<Keys, Action> keys_{};
  };
}