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
      : m_axis{axis}, m_angle{angle}
  {
    if (m_axis == glm::vec3{0.f, 0.f, 0.f})
      m_axis = glm::vec3{0.f, 1.f, 0.f};
  }

  void Ditto::sendInstances(std::vector<glm::mat4> matrices, const std::vector<Motions> &motions, unsigned int location)
  {
    assert(matrices.size() == motions.size());

    m_instanceCount = matrices.size();

    for (std::size_t i{0}; i < m_instanceCount; ++i)
    {
      matrices[i] = glm::translate(glm::mat4{1.0f}, motions[i].position);
      matrices[i] = glm::rotate(matrices[i], motions[i].rotation.angle(), motions[i].rotation.axis());
      matrices[i] = glm::scale(matrices[i], motions[i].scale);
    }

    m_layout.bind();
    m_instanceBuffer.gen();
    m_instanceBuffer.bind();
    m_instanceBuffer.send(matrices);

    m_layout.instance(location);
  }

  void Ditto::draw()
  {
    s_spaces.mvp = s_spaces.projection * s_spaces.view;

    m_shader.activate();
    m_shader.set("mvp", s_spaces.mvp);

    m_layout.bind();
    glDrawElementsInstanced(GL_TRIANGLES, m_element.indiceCount, GL_UNSIGNED_INT, 0, m_instanceCount);
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