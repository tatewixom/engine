#include "Window.h"
#include "Callback.h"

#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

namespace Nuke
{
  Window::Window(const char *title, const char *path_to_icon, float widthRatio, float heightRatio, GLFWmonitor *screenMode, GLFWwindow *share)
  {
    // setting window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    const GLFWvidmode *monitor{glfwGetVideoMode(glfwGetPrimaryMonitor())};
    if (!monitor)
      throw std::runtime_error{"ERROR::WINDOW.CPP::WINDOW()::FAILED_TO_GET_PRIMARY_MONITOR"};

    // finding ratio related to screen width
    int windowWidth{static_cast<int>(monitor->width / widthRatio)};
    int windowHeight{static_cast<int>(windowWidth - (windowWidth / heightRatio))};

    // set window dimensions
    dimensions_ = Dimensions{windowHeight, windowWidth};

    // centering window
    int finalPosX{(monitor->width / 2) - (windowWidth / 2)};
    int finalPosY{(monitor->height / 2) - (windowHeight / 2)};

    glfwWindowHint(GLFW_POSITION_X, finalPosX);
    glfwWindowHint(GLFW_POSITION_Y, finalPosY);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // creating window
    window_ = glfwCreateWindow(windowWidth, windowHeight, title, screenMode, share);

    // checking if window was created successfully
    if (!window_)
    {
      std::cerr << "ERROR::WINDOW.CPP::WINDOW()::FAILURE_IN_WINDOW_CREATION\n";
      glfwTerminate();
      return;
    }

    // setting current context to window
    glfwMakeContextCurrent(window_);

    // turning on v-sync by default
    glfwSwapInterval(1);

    // loading GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      std::cerr << "ERROR::WINDOW.CPP::WINDOW()::FAILURE_IN_INITIALIZING_GLAD\n";
      glfwTerminate();
      return;
    }

    // setting window icon
    std::string root{std::filesystem::current_path().string() + '/'};
    std::string path{root + std::string{path_to_icon}};

    GLFWimage icon{};
    icon.pixels = stbi_load(path.c_str(), &icon.width, &icon.height, 0, 4); // rgba channels
    if (icon.pixels)
    {
      glfwSetWindowIcon(window_, 1, &icon);
      stbi_image_free(icon.pixels);
    }

    // setting window callbacks
    glfwSetFramebufferSizeCallback(window_, Callback::framebuffer);
    glfwSetWindowRefreshCallback(window_, Callback::refresh);
  }

  void Window::title(const char *title)
  {
    glfwSetWindowTitle(window_, title);
  }

  void Window::size(Dimensions dimensions)
  {
    dimensions_ = dimensions;
    glViewport(0, 0, dimensions.width, dimensions.height);
  }

  void Window::vsync(bool val)
  {
    if (val)
      glfwSwapInterval(1);
    else
      glfwSwapInterval(0);
  }
}