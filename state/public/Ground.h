#pragma once

#include "State.h"
#include "Engine.h"

class Window;
class Camera;
class Mouse;

class Ground final : public IState
{
public:
  Ground(State& state, Engine& engine);

  ~Ground() override;

  void initialize();
  void input();
  void clear();
  void update();
  void render();
  void clean();

  void loop() override;

private:
  Engine& engine_;
};
