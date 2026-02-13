#pragma once

#include "State.h"
#include "Ditto.h"
#include "Collection.h"
#include "Interface.h"
#include "Object.h"
#include "Shader.h"
#include "Buffer.h"

class Window;
class Camera;
class Mouse;

class World final : public IState
{
public:
  World(State& state, Window& window, Camera& camera, Mouse& mouse);

  ~World() override
  {
    clean();
  }

  void initialize();
  void input();
  void clear();
  void update();
  void render();
  void loop() override;
  void clean();

private:
  Window& m_window;
  Camera& m_camera;
  Mouse& m_mouse;

  std::unique_ptr<Interface> m_interface{ std::make_unique<Interface>(state_, m_window, m_camera, m_mouse) };
};
