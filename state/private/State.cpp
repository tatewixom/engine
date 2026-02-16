#include "State.h"

namespace Nuke
{
  void State::loop()
  {
    if (!states_.empty())
      states_.back()->loop();
  }

  void State::pop()
  {
    if (!states_.empty() && states_.size() > 1)
      states_.pop_back();
  }

  void State::loopBelow()
  {
    if (!states_.empty() && states_.size() > 1)
      states_[states_.size() - 2]->loop();
  }

}
