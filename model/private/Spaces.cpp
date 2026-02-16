#include "Spaces.h"
#include "Window.h"
#include "Camera.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

namespace Nuke
{
  Rotation::Rotation(const glm::vec3 &axis, float angle)
      : axis_{axis}
      , angle_{angle}
  {
    if (axis_ == glm::vec3{ 0.f, 0.f, 0.f })
      axis_ = glm::vec3{ 0.f, 1.f, 0.f };
  }

  void Spaces::update(Camera &camera, Window &window)
  {
    Window::Dimensions dimensions{window.dimensions()};

    // aspect ratio = width / height
    float aspect{static_cast<float>(dimensions.width) / static_cast<float>(dimensions.height)};

    projection = glm::perspective(glm::radians(camera.fov), aspect, camera.nearPlane, camera.farPlane);

    view = camera.getViewMatrix();
  }
}
