#include "Texture.h"
#include <map>
#include <filesystem>
#include <exception>

#include <tiny_gltf.h>


namespace Nuke
{
  /*
    last session summary:

    1. when loading the alien model, for some reason the component type is
    GL_UNSIGNED_INT when it actually renders with GL_UNSIGNED_SHORT. As I'm
    typing this, I realize that it's most likely caused by the container that
    I use to store indices. The container is locked at std::vector<std::uint16_t>
    Note that I still need to add 3-pathway system for accepting the three
    supported data types for indices.

    2. I tried getting the texture for the backpack to work but it ended up as is.
    I realized that the texture extrapolator was messed up as the range for the
    materialIndex on the if statement didn't include zero ([expr] > 0), which
    didn't include first material (often the only one). The RGB striped pattern
    is very specific and I guess that it still has to do with the way I'm getting
    the texture. Every other texture works except the backpack.

    3. I believe that the issue resides in two places.
    One, again, I think the way
    I am extracting the textures is bad and there's definitely an error (or
    assumption) being made. The best remedy for that would be to document the ENTIRE
    load process.
    Second, I think there is a problem with how the textures are actually being called.
    The alien mesh draws totally fine when the vehicle hasn't been initialized or drawn.
    If the vehicle is initialized but not drawn, both the backpack's and alien's texture
    is the normal map of the vehicle. When the vehicle is also drawn however, the alien and
    backpack have the same camo-pattern as the vehicle. I feel as if the logic surrounding
    the Texture class is misguided and/or is making false assumptions.

    All in all, I need to check the texture class to make sure it is assinging the correct
    units to each texture and is also returning the correct information to the caller when
    the use() function is called.

    Only when this is fixed could I make optimizations to the way the Texture and Model class
    interact as the Mesh class owns the texture-vector, but the model obviously shares the
    base color map across meshes. It wasteful as of right now, but it works (at least, I
    assume).
  */

  namespace Experimental
  {
    struct Channel
    {
      GLenum internal{};
      GLenum upload{};
      GLenum type{};
    };

    bool isRGBA(std::string_view sv) noexcept
    {
      // initially look for '.' char
      std::string_view::size_type sv_pos{ sv.find_last_of('.') };

      // check if it wasn't found
      if (sv_pos == std::string_view::npos)
      {
        // instead look for last of '/' char
        sv_pos = sv.find_last_of('/');

        // check if it was also not found
        if (sv_pos == std::string_view::npos)
        {
          std::cerr << "[texture] unable to determine image format of '"
            << sv << "'\n[texture] Resorting to RGBA format...\n";

          // resort to RGBA format
          return true;
        }
      }

      // get string_view of characters after last of chosen chars ('.', '/')
      std::string_view format{ sv.substr(sv_pos + 1) };

      // constant static "png" string for detecting RGBA format
      static constexpr std::string_view png{ "png" };

      // return whether its a .png or not
      return format == png;
      /* THIS ASSUMES THAT .PNG IS THE ONLY FORMAT THAT IS RGBA */
    }

    GLenum gltf_wrap_to_gl(int wrap) noexcept
    {
      switch (wrap)
      {
      case TINYGLTF_TEXTURE_WRAP_REPEAT:
        return GL_REPEAT;
      case TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE:
        return GL_CLAMP_TO_EDGE;
      case TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT:
        return GL_MIRRORED_REPEAT;
      default:
        return GL_REPEAT;
      }
    }

    GLenum gltf_filter_to_gl(int filter) noexcept
    {
      switch (filter)
      {
      case TINYGLTF_TEXTURE_FILTER_NEAREST: //using mipmaps
      case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR:
        return GL_NEAREST_MIPMAP_LINEAR;
      case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST:
        return GL_NEAREST_MIPMAP_NEAREST;
      case -1: //default to linear filter mipmap if not specified (-1 is unspecified via tinygltf spec)
      case TINYGLTF_TEXTURE_FILTER_LINEAR: //using mipmaps
      case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR:
        return GL_LINEAR_MIPMAP_LINEAR;
      case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST:
        return GL_LINEAR_MIPMAP_NEAREST;
      default:
        return GL_LINEAR_MIPMAP_LINEAR;
      }
    }

