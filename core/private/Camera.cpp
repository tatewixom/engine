#include "Camera.h"
#include "Mouse.h"

#include <cmath>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

Camera::Camera(Mouse & mouse)
  : mouse_{ mouse }
{ }

Camera::Camera(Mouse& mouse, float vfov, glm::vec3 vposition, float vspeed = 5.0f, float vsensitivity = 0.1f)
  : position{ vposition }
  , fov{ vfov }
  , speed{ vspeed }
  , sensitivity{ vsensitivity }
  , mouse_{ mouse }
{ }

void Camera::move(Movement movement, float deltaTime)
{
  float velocity = speed * deltaTime;

  switch (movement)
  {
  case FORWARD:
    position += front * velocity;
    break;
  case BACKWARD:
    position -= front * velocity;
    break;
  case LEFT:
    position -= right * velocity;
    break;
  case RIGHT:
    position += right * velocity;
    break;
  case UP:
    position += up * velocity;
    break;
  case DOWN:
    position -= up * velocity;
    break;
  default:
    break;
  }
}

void Camera::update()
{
  Mouse::Position cameraMovement{ mouse_.poffset() };

  if (mouse_.isDisabled())
  {
    yaw += static_cast<float>(cameraMovement.x * sensitivity);
    pitch += static_cast<float>(cameraMovement.y * sensitivity);

    //constexpr static min and max values for pitch
    static constexpr float min_pitch{ -89.f };
    static constexpr float max_pitch{ 89.f };

    //constraining the amount you can look up and look down
    pitch = glm::clamp(pitch, min_pitch, max_pitch);

    //calculating camera direction vector
    glm::vec3 frontDirection{};
    frontDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    frontDirection.y = sin(glm::radians(pitch));
    frontDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(frontDirection);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(cross(right, front));

    //constexpr static min and max values for fov
    static constexpr float min_fov{ 30.f };
    static constexpr float max_fov{ 120.f };

    //constraining camera fov based off mouse scroll wheel
    fov -= static_cast<float>(mouse_.soffset().y);
    
    //keep fov within 30 and 120
    fov = glm::clamp(fov, min_fov, max_fov);
  }
}

glm::mat4 Camera::getViewMatrix() const
{
  return glm::lookAt(position, position + front, up);
}
