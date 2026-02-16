#include "Object.h"
#include "Camera.h"
#include "Shader.h"
#include "Window.h"

namespace Nuke
{
  Object::Spaces Object::s_spaces{};

  Object::Object(const Buffer &buffer, glm::vec3 position, const Material &material)
      : position_{ position }
      , material_{ material }
      , VAO_{ buffer.getVAO() }
  {
  }

  Object::Object(const GLuint VAO, glm::vec3 position, const Material &material)
      : position_{position}
      , material_{material}
      , VAO_{ VAO }
  {
  }

  void Object::initialize(const Buffer &buffer)
  {
    VAO_ = buffer.getVAO();
  }

  void Object::move(const glm::vec3 &position)
  {
    position_ = position;
  }

  void Object::rotate(const Rotation &rotation)
  {
    rotation_ = rotation;
  }

  void Object::scale(const glm::vec3 &scalar)
  {
    dimensions_.scalar = scalar;
  }

  void Object::update(const Camera &camera, const Window &window)
  {
    const Window::Dimensions dimensions{window.dimensions()};

    s_spaces.projection = glm::perspective(
        glm::radians(camera.fov),
        static_cast<float>(dimensions.height) / static_cast<float>(dimensions.width),
        camera.nearPlane, camera.farPlane);

    s_spaces.view = camera.getViewMatrix();
  }

  void Object::draw(const Shader &shader) const
  {
    s_spaces.model = glm::mat4{1.f};

    s_spaces.model = glm::translate(s_spaces.model, position_);
    s_spaces.model = glm::rotate(s_spaces.model, rotation_.angle(), rotation_.axis());
    s_spaces.model = glm::scale(s_spaces.model, dimensions_.scalar);

    s_spaces.mvp = glm::mat4{s_spaces.projection * s_spaces.view * s_spaces.model};

    shader.activate();

    shader.set("mvp", s_spaces.mvp);
    shader.set(Shader::model, s_spaces.model);

    // glBindVertexArray(m_VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 36); // 36 is the amount of vertices

    glBindVertexArray(VAO_);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  }

  void Object::material(const Material &material)
  {
    material_ = material;
  }

  Object::Rotation::Rotation(const glm::vec3 &axis, float angle)
      : m_axis{axis}, m_angle{angle}
  {
    if (m_axis == glm::vec3{0.f, 0.f, 0.f})
      m_axis = glm::vec3{0.f, 1.f, 0.f};
  }
}