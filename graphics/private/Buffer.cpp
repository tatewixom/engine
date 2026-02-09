#include "Buffer.h"

Buffer::~Buffer()
{
  glDeleteBuffers(1, &m_VBO);
  glDeleteVertexArrays(1, &m_VAO);
  glDeleteBuffers(1, &m_EBO);
}

void Buffer::initialize(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
{
  //generating vertex array object
  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  glGenBuffers(1, &m_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, getSize(vertices), vertices.data(), GL_STATIC_DRAW);

  //creating an element array buffer to store indices in to tell opengl how to draw rectangle
  glGenBuffers(1, &m_EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, getSize(indices), indices.data(), GL_STATIC_DRAW);

  //creating preferred settings for telling opengl how to connect data
  glEnableVertexAttribArray(m_position.location);
  glVertexAttribPointer(m_position.location, m_position.size, GL_FLOAT, GL_FALSE, getStride(), reinterpret_cast<void*>(m_position.offset)); //fix warning at last parameter

  //telling opengl how to interpret colors
  glEnableVertexAttribArray(m_color.location);
  glVertexAttribPointer(m_color.location, m_color.size, GL_FLOAT, GL_FALSE, getStride(), reinterpret_cast<void*>(m_color.offset)); //fix warning at last parameter

  //telling opengl how to interpret textures
  glEnableVertexAttribArray(m_texture.location);
  glVertexAttribPointer(m_texture.location, m_texture.size, GL_FLOAT, GL_FALSE, getStride(), reinterpret_cast<void*>(m_texture.offset));

  glEnableVertexAttribArray(m_normal.location);
  glVertexAttribPointer(m_normal.location, m_normal.size, GL_FLOAT, GL_FALSE, getStride(), reinterpret_cast<void*>(m_normal.offset));
  glEnableVertexAttribArray(0); //disable
}

void Buffer::initialize(const std::vector<float>& vertices)
{
  //generating vertex array object
  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  glGenBuffers(1, &m_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, getSize(vertices), vertices.data(), GL_STATIC_DRAW);

  //creating preferred settings for telling opengl how to connect data
  glEnableVertexAttribArray(m_position.location);
  glVertexAttribPointer(m_position.location, m_position.size, GL_FLOAT, GL_FALSE, getStride(), reinterpret_cast<void*>(m_position.offset)); //fix warning at last parameter
  glEnableVertexAttribArray(0); //disable

  //telling opengl how to interpret colors
  glEnableVertexAttribArray(m_color.location);
  glVertexAttribPointer(m_color.location, m_color.size, GL_FLOAT, GL_FALSE, getStride(), reinterpret_cast<void*>(m_color.offset)); //fix warning at last parameter
  glEnableVertexAttribArray(0); //disable

  //telling opengl how to interpret textures
  glEnableVertexAttribArray(m_texture.location);
  glVertexAttribPointer(m_texture.location, m_texture.size, GL_FLOAT, GL_FALSE, getStride(), reinterpret_cast<void*>(m_texture.offset));
  glEnableVertexAttribArray(0); //disable

  glEnableVertexAttribArray(m_normal.location);
  glVertexAttribPointer(m_normal.location, m_normal.size, GL_FLOAT, GL_FALSE, getStride(), reinterpret_cast<void*>(m_normal.offset));
  glEnableVertexAttribArray(0); //disable
}

void Buffer::remoteVAO(GLuint& VAO) const 
{
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // we only need to bind to the VBO, the container's VBO's data already contains the data.
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

  // set the vertex attribute 
  glEnableVertexAttribArray(m_position.location);
  glVertexAttribPointer(m_position.location, m_position.size, GL_FLOAT, GL_FALSE, getStride(), reinterpret_cast<void*>(m_position.offset));
  glEnableVertexAttribArray(0);
}

GLuint Buffer::remoteVAO() const
{
  GLuint VAO{};

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // we only need to bind to the VBO, the container's VBO's data already contains the data.
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

  // set the vertex attribute 
  glEnableVertexAttribArray(m_position.location);
  glVertexAttribPointer(m_position.location, m_position.size, GL_FLOAT, GL_FALSE, getStride(), reinterpret_cast<void*>(m_position.offset));
  glEnableVertexAttribArray(0);

  return VAO;

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindVertexArray(0);

  // we only need to bind to the VBO, the container's VBO's data already contains the data.
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}
