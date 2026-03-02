#include "Mouse.h"
#include "Window.h"
#include "Callback.h"

namespace Nuke
{
  /*
    HOT FIX:
    On Wayland (which is what Asahi uses), if you mix polling with callbacks,
    it'll starve some of the inputs. Which is why the camera/mouse (at the time of writing)
    won't move unless you stop pressing a key.

    Keyboard namespace uses glfwGetKey(), while the mouse position has a callback.

    SOLUTION:
    I need to pick a model, either only use polling, or only use callbacks. callbacks
    seem more attractive at the time of writing due to predictable behavior.

    It's easier to see exactly when an input was set via glfwPollEvents().
  */

  Mouse::Mouse(Window& window)
    : window_{ window }
  {
    glfwSetCursorPosCallback(window_, Callback::mouse);
    glfwSetScrollCallback(window_, Callback::scroll);
    glfwSetMouseButtonCallback(window_, Callback::mouseButton);
  }

  void Mouse::update()
  {
    static bool firstMouse{ true };

    if (firstMouse)
    {
      center();
      Window::Dimensions window{ window_.dimensions() };
      position_ = Mouse::Position{ window.width / 2.0f, window.height / 2.0f }; // preventing mouse "snapping" into position
      lastPosition_ = position_;
      firstMouse = false;
    }

    offset_.x = position_.x - lastPosition_.x; // calculating offset
    offset_.y = lastPosition_.y - position_.y;

    lastPosition_ = position_; // setting last position
  }

  void Mouse::center()
  {
    Window::Dimensions window{ window_.dimensions() };
    glfwSetCursorPos(window_, static_cast<double>(window.width) / 2.0, static_cast<double>(window.height) / 2.0);
  }

  void Mouse::viewMode()
  {
    position_ = lastPosition_;

    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  void Mouse::selectionMode()
  {
    lastPosition_ = position_;

    center();
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }

  bool Mouse::isButtonPressed(int mouseButton) const
  {
    if (button_.button == mouseButton && button_.action == GLFW_PRESS)
      return true;

    return false;
  }

  bool Mouse::isDisabled() const
  {
    return glfwGetInputMode(window_, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
  }

  void Mouse::setLastPosition()
  {
    position_ = lastPosition_;
  }

  bool operator==(const Mouse::Position& pos1, const Mouse::Position& pos2)
  {
    return (pos1.x == pos2.x) && (pos1.y == pos2.y);
  }

  Mouse::Position operator-(const Mouse::Position& pos1, const Mouse::Position& pos2)
  {
    return Mouse::Position{ pos1.x - pos2.x, pos1.y - pos2.y };
  }
}