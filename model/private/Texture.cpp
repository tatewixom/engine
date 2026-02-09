#include "Texture.h"

std::deque<Texture*> Texture::s_textureQueue{};
int Texture::s_activeUnits{};

void Texture::initialize(std::string_view path, Type type)
{
  //assigning type
  m_type = type;

  //look into adding a guard for double initialization
  //or perhaps make another initialization perceived as changing texture to new one

  if (s_textureQueue.size() < getMaxUnits())
  {
    //assigning current texture to current units active + 1
    m_assignedUnit = static_cast<int>(GL_TEXTURE0) + s_activeUnits;
    ++s_activeUnits;
  }
  else
  {
    //assigning current texture to back-of-queue's texture unit
    m_assignedUnit = s_textureQueue.back()->m_assignedUnit;

    //nullifying back-of-queue's texture
    s_textureQueue.back()->m_assignedUnit = s_null_tex;

    //removing unused texture from queue
    s_textureQueue.pop_back();
  }

  //load texture based off path
  load(path);

  //add current texture to front of queue
  s_textureQueue.push_front(this);
}

void Texture::initialize(const unsigned char* data, int width, int height, std::string_view format, Type type)
{
  //assigning type
  m_type = type;

  //look into adding a guard for double initialization
  //or perhaps make another initialization perceived as changing texture to new one

  if (s_textureQueue.size() < getMaxUnits())
  {
    //assigning current texture to current units active + 1
    m_assignedUnit = static_cast<int>(GL_TEXTURE0) + s_activeUnits;
    ++s_activeUnits;
  }
  else
  {
    //assigning current texture to back-of-queue's texture unit
    m_assignedUnit = s_textureQueue.back()->m_assignedUnit;

    //nullifying back-of-queue's texture
    s_textureQueue.back()->m_assignedUnit = s_null_tex;

    //removing unused texture from queue
    s_textureQueue.pop_back();
  }

  //load texture based off raw data from gltf
  loadFromGLTF(data, width, height, format);


  //add current texture to front of queue
  s_textureQueue.push_front(this);
}

int Texture::use()
{
  //check to see if texture is currently tied to a unit
  if (m_assignedUnit != s_null_tex)
  {
    glActiveTexture(static_cast<GLenum>(m_assignedUnit));
    glBindTexture(GL_TEXTURE_2D, m_id); //bind texture just in case
    return m_assignedUnit - static_cast<int>(GL_TEXTURE0); //return if not null
  }
  else
  {
    //assigning current texture the back-of-queue's texture unit
    m_assignedUnit = s_textureQueue.back()->m_assignedUnit;

    //binding current textures id to new unit
    glActiveTexture(static_cast<GLenum>(m_assignedUnit));
    glBindTexture(GL_TEXTURE_2D, m_id);
    glActiveTexture(GL_TEXTURE0); //setting to default

    //nullifying back-of-queue's texture
    s_textureQueue.back()->m_assignedUnit = s_null_tex;

    //popping null texture at back of queue
    s_textureQueue.pop_back();

    //pushing current texture to front of queue
    s_textureQueue.push_front(this);

    //reuturning assigned texture unit
    return m_assignedUnit - static_cast<int>(GL_TEXTURE0);
  }
}

bool Texture::isRGBA(std::string_view sv) const
{
  //initially look for '.' char 
  std::string_view::size_type sv_pos{ sv.find_last_of('.') };

  //check if it wasn't found
  if (sv_pos == std::string_view::npos)
  {
    //instead look for last of '/' char
    sv_pos = sv.find_last_of('/');

    //check if it was also not found
    if (sv_pos == std::string_view::npos)
    {
      std::cerr << "ERROR::TEXTURE.CPP::ISRGBA()::UNABLE_TO_IDENTIFY_IMAGE_TYPE\n"
        << "WARNING::TEXTURE.CPP::ISRGBA()::RESORTING_TO_RGBA\n";

      //resort to RGBA format
      return true;
    }
  }

  //get string_view of characters after last of chosen chars ('.', '/')
  std::string_view format{ sv.substr(sv_pos + 1) };

  //constant static "png" string for detecting RGBA format
  static constexpr std::string_view png{ "png" };

  //return whether its a .png or not 
  return format == png;
  /* THIS ASSUMES THAT .PNG IS THE ONLY FORMAT THAT IS RGBA */
}

void Texture::load(const std::string_view format)
{
  //activating assigned texture unit
  glActiveTexture(static_cast<GLenum>(m_assignedUnit));

  //generating texture, assigning texture id, and binding texture
  glGenTextures(1, &m_id);
  glBindTexture(GL_TEXTURE_2D, m_id);

  //specifying image wrapping to gl
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

  //specifying mipmap filter to gl
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width{}, height{}, channels{};
  unsigned char* data{ stbi_load(format.data(), &width, &height, &channels, 0) };

  if (!data)
    std::cerr << "ERROR::TEXTURE.CPP::LOAD()::DATA_CHAR_ARRAY_EMPTY\n";
  else
  {
    if (isRGBA(format))
    {
      //send image data to gl with RGBA format
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
      //send image data to gl with RGB format
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
  }

  stbi_image_free(data);
}

void Texture::loadFromGLTF(const unsigned char* data, int width, int height, std::string_view format)
{
  //activating assigned texture unit
  glActiveTexture(static_cast<GLenum>(m_assignedUnit));

  //generating texture, assigning texture id, and binding texture
  glGenTextures(1, &m_id);
  glBindTexture(GL_TEXTURE_2D, m_id);

  //specifying image wrapping to gl
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

  // specifying mipmap filter to gl
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  //check if data is empty (null)
  if (!data)
    std::cerr << "ERROR::TEXTURE.CPP::LOAD()::DATA_CHAR_ARRAY_EMPTY\n";
  else
  {
    if (isRGBA(format))
    {
      //send image data to gl with RGBA format
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
      //send image data to gl with RGB format
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
  }
}

GLint Texture::getMaxUnits() const 
{
  GLint maxUnits{};

  //gl function made for accessing max texture units on system
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxUnits);

  return maxUnits;
}

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