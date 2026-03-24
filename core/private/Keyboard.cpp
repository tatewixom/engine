#include "Keyboard.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <cstdint>

#include <iostream>

namespace Nuke
{
  static Keyboard::Keys glfw_key_to_nuke(int glfw_key)
  {
    switch (glfw_key)
    {
    case GLFW_KEY_A:
      return Keyboard::Keys::A;
    case GLFW_KEY_B:
      return Keyboard::Keys::B;
    case GLFW_KEY_C:
      return Keyboard::Keys::C;
    case GLFW_KEY_D:
      return Keyboard::Keys::D;
    case GLFW_KEY_E:
      return Keyboard::Keys::E;
    case GLFW_KEY_F:
      return Keyboard::Keys::F;
    case GLFW_KEY_G:
      return Keyboard::Keys::G;
    case GLFW_KEY_H:
      return Keyboard::Keys::H;
    case GLFW_KEY_I:
      return Keyboard::Keys::I;
    case GLFW_KEY_J:
      return Keyboard::Keys::J;
    case GLFW_KEY_K:
      return Keyboard::Keys::K;
    case GLFW_KEY_L:
      return Keyboard::Keys::L;
    case GLFW_KEY_M:
      return Keyboard::Keys::M;
    case GLFW_KEY_N:
      return Keyboard::Keys::N;
    case GLFW_KEY_O:
      return Keyboard::Keys::O;
    case GLFW_KEY_P:
      return Keyboard::Keys::P;
    case GLFW_KEY_Q:
      return Keyboard::Keys::Q;
    case GLFW_KEY_R:
      return Keyboard::Keys::R;
    case GLFW_KEY_S:
      return Keyboard::Keys::S;
    case GLFW_KEY_T:
      return Keyboard::Keys::T;
    case GLFW_KEY_U:
      return Keyboard::Keys::U;
    case GLFW_KEY_V:
      return Keyboard::Keys::V;
    case GLFW_KEY_W:
      return Keyboard::Keys::W;
    case GLFW_KEY_X:
      return Keyboard::Keys::X;
    case GLFW_KEY_Y:
      return Keyboard::Keys::Y;
    case GLFW_KEY_Z:
      return Keyboard::Keys::Z;
    case GLFW_KEY_0:
      return Keyboard::Keys::ZERO;
    case GLFW_KEY_1:
      return Keyboard::Keys::ONE;
    case GLFW_KEY_2:
      return Keyboard::Keys::TWO;
    case GLFW_KEY_3:
      return Keyboard::Keys::THREE;
    case GLFW_KEY_4:
      return Keyboard::Keys::FOUR;
    case GLFW_KEY_5:
      return Keyboard::Keys::FIVE;
    case GLFW_KEY_6:
      return Keyboard::Keys::SIX;
    case GLFW_KEY_7:
      return Keyboard::Keys::SEVEN;
    case GLFW_KEY_8:
      return Keyboard::Keys::EIGHT;
    case GLFW_KEY_9:
      return Keyboard::Keys::NINE;
    case GLFW_KEY_SPACE:
      return Keyboard::Keys::SPACE;
    case GLFW_KEY_LEFT_SHIFT:
      return Keyboard::Keys::LEFT_SHIFT;
    case GLFW_KEY_RIGHT_SHIFT:
      return Keyboard::Keys::RIGHT_SHIFT;
    case GLFW_KEY_UP:
      return Keyboard::Keys::UP;
    case GLFW_KEY_DOWN:
      return Keyboard::Keys::DOWN;
    case GLFW_KEY_LEFT:
      return Keyboard::Keys::LEFT;
    case GLFW_KEY_RIGHT:
      return Keyboard::Keys::RIGHT;
    case GLFW_KEY_ESCAPE:
      return Keyboard::Keys::ESCAPE;
    default:
      return Keyboard::Keys::none;
    }
  }

  static Keyboard::Action glfw_action_to_nuke(int glfw_action)
  {
    switch (glfw_action)
    {
    case GLFW_PRESS:
      return Keyboard::Action::PRESSED;
    case GLFW_RELEASE:
      return Keyboard::Action::RELEASED;
    case GLFW_REPEAT:
      return Keyboard::Action::REPEATING;
    default:
      return Keyboard::Action::none;
    }
  }

  /* DEPRECATED KEYBOARD FUNCTIONS
  bool isKeyPressed(GLFWwindow* window, int key)
  {
    return glfwGetKey(window, key) == GLFW_PRESS;
  }

  bool isKeyJustPressed(GLFWwindow* window, int key)
  {
    static std::unordered_map<int, bool>  keyStates;
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

  void processWindowEscape(GLFWwindow* window)
  {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
  }
  */

  Keyboard::Keyboard(Window& window)
    : window_{ window }
  {
  }

  void Keyboard::action(Key key)
  {
    keys_[key.key] = key.action;
  }

  bool Keyboard::is(Keys key)
  {
    return (keys_[key] == Action::PRESSED) || (keys_[key] == Action::REPEATING);
  }

  bool Keyboard::now(Keys key)
  {
    static Itemizer<Keys, Action> current_states{};
    const bool currently_pressed = is(key);

    // if key was released before and is now pressed
    if (currently_pressed && !((current_states[key] == Action::PRESSED) || (current_states[key] == Action::REPEATING)))
    {
      current_states[key] = Action::PRESSED;
      return true;
    }

    if (!currently_pressed)
      current_states[key] = Action::RELEASED;

    return false;
  }

  Keyboard::Key::Key(int code, int action)
    : key{ glfw_key_to_nuke(code) }
    , action{ glfw_action_to_nuke(action) }
  {
  }
}