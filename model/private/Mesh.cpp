#include "Mesh.h"
#include "Random.h"
#include "Shader.h"
#include "Spaces.h"

#include <iostream>

/*
std::vector<Vertex> vertex
{
  Vertex{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f }, { 0.0f,  0.0f, -1.0f } },
  Vertex{ { 0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f }, { 0.0f,  0.0f, -1.0f } },
  Vertex{ { 0.5f, 0.5f, -0.5f }, { 1.0f, 1.0f }, { 0.0f,  0.0f, -1.0f } },
  Vertex{ { -0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f }, { 0.0f,  0.0f, -1.0f } },

  Vertex{ { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f }, { 0.0f,  0.0f, 1.0f } },
  Vertex{ { 0.5f, -0.5f, 0.5f }, { 1.0f, 0.0f }, { 0.0f,  0.0f, 1.0f } },
  Vertex{ { 0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f }, { 0.0f,  0.0f, 1.0f } },
  Vertex{ { -0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f }, { 0.0f,  0.0f, 1.0f } },

  Vertex{ { -0.5f, 0.5f, 0.5f }, { 1.0f, 0.0f }, { -1.0f,  0.0f, 0.0f } },
  Vertex{ { -0.5f, 0.5f, -0.5f }, { 1.0f, 1.0f }, { -1.0f,  0.0f, 0.0f } },
  Vertex{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f }, { -1.0f,  0.0f, 0.0f } },
  Vertex{ { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f }, { -1.0f,  0.0f, 0.0f } },

  Vertex{ { 0.5f, 0.5f, 0.5f }, { 1.0f, 0.0f }, { 1.0f,  0.0f, 0.0f } },
  Vertex{ { 0.5f, 0.5f, -0.5f }, { 1.0f, 1.0f }, { 1.0f,  0.0f, 0.0f } },
  Vertex{ { 0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f }, { 1.0f,  0.0f, 0.0f } },
  Vertex{ { 0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f }, { 1.0f,  0.0f, 0.0f } },

  Vertex{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f }, { 0.0f,  -1.0f, 0.0f } },
  Vertex{ { 0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f }, { 0.0f,  -1.0f, 0.0f } },
  Vertex{ { 0.5f, -0.5f, 0.5f }, { 1.0f, 0.0f }, { 0.0f,  -1.0f, 0.0f } },
  Vertex{ { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f }, { 0.0f,  -1.0f, 0.0f } },

  Vertex{ { -0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f }, { 0.0f,  1.0f, 0.0f } },
  Vertex{ { 0.5f, 0.5f, -0.5f }, { 1.0f, 1.0f }, { 0.0f,  1.0f, 0.0f } },
  Vertex{ { 0.5f, 0.5f, 0.5f }, { 1.0f, 0.0f }, { 0.0f,  1.0f, 0.0f } },
  Vertex{ { -0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f }, { 0.0f,  1.0f, 0.0f } },
};

std::vector<Indices> indices
{
  0, 1, 2,
  2, 3, 0,

  4, 5, 6,
  6, 7, 4,

  8, 9, 10,
  10, 11, 8,

  12, 13, 14,
  14, 15, 12,

  16, 17, 18,
  18, 19, 16,

  20, 21, 22,
  22, 23, 20,
};
*/

/*

//attributes
for (const auto& attrib : newAttributes)
{
  glEnableVertexAttribArray(attrib.location);
  glVertexAttribPointer(attrib.location, attrib.size, GL_FLOAT, GL_FALSE, attrib.stride, reinterpret_cast<void*>(attrib.offset));
}

//buffer data
if (vertices.size() > 0)
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
else
  std::cerr << "ERROR::MESH.CPP::VERTEXBUFFER_T::SEND()::VERTICE_ARRAY_WITH_SIZE_BELOW_ONE\n";

//Mesh
void Mesh_t::draw(Shader& shader)
{
  Spaces& spaces{ Spaces::getInstance() };

  spaces.model = glm::mat4{ 1.f };

  glm::vec3 position{ 0.f, 0.f, 0.f };
  Rotation rotation{};
  Dimension dimensions{};
  dimensions.scalar = glm::vec3{ 5.f };

  spaces.model = glm::translate(spaces.model, position);
  spaces.model = glm::rotate(spaces.model, rotation.angle(), rotation.axis());
  spaces.model = glm::scale(spaces.model, dimensions.scalar);

  spaces.mvp = glm::mat4{ spaces.projection * spaces.view * spaces.model };

  shader.activate();
  shader.set("mvp", spaces.mvp);

  glBindVertexArray(m_layout.VAO);
  glDrawElements(GL_TRIANGLES, m_element.indiceCount, GL_UNSIGNED_INT, 0);
}
*/

