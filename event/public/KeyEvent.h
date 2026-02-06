#pragma once

#include <string>

namespace Events
{
  struct KeyPressed
  {
    KeyPressed(const int id, bool isRepeat = false);

    std::string string() const;

    int id{};
    bool isRepeat{};
  };

  struct KeyReleased
  {
    std::string string() const;

    int id{};
  };
}

