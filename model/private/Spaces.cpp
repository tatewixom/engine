#include "Spaces.h"
#include "Window.h"
#include "Camera.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

Rotation::Rotation(const glm::vec3& axis, float angle)
  : m_axis{ axis }
  , m_angle{ angle }
{
  if (m_axis == glm::vec3{ 0.f, 0.f, 0.f })
    m_axis = glm::vec3{ 0.f, 1.f, 0.f };
}

void Spaces::update(Camera& camera, Window& window)
{
  Window::Dimensions dimensions{ window.dimensions() };
  
  projection = glm::perspective(
    glm::radians(camera.fov), 
    static_cast<float>(dimensions.height) / static_cast<float>(dimensions.width), 
    camera.nearPlane, camera.farPlane
  );

  view = camera.getViewMatrix();
}

