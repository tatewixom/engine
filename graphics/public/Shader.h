#pragma once

#include <iostream>
#include <string_view>
#include <utility>
#include <vector>

#include <glad/glad.h>

#include <glm.hpp>
#include <gtc/type_ptr.hpp>

namespace Nuke
{
  // compiles shader programs
  class Shader
  {
  public:
    enum Variables
    {
      projection,
      model,
      view,
      ortho,
      mvp,
    };

    Shader() = default;
    ~Shader();

    // implicity converts shader object to GLuint m_program when used in glUseProgram function (and like functions)
    operator GLuint() const { return program_; }

    void initialize(std::string_view vertex, std::string_view fragment);

    void destroyProgram() const { glDeleteProgram(program_); }

    void compile(const GLuint shader, const char *contents);
    void link(GLuint program, GLuint vertex, GLuint fragment);

    void set(std::string_view var, glm::vec4 vec) const;
    void set(std::string_view var, glm::vec3 vec) const;
    void set(std::string_view var, glm::vec2 vec) const;
    void set(std::string_view var, float x) const;

    template <typename T>
    void set(const std::vector<std::pair<std::string_view, T>> &vec);

    void set(std::string_view var, int x) const;

    void set(Shader::Variables var, const glm::mat4 &mat) const;

    void set(std::string_view var, const glm::mat4 &mat) const;

    void activate() const { glUseProgram(program_); }
    void deactivate() const { glUseProgram(0); }

  private:
    GLuint program_{};
    GLuint vertex_{};
    GLuint fragment_{};
  };

  template <typename T>
  inline void Shader::set(const std::vector<std::pair<std::string_view, T>> &vec)
  {
    for (const auto &pair : vec)
      set(pair.first, pair.second);
  }
}