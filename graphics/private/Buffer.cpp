#include "Buffer.h"

namespace Nuke
{
  Buffer::~Buffer()
  {
    glDeleteBuffers(1, &VBO_);
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &EBO_);
  }

  void Buffer::initialize(const std::vector<float> &vertices, const std::vector<unsigned int> &indices)
  {
    // generating vertex array object
    glGenVertexArrays(1, &VAO_);
    glBindVertexArray(VAO_);

    glGenBuffers(1, &VBO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, getSize(vertices), vertices.data(), GL_STATIC_DRAW);

    // creating an element array buffer to store indices in to tell opengl how to draw rectangle
    glGenBuffers(1, &EBO_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getSize(indices), indices.data(), GL_STATIC_DRAW);

    // creating preferred settings for telling opengl how to connect data
    glEnableVertexAttribArray(position_.location);
    glVertexAttribPointer(position_.location, position_.size, GL_FLOAT, GL_FALSE, getStride(), reinterpret_cast<void *>(position_.offset)); // fix warning at last parameter

    // telling opengl how to interpret colors
    glEnableVertexAttribArray(color_.location);
    glVertexAttribPointer(color_.location, color_.size, GL_FLOAT, GL_FALSE, getStride(), reinterpret_cast<void *>(color_.offset)); // fix warning at last parameter

    // telling opengl how to interpret textures
    glEnableVertexAttribArray(texture_.location);
    glVertexAttribPointer(texture_.location, texture_.size, GL_FLOAT, GL_FALSE, getStride(), reinterpret_cast<void *>(texture_.offset));

    glEnableVertexAttribArray(normal_.location);
    glVertexAttribPointer(normal_.location, normal_.size, GL_FLOAT, GL_FALSE, getStride(), reinterpret_cast<void *>(normal_.offset));
    glEnableVertexAttribArray(0); // disable
  }

  void Buffer::initialize(const std::vector<float> &vertices)
  {
    // generating vertex array object
    glGenVertexArrays(1, &VAO_);
    glBindVertexArray(VAO_);

    glGenBuffers(1, &VBO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, getSize(vertices), vertices.data(), GL_STATIC_DRAW);

    // creating preferred settings for telling opengl how to connect data
    glEnableVertexAttribArray(position_.location);
    glVertexAttribPointer(position_.location, position_.size, GL_FLOAT, GL_FALSE, getStride(), reinterpret_cast<void *>(position_.offset)); // fix warning at last parameter
    glEnableVertexAttribArray(0);                                                                                                           // disable

    // telling opengl how to interpret colors
    glEnableVertexAttribArray(color_.location);
    glVertexAttribPointer(color_.location, color_.size, GL_FLOAT, GL_FALSE, getStride(), reinterpret_cast<void *>(color_.offset)); // fix warning at last parameter
    glEnableVertexAttribArray(0);                                                                                                  // disable

    // telling opengl how to interpret textures
    glEnableVertexAttribArray(texture_.location);
    glVertexAttribPointer(texture_.location, texture_.size, GL_FLOAT, GL_FALSE, getStride(), reinterpret_cast<void *>(texture_.offset));
    glEnableVertexAttribArray(0); // disable

    glEnableVertexAttribArray(normal_.location);
    glVertexAttribPointer(normal_.location, normal_.size, GL_FLOAT, GL_FALSE, getStride(), reinterpret_cast<void *>(normal_.offset));
    glEnableVertexAttribArray(0); // disable
  }

  void Buffer::remoteVAO(GLuint &VAO) const
  {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);

    // set the vertex attribute
    glEnableVertexAttribArray(position_.location);
    glVertexAttribPointer(position_.location, position_.size, GL_FLOAT, GL_FALSE, getStride(), reinterpret_cast<void *>(position_.offset));
    glEnableVertexAttribArray(0);
  }

  GLuint Buffer::remoteVAO() const
  {
    GLuint VAO{};

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);

    // set the vertex attribute
    glEnableVertexAttribArray(position_.location);
    glVertexAttribPointer(position_.location, position_.size, GL_FLOAT, GL_FALSE, getStride(), reinterpret_cast<void *>(position_.offset));
    glEnableVertexAttribArray(0);

    return VAO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindVertexArray(0);

    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  }
}