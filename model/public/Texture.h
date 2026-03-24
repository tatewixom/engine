#pragma once

#include <iostream>
#include <deque>

#include <glad/glad.h>
#include <glm.hpp>

namespace Nuke
{
  namespace Matter
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
      max,
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

    struct Texture
    {
      Image image{};
      Type type{};
      Factor factor{};
    };

    class Material
    {
    public:
      Material(const Texture& texture, const Sampler& sampler) noexcept;
      //~Material() noexcept { glDeleteTextures(1, &id_); }

      unsigned int id() const noexcept { return id_; }
      Type type() const noexcept { return texture_.type; }
    private:
      void load() noexcept;

    private:
      Texture texture_{};
      Sampler sampler_{};
      unsigned int id_{};
    };
  }
}