#pragma once

#include <iostream>
#include <deque>

#include <glad/glad.h>

#include <stb_image.h>

namespace Nuke
{
  class Texture
  {
  public:
    enum class Type
    {
      base = 0,
      normal,
      roughness,
    };

    void initialize(std::string_view path, Type type);
    void initialize(const unsigned char *data, int width, int height, std::string_view format, Type type);

    // returns id of texture assigned by gl
    GLint id() const noexcept { return id_; }
    Type type() const noexcept { return type_; }

    // function that makes sure texture is valid and returns assigned unit
    int use();

  private:
    bool isRGBA(std::string_view path) const;

    void load(const std::string_view path);
    void loadFromGLTF(const unsigned char *data, int width, int height, std::string_view format);

    GLint getMaxUnits() const;

  private:
    GLuint id_{}; // texture object id
    int assignedUnit_{0};
    Type type_{};

  private:
    static std::deque<Texture*> s_textureQueue;
    static int s_activeUnits;
    static constexpr int s_null_tex{ 0 };
  };
}