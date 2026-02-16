#include "State.h"

namespace Nuke
{
  void States::loop()
  {
    if (!states_.empty())
      states_.back()->loop();
  }

  void States::pop()
  {
    if (!states_.empty() && states_.size() > 1)
      states_.pop_back();
  }

  void States::loopBelow()
  {
    if (!states_.empty() && states_.size() > 1)
      states_[states_.size() - 2]->loop();
  }

}