    GLenum gltf_pixel_type_to_gl(int type) noexcept
    {
      switch (type)
      {
      case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
        return GL_UNSIGNED_BYTE;
      case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
        return GL_UNSIGNED_SHORT;
      case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
        return GL_UNSIGNED_INT;
      case TINYGLTF_COMPONENT_TYPE_BYTE:
        return GL_BYTE;
      case TINYGLTF_COMPONENT_TYPE_SHORT:
        return GL_SHORT;
      case TINYGLTF_COMPONENT_TYPE_INT:
        return GL_INT;
      case TINYGLTF_COMPONENT_TYPE_FLOAT:
        return GL_FLOAT;
      case TINYGLTF_COMPONENT_TYPE_DOUBLE:
        return GL_DOUBLE;
      default:
        return GL_UNSIGNED_BYTE;
      }
    }

    Channel get_channel_format(const Material& material) noexcept
    {
      Channel channel{};
      channel.type = gltf_pixel_type_to_gl(material.image.layout);

      //check if sRGB
      if (material.type == Type::base)
      {
        channel.upload = GL_RGBA;

        if (material.image.channels == 4)
          channel.internal = GL_SRGB8_ALPHA8;
        else
          channel.internal = GL_SRGB8;

        return channel;
      }

      //get upload format first
      const Image& image{ material.image };
      switch (image.channels)
      {
      case 1:
        channel.upload = GL_RED;
        break;
      case 2:
        channel.upload = GL_RG;
        break;
      case 3:
        channel.upload = GL_RGB;
        break;
      case 4:
        channel.upload = GL_RGBA;
        break;
      default:
        channel.upload = GL_RGBA;
        break;
      }

      int bits{ image.bits };
      if (bits == 0)
      {
        switch (channel.type)
        {
        case GL_UNSIGNED_BYTE:
        case GL_UNSIGNED_SHORT:
        case GL_BYTE:
        case GL_SHORT:
          bits = 8;
          break;
        case GL_UNSIGNED_INT:
        case GL_INT:
        case GL_FLOAT:
          bits = 32;
          break;
        case GL_DOUBLE:
          bits = 64;
          break;
        default:
          bits = 8;
          break;
        }
      }

      if (bits == 8) //8-bit storage mechanism
      {
        if ((channel.type == GL_UNSIGNED_BYTE) || (channel.type == GL_BYTE) || (channel.type == GL_UNSIGNED_SHORT) || (channel.type == GL_SHORT))
        {
          switch (image.channels)
          {
          case 1:
            channel.internal = GL_R8;
            break;
          case 2:
            channel.internal = GL_RG8;
            break;
          case 3:
            channel.internal = GL_RGB8;
            break;
          case 4:
            channel.internal = GL_RGBA8;
            break;
          default:
            channel.internal = GL_RGBA8;
            break;
          }
        }
        else if (channel.type == GL_UNSIGNED_INT)
        {
          switch (image.channels)
          {
          case 1:
            channel.internal = GL_R8UI;
            break;
          case 2:
            channel.internal = GL_RG8UI;
            break;
          case 3:
            channel.internal = GL_RGB8UI;
            break;
          case 4:
            channel.internal = GL_RGBA8UI;
            break;
          default:
            channel.internal = GL_RGBA8UI;
            break;
          }
        }
        else if (channel.type == GL_INT)
        {
          switch (image.channels)
          {
          case 1:
            channel.internal = GL_R8I;
            break;
          case 2:
            channel.internal = GL_RG8I;
            break;
          case 3:
            channel.internal = GL_RGB8;
            break;
          case 4:
            channel.internal = GL_RGBA8I;
            break;
          default:
            channel.internal = GL_RGBA8I;
            break;
          }
        }
        else
        {
          std::cerr << "[texture] unsupported 8-bit format (GL enum): " << channel.type << '\n';
          std::cerr << "[texture] defaulting to GL_RGBA8UI\n";
          channel.internal = GL_RGBA8UI;
        }
      }
      else if (bits == 16) //16-bit storage mechanism
      {
        if ((channel.type == GL_UNSIGNED_SHORT) || (channel.type == GL_SHORT))
        {
          switch (image.channels)
          {
          case 1:
            channel.internal = GL_R16;
            break;
          case 2:
            channel.internal = GL_RG16;
            break;
          case 3:
            channel.internal = GL_RGB16;
            break;
          case 4:
            channel.internal = GL_RGBA16;
            break;
          default:
            channel.internal = GL_RGBA16;
            break;
          }
        }
        else if (channel.type == GL_UNSIGNED_INT)
        {
          switch (image.channels)
          {
          case 1:
            channel.internal = GL_R16UI;
            break;
          case 2:
            channel.internal = GL_RG16UI;
            break;
          case 3:
            channel.internal = GL_RGB16UI;
            break;
          case 4:
            channel.internal = GL_RGBA16UI;
            break;
          default:
            channel.internal = GL_RGBA16UI;
            break;
          }
        }
        else if (channel.type == GL_INT)
        {
          switch (image.channels)
          {
          case 1:
            channel.internal = GL_R16I;
            break;
          case 2:
            channel.internal = GL_RG16I;
            break;
          case 3:
            channel.internal = GL_RGB16I;
            break;
          case 4:
            channel.internal = GL_RGBA16I;
            break;
          default:
            channel.internal = GL_RGBA16I;
            break;
          }
        }
        else if (channel.type == GL_FLOAT)
        {
          switch (image.channels)
          {
          case 1:
            channel.internal = GL_R16F;
            break;
          case 2:
            channel.internal = GL_RG16F;
            break;
          case 3:
            channel.internal = GL_RGB16F;
            break;
          case 4:
            channel.internal = GL_RGBA16F;
            break;
          default:
            channel.internal = GL_RGBA16F;
            break;
          }
        }
        else
        {
          std::cerr << "[texture] unsupported 16-bit format (GL enum): " << channel.type << '\n';
          std::cerr << "[texture] defaulting to GL_RGBA16UI\n";
          channel.internal = GL_RGBA16UI;
        }
      }
      else if (bits == 32) //32-bit storage mechanism
      {
        if (channel.type == GL_UNSIGNED_INT)
        {
          switch (image.channels)
          {
          case 1:
            channel.internal = GL_R32UI;
            break;
          case 2:
            channel.internal = GL_RG32UI;
            break;
          case 3:
            channel.internal = GL_RGB32UI;
            break;
          case 4:
            channel.internal = GL_RGBA32UI;
            break;
          default:
            channel.internal = GL_RGBA32UI;
            break;
          }
        }
        else if (channel.type == GL_FLOAT)
        {
          switch (image.channels)
          {
          case 1:
            channel.internal = GL_R32F;
            break;
          case 2:
            channel.internal = GL_RG32F;
            break;
          case 3:
            channel.internal = GL_RGB32F;
            break;
          case 4:
            channel.internal = GL_RGBA32F;
            break;
          default:
            channel.internal = GL_RGBA32F;
            break;
          }
        }
        else if (channel.type == GL_INT)
        {
          switch (image.channels)
          {
          case 1:
            channel.internal = GL_R32I;
            break;
          case 2:
            channel.internal = GL_RG32I;
            break;
          case 3:
            channel.internal = GL_RGB32I;
            break;
          case 4:
            channel.internal = GL_RGBA32I;
            break;
          default:
            channel.internal = GL_RGBA32I;
            break;
          }
        }
        else
        {
          std::cerr << "[texture] unsupported 32-bit format (GL enum): " << channel.type << '\n';
          std::cerr << "[texture] defaulting to GL_RGBA32UI\n";
          channel.internal = GL_RGBA32UI;
        }
      }
      else
      {
        std::cerr << "[texture] unsupported bit count: " << bits << '\n';
        std::cerr << "[texture] supported bit counts are 8, 16, and 32\n[texture] defaulting to GL_RGBA8\n";
        channel.internal = GL_RGBA8;
      }

      return channel;
    }

