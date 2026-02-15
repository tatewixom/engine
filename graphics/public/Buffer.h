#pragma once

#include <vector>
#include <array>
#include <string>

#include <glad/glad.h>

/*
EXPECTED LAYOUT FOR VERTEX POSITION AND COLOR:

std::vector<float> vertices
{
  //positon (3D)        //color for vertex (RGBA) //texture coords    //normals
  0.0f,  0.0f, 0.0f,    1.0f, 0.0f, 0.0f, 1.0f,   1.0f, 1.0f,  0.0f,  0.0f, 0.0f,
  1.0f,  1.0f, 0.0f,    0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 1.0f,  0.0f,  0.0f, 0.0f,
 -1.0f,  1.0f, 0.0f,    0.0f, 0.0f, 1.0f, 1.0f,   1.0f, 1.0f,  0.0f,  0.0f, 0.0f,
 -1.0f, -1.0f, 0.0f,    0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 1.0f,  0.0f,  0.0f, 0.0f,
  1.0f, -1.0f, 0.0f,    1.0f, 0.0f, 0.0f, 1.0f,   1.0f, 1.0f,  0.0f,  0.0f, 0.0f,
  ...
}

std::vector<unsigned int> indices
{
  0, 1, 2,   // first triangle
  0, 4, 3    // second triangle
  ...
};

std::vector<float> texture
  {
    0.0f, 0.0f,  // lower-left corner
    1.0f, 0.0f,  // lower-right corner
    0.5f, 1.0f   // top-center corner
  };
*/

class Buffer
{
public:
  struct Attribute
  {
    unsigned int location{};
    int size{};
    int offset{};
  };

  Buffer() = default;
  ~Buffer();

  void initialize(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
  void initialize(const std::vector<float>& vertices);

  constexpr GLuint getVAO() const { return VAO_; }
  constexpr GLuint getEBO() const { return EBO_; }
  constexpr GLsizei getStride() const { return 12 * sizeof(float); }
  template <typename T>
  constexpr std::size_t getSize(const std::vector<T>& vec) const { return vec.size() * sizeof(T); }

  void remoteVAO(GLuint& VAO) const;
  GLuint remoteVAO() const;
private:
  static constexpr Attribute position_{ 0u, 3, 0 };
  static constexpr Attribute color_{ 1u, 4, (sizeof(float) * 3) };
  static constexpr Attribute texture_{ 2u, 2, (sizeof(float) * 7) };
  static constexpr Attribute normal_{ 3u, 3, (sizeof(float) * 9) };

  GLuint VBO_{};
  GLuint VAO_{};
  GLuint EBO_{};
};