void Layout::initialize(std::vector<Attribute>&& newAttributes)
{
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  attributes = std::move(newAttributes);
}

void Layout::interpret()
{
  for (const auto& attrib : attributes)
  {
    glEnableVertexAttribArray(attrib.location);
    glVertexAttribPointer(attrib.location, attrib.size, GL_FLOAT, GL_FALSE, attrib.stride, reinterpret_cast<void*>(attrib.offset));
    glEnableVertexAttribArray(0); //disable
  }
}

void Layout::instance() //add attribute location here & make overloaded instance() for automatic location
{
  std::size_t vec4Size{ sizeof(glm::vec4) };
  for (GLuint i{ 0 }; i < 4; ++i)
  {
    glEnableVertexAttribArray(4u + i);
    glVertexAttribPointer(4u + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(i * vec4Size));
    glVertexAttribDivisor(4u + i, 1); // advance per instance
    glEnableVertexAttribArray(0); //disable
  }

  //allow for definition of starting attribute location and later automatic attribute handling.
  //this will most likely be the source of most problems
}

void Layout::instance(unsigned int location)
{
  std::size_t vec4Size{ sizeof(glm::vec4) };
  for (GLuint i{ 0 }; i < 4; ++i)
  {
    glEnableVertexAttribArray(location + i);
    glVertexAttribPointer(location + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(i * vec4Size));
    glVertexAttribDivisor(location + i, 1); // advance per instance
    glEnableVertexAttribArray(0); //disable
  }
}

void VertexBuffer::initialize(const std::vector<Vertex>& vertices)
{
  vertexCount = vertices.size();

  glGenBuffers(1, &VBO);
  bind();
  send(vertices);
}

void VertexBuffer::send(const std::vector<Vertex>& vertices)
{
  if (vertices.size() > 0)
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
  else
    std::cerr << "ERROR::MESH.CPP::VERTEXBUFFER::SEND()::VERTICE_ARRAY_WITH_SIZE_BELOW_ONE\n";
}

void VertexBuffer::send(const std::vector<glm::mat4>& matrices)
{
  if (matrices.size() > 0)
    glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(*matrices.data()), matrices.data(), GL_STATIC_DRAW);
  else
    std::cerr << "ERROR::MESH.CPP::VERTEXBUFFER::SEND()::MATRICE_ARRAY_WITH_SIZE_BELOW_ONE\n";
}

void Mesh::draw(Shader& shader)
{
  shader.activate();

  std::size_t baseNum{ 1 };
  std::size_t normNum{ 1 };
  for (auto& t : textures_)
  {
    int unit{ t.use() };

    switch (t.type())
    {
    case Texture::Type::base:
    {
      std::string str{ "texture_base_" + std::to_string(baseNum) };
      shader.set(str, unit);
      ++baseNum;
      break;
    }
    case Texture::Type::normal:
    {
      std::string str{ "texture_norm_" + std::to_string(normNum) };
      shader.set(str, unit);
      ++normNum;
      break;
    }
    default:
      std::cerr << "ERROR::MESH.CPP::DRAW()::NEED_TO_IMPLEMENT_NEW_TEXTURE_TYPE\n";
      break;
    }
  }

  m_layout.bind();
  if (m_element.indiceCount >= 0)
    glDrawElements(GL_TRIANGLES, m_element.indiceCount, GL_UNSIGNED_SHORT, 0); //using short because of   
  else                                                                       //lack of option to return different vector types
    glDrawArrays(GL_TRIANGLES, 0, m_buffer.vertexCount);

  m_layout.unbind();
  shader.deactivate();
}