    Texture::Texture(const Material& material, const Sampler& sampler) noexcept
      : material_{ material }
      , sampler_{ sampler }
    {
      // load texture based off raw data from gltf
      load();
    }

    void Texture::load() noexcept
    {
      // generating texture, assigning texture id, and binding texture
      glGenTextures(1, &id_);
      glBindTexture(GL_TEXTURE_2D, id_);

      // specifying image wrapping to gl
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gltf_wrap_to_gl(sampler_.ws));
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gltf_wrap_to_gl(sampler_.wt));

      // specifying mipmap filter to gl
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gltf_filter_to_gl(sampler_.min));
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gltf_filter_to_gl(sampler_.mag));

      // check if data is empty (null)
      if (!material_.image.data.data())
        std::cerr << "[texture] error in loading image data; image data pointer null\n";
      else
      {
        if (const Image& image{ material_.image }; isRGBA(image.format))
        {
          Channel channel{ get_channel_format(material_) };
          // send image data to gl with RGBA format
          glTexImage2D(GL_TEXTURE_2D, 0, channel.internal, image.width, image.height, 0, channel.upload, channel.type, image.data.data());
          glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
          Channel channel{ get_channel_format(material_) };
          // send image data to gl with RGB format
          glTexImage2D(GL_TEXTURE_2D, 0, channel.internal, image.width, image.height, 0, channel.upload, channel.type, image.data.data());
          glGenerateMipmap(GL_TEXTURE_2D);
        }
      }

      //unbinding for safety; allowed to removed in release builds.
      glBindTexture(GL_TEXTURE_2D, 0);
    }
  }
}