#include "Ditto.h"
#include "Random.h"
#include "Camera.h"
#include "Window.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

namespace Nuke
{
  Ditto::Spaces Ditto::s_spaces{};

  Ditto::Rotation::Rotation(const glm::vec3 &axis, float angle)
      : axis_{ axis }
      , angle_{angle}
  {
    if (axis_ == glm::vec3{ 0.f, 0.f, 0.f })
      axis_ = glm::vec3{ 0.f, 1.f, 0.f };
  }

  void Ditto::sendInstances(std::vector<glm::mat4> matrices, const std::vector<Motions> &motions, unsigned int location)
  {
    assert(matrices.size() == motions.size());

    instanceCount_ = matrices.size();

    for (std::size_t i{0}; i < instanceCount_; ++i)
    {
      matrices[i] = glm::translate(glm::mat4{1.0f}, motions[i].position);
      matrices[i] = glm::rotate(matrices[i], motions[i].rotation.angle(), motions[i].rotation.axis());
      matrices[i] = glm::scale(matrices[i], motions[i].scale);
    }

    layout_.bind();
    instanceBuffer_.gen();
    instanceBuffer_.bind();
    instanceBuffer_.send(matrices);

    layout_.instance(location);
  }

  void Ditto::draw()
  {
    s_spaces.mvp = s_spaces.projection * s_spaces.view;

    shader_.activate();
    shader_.set("mvp", s_spaces.mvp);

    layout_.bind();
    glDrawElementsInstanced(GL_TRIANGLES, element_.indiceCount, GL_UNSIGNED_INT, 0, instanceCount_);
  }

  void Ditto::update(const Camera &camera, const Window &window)
  {
    const Window::Dimensions dimensions{window.dimensions()};

    s_spaces.projection = glm::perspective(
        glm::radians(camera.fov),
        static_cast<float>(dimensions.height / dimensions.width),
        camera.nearPlane, camera.farPlane);

    s_spaces.view = camera.getViewMatrix();
  }
}