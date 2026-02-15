#pragma once

#include <glm.hpp>

class Mouse;

class Camera
{
public:
  enum Movement
  {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
  };

  Camera(Mouse& mouse, float vfov, glm::vec3 vposition, float vspeed, float vsensitivity);
  Camera(Mouse& mouse);

  void move(Movement direction, float deltaTime);
  void update();

  //position vectors
  glm::vec3 position{ 0.f, 0.f, 3.f };
  glm::vec3 front{ 0.0f, 0.0f, -1.0f };
  glm::vec3 up{ 0.0f, 1.0f, 0.0f };
  glm::vec3 direction{ glm::normalize(position - glm::vec3{ 0.0f, 0.0f, 0.0f }) };
  glm::vec3 right{ glm::normalize(glm::cross(up, direction)) };

  static constexpr glm::vec3 worldUp{ 0.0f, 1.0f, 0.0f };

  //frustums
  float nearPlane{ 0.01f };
  float farPlane{ 1000.0f };

  //camera rotation
  float yaw{ -90.0f };	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a
  float pitch{ 0.0f };  // direction vector pointing to the right so we initially rotate a bit to the left.

  //misc camera attributes
  float fov{ 120.f };
  float speed{ 5.f };
  float sensitivity{ 0.1f };
  float zoom{};

  glm::mat4 getViewMatrix() const;

private:
  Mouse& mouse_;
};

