#include "Shader.h"
#include "File.h"

#include <expected>

static constexpr std::string_view getString(Shader::Variables var)
{
  switch (var)
  {
  case Shader::projection:
    return "projection";
  case Shader::model:
    return "model";
  case Shader::view:
    return "view";
  case Shader::ortho:
    return "ortho";
  case Shader::mvp:
    return "mvp";
  default:
    return "ERROR::SHADER.CPP::GETSTRING()::UNKNOWN_ENUMERATION\n";
  }
}

Shader::~Shader()
{
  deactivate();

  //deleting vertex and fragment objects
  glDeleteShader(m_vertex);
  glDeleteShader(m_fragment);

  glDeleteProgram(m_program);
}

void Shader::initialize(std::string_view vertex, std::string_view fragment)
{
  m_program = glCreateProgram();
  m_vertex = glCreateShader(GL_VERTEX_SHADER);
  m_fragment = glCreateShader(GL_FRAGMENT_SHADER);

  static_assert(__cplusplus >= 202100L);

  auto vertexFile{ File::retrieve(vertex) };
  auto fragmentFile{ File::retrieve(fragment) };

  if ((!vertexFile) || (!fragmentFile))
    ;
  else
  {
    //compile vertex shader
    const char* vertContents{ vertexFile.value().c_str() };
    compile(m_vertex, vertContents);

    //compile fragment shader
    const char* fragContents{ fragmentFile.value().c_str() };
    compile(m_fragment, fragContents);

    //link shaders to program
    link(m_program, m_vertex, m_fragment);
  }
}

void Shader::compile(const GLuint shader, const char* contents)
{
  GLint success{};

  glShaderSource(shader, 1, &contents, nullptr);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  //error handling
  if (!success)
  {
    char infoLog[512]{};
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    std::cerr << "ERROR::SHADER.H::COMPILE()::FAILURE_IN_COMPILATION_OF_SHADER\n";
    std::cerr << infoLog << '\n';
  }
}

void Shader::link(GLuint program, GLuint vertex, GLuint fragment)
{
  GLint success{};

  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  
  //error handling
  if (!success)
  {
    char infoLog[512]{};
    glGetProgramInfoLog(program, 512, nullptr, infoLog);
    std::cerr << "ERROR::SHADER.H::COMPILE()::FAILURE_ATTACHING_SHADERS_TO_PROGRAM\n";
    std::cerr << infoLog << '\n';
  }
}

void Shader::set(std::string_view var, glm::vec4 vec) const
{
  glUniform4f(glGetUniformLocation(m_program, var.data()), vec.x, vec.y, vec.z, vec.w);
}

void Shader::set(std::string_view var, glm::vec3 vec) const
{
  glUniform3f(glGetUniformLocation(m_program, var.data()), vec.x, vec.y, vec.z);
}

void Shader::set(std::string_view var, glm::vec2 vec) const
{
  glUniform2f(glGetUniformLocation(m_program, var.data()), vec.x, vec.y);
}

void Shader::set(std::string_view var, float x) const
{
  glUniform1f(glGetUniformLocation(m_program, var.data()), x);
}

void Shader::set(std::string_view var, int x) const
{
  glUniform1i(glGetUniformLocation(m_program, var.data()), x);
}

void Shader::set(Shader::Variables var, const glm::mat4& mat) const
{
  glUniformMatrix4fv(glGetUniformLocation(m_program, getString(var).data()), 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::set(std::string_view var, const glm::mat4& mat) const
{
  glUniformMatrix4fv(glGetUniformLocation(m_program, var.data()), 1, GL_FALSE, glm::value_ptr(mat));
}
