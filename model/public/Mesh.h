#pragma once

#include "Texture.h"

#include <glad/glad.h>
#include <gtc/matrix_transform.hpp>

#include <vector>
#include <array>
#include <utility>

namespace Nuke
{
  struct Vertex
  {
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    glm::vec2 texture{0.0f, 0.0f};
    glm::vec3 normal{0.0f, 0.0f, 0.0f};
  };

  struct Attribute
  {
    unsigned int location{};
    int size{};
    int stride{};
    int offset{};
  };

  struct Layout
  {
    Layout() = default;

    void initialize(std::vector<Attribute> &&newAttributes);

    void bind() const { glBindVertexArray(VAO); }
    void unbind() const { glBindVertexArray(0); }
    void interpret();
    void instance();
    void instance(unsigned int location);

    std::vector<Attribute> attributes{};
    unsigned int VAO{};
  };

  struct VertexBuffer
  {
    VertexBuffer() = default;

    void initialize(const std::vector<Vertex> &vertices);

    void gen() { glGenBuffers(1, &VBO); }
    void bind() const { glBindBuffer(GL_ARRAY_BUFFER, VBO); }
    void send(const std::vector<Vertex> &vertices);
    void send(const std::vector<glm::mat4> &matrices);

    unsigned int VBO{};
    unsigned int vertexCount{};
  };

  struct Element
  {
    Element() = default;

    template <typename T>
    void initialize(const std::vector<T> &indices)
    {
      gen();
      bind();
      send(indices);

      indiceCount = indices.size();
    }

    void gen() { glGenBuffers(1, &EBO); }
    void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); }

    template <typename T>
    void send(const std::vector<T> &indices)
    {
      if (indices.size() > 0)
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(T), indices.data(), GL_STATIC_DRAW);
      else
        std::cerr << "ERROR::MESH.CPP::ELEMENT::SEND()::INDICE_ARRAY_WITH_SIZE_BELOW_ONE\n";
    }

    unsigned int EBO{};
    std::size_t indiceCount{};
    GLenum indexType{GL_UNSIGNED_INT};
  };

  class Shader;

  class Mesh
  {
  public:
    template <typename T>
    void initialize(const std::vector<Vertex> &vertices, const std::vector<T> &indices, std::vector<Attribute> &&attributes, std::vector<Texture> &&textures)
    {
      textures_ = std::move(textures);

      /*
      order that seems to work is:
      initialize VAO ->
      bind VAO ->
      initialize VBO ->
      bind VBO ->
      interpret VAO attributes ->
      initialize EBO
      */

      layout_.initialize(std::move(attributes)); // initialize & bind VAO

      buffer_.initialize(vertices); // initialize & bind VBO

      element_.initialize(indices); // initialize & bind EBO

      /* this MUST be interpreted AFTER the VBO has been initialized */
      layout_.interpret(); // interpret attributes
    }

    template <typename T>
    void initialize(const std::vector<Vertex> &vertices, const std::vector<T> &indices, std::vector<Attribute> &&attributes)
    {
      /*
      order that seems to work is:
      initialize VAO ->
      bind VAO ->
      initialize VBO ->
      bind VBO ->
      interpret VAO attributes ->
      initialize EBO
      */

      layout_.initialize(std::move(attributes)); // initialize & bind VAO

      buffer_.initialize(vertices); // initialize & bind VBO

      element_.initialize(indices); // initialize & bind EBO

      /* this MUST be interpreted AFTER the VBO has been initialized */
      layout_.interpret(); // interpret attributes
    }

    void changeIndexType(GLenum e) { element_.indexType = e; }

    void draw(Shader &shader);

  protected:
    Layout layout_{};
    VertexBuffer buffer_{};
    Element element_{};
    std::vector<Texture> textures_{};
  };
}