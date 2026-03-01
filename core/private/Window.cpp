#include "Window.h"
#include "Callback.h"
#include "File.h"

#include <iostream>
#include <stdexcept>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

namespace Nuke
{
  /*
    The old mindset was to try and set the window and frame buffer size to something
    nice looking, however, this doesn't work portably.

    So, it is best to have a "reaction-based" approach, where we just rely on the user
    to set the window size to what they please. 

    In the future, it would be nice to add a mechanism where the engine remembers the 
    previous window size and settings (fullscreen, width & height, etc).
  */

  Window::Window(const char* title, const char* path_to_icon, float widthRatio, float heightRatio)
  {
    // setting window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif

    // create "basic" window
    // meaning, this is the default values if other options arent found
    window_ = glfwCreateWindow(800, 600, title, nullptr, nullptr);
    if (!window_)
    {
      std::cerr << "[window] fatal: window failed to create\n";
      glfwTerminate();
      return; 
    }

    glfwMakeContextCurrent(window_);

    //get window size from FRAMEBUFFER, not WINDOW. 
    glfwGetFramebufferSize(window_, &dimensions_.width, &dimensions_.height);
    glfwSetFramebufferSizeCallback(window_, Callback::framebuffer);
      
    // setting window icon
    std::filesystem::path path{ File::get_executable_dir() / std::filesystem::path{ path_to_icon } };
    GLFWimage icon{};
    icon.pixels = stbi_load(path.c_str(), &icon.width, &icon.height, 0, 4); // rgba channels
    if (icon.pixels)
    {
      glfwSetWindowIcon(window_, 1, &icon);
      stbi_image_free(icon.pixels);
    }
    else
      std::cerr << "[window] unable to load or locate window icon\n[window] file path: " << path << '\n';
  }

  void Window::title(const char* title) const 
  {
    glfwSetWindowTitle(window_, title);
  }

  void Window::size(Dimensions dimensions) noexcept
  {
    dimensions_ = dimensions;
    glViewport(0, 0, dimensions.width, dimensions.height);
  }

  void Window::vsync(bool val) const
  {
    if (val)
      glfwSwapInterval(1);
    else
      glfwSwapInterval(0);
  }
}