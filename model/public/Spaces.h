#pragma once

#include <glad/glad.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

struct Dimension
{
  float x{};
  float y{};
  float z{};
  glm::vec3 scalar{ 1.f };
};

class Rotation
{
public:
  Rotation() = default;

  Rotation(const glm::vec3& axis, float angle);

  auto angle() const { return glm::radians(m_angle); }
  auto axis() const { return glm::normalize(m_axis); }

private:
  glm::vec3 m_axis{ 0.f, 1.f, 0.f };
  float m_angle{ 0.f };
};

class Camera;
class Window;

struct Spaces
{
public:
  Spaces() = default;
  void update(Camera& camera, Window& window);

  glm::mat4 model{ 1.0f };
  glm::mat4 projection{ 1.0f };
  glm::mat4 view{ 1.0f };
  glm::mat4 mvp{ 1.0f };
};

