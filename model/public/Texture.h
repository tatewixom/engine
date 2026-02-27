#pragma once

#include <iostream>
#include <deque>

#include <glad/glad.h>
#include <glm.hpp>

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
    void initialize(const unsigned char* data, int width, int height, std::string_view format, Type type);

    // returns id of texture assigned by gl
    GLint id() const noexcept { return id_; }
    Type type() const noexcept { return type_; }

    // function that makes sure texture is valid and returns assigned unit
    int use();

  private:
    bool isRGBA(std::string_view path) const;

    void load(const std::string_view path);
    void loadFromGLTF(const unsigned char* data, int width, int height, std::string_view format);

    GLint getMaxUnits() const;

  private:
    GLuint id_{}; // texture object id
    int assignedUnit_{ 0 };
    Type type_{};

  private:
    static constexpr int s_null_tex{ 0 };
  };

  namespace Experimental
  {
    enum class Type
    {
      none,
      base,
      normal,
      metallic,
      roughness,
      emissive,
      occlusion,
    };

    struct Image
    {
      std::vector<unsigned char> data{};
      int width{};
      int height{};
      std::string format{};
      int channels{};
      int layout{};
      int bits{};
    };

    struct Sampler
    {
      int min{};
      int mag{};
      int ws{};
      int wt{};
    };

    using Factor = glm::vec4;

    struct Material
    {
      Image image{};
      Type type{};
      Factor factor{};
    };

    //TODO: flip Material and Texture names. Material is more encapsulating and texture doesn't make sense for the whole

    class Texture
    {
    public:
      Texture(const Material& material, const Sampler& sampler) noexcept;
      //~Texture() noexcept { glDeleteTextures(1, &id_); }

      unsigned int id() const noexcept { return id_; }
      Type type() const noexcept { return material_.type; }
    private:
      void load() noexcept;

    private:
      Material material_{};
      Sampler sampler_{};
      unsigned int id_{};
    };
  }
}