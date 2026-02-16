#pragma once

#include "State.h"
#include "Engine.h"

class Window;
class Camera;
class Mouse;

namespace Nuke
{
  class Ground final : public State
  {
  public:
    Ground(States &states, Engine &engine);

    ~Ground() override;

    void initialize();
    void input();
    void clear();
    void update();
    void render();
    void clean();

    void loop() override;

  private:
    Engine &engine_;
  };
}
