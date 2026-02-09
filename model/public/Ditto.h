#pragma once

#include "Shader.h"
#include "Mesh.h"

#include <glad/glad.h>
#include <glm.hpp>

class Camera;
class Window;

class Ditto : public Mesh
{
public:
  struct Spaces
  {
    glm::mat4 projection{ 1.f };
    glm::mat4 view{ 1.f };
    glm::mat4 mvp{ 1.f };
  };

  class Rotation
  {
  public:
    Rotation() = default;

    Rotation(const glm::vec3& axis, float angle);

    auto angle() const { return glm::radians(m_angle); }
    void angle(const float angle) { m_angle = angle; }

    auto axis() const { return glm::normalize(m_axis); }
    void axis(const glm::vec3& axis) { m_axis = axis; }

  private:
    glm::vec3 m_axis{ 0.f, 1.f, 0.f };
    float m_angle{ 0.f };
  };

  struct Motions
  {
    glm::vec3 position{};
    glm::vec3 scale{ 1.f };
    Rotation rotation{};
  };

  template <typename T>
  void initialize(const std::vector<Vertex>& vertices, const std::vector<T>& indices, std::vector<Attribute>&& attributes)
  {
    Mesh::initialize(vertices, indices, std::move(attributes));
  }

  void sendInstances(std::vector<glm::mat4> matrices, const std::vector<Motions>& motions, unsigned int location);

  void setShader(Shader& shader) { m_shader = shader; };

  void draw();

public:
  static void update(const Camera& camera, const Window& window);

private:
  VertexBuffer m_instanceBuffer{};
  Shader m_shader;
  std::size_t m_instanceCount{};

private:
  static Spaces s_spaces;
};