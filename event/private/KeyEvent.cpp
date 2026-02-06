#include "KeyEvent.h"

#include <iostream>
#include <string>

namespace Events
{
  KeyPressed::KeyPressed(const int id, bool isRepeat)
    : id{ id }
    , isRepeat{ isRepeat }
  { }

  std::string KeyPressed::string() const
  {
    return "KeyPressed: " + std::string{ std::to_string(id) };
  }

  std::string KeyReleased::string() const
  {
    return "KeyReleased: " + std::string{ std::to_string(id) };
  }
}
