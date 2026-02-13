#include "Window.h"
#include "Callback.h"

#include <iostream>

#include <glad/glad.h>
#include <glfw3.h>

#include <stb_image.h>

Window::Window(const char* title, const char* path_to_icon, float widthRatio, float heightRatio, GLFWmonitor* screenMode, GLFWwindow* share)
{

  /*
    FIX ME!!

    Currently, the engine works with singletons, when it really doesn't need them.
    The GLFW callback system works off of this, but is getting segfaulted.

    Need to use: glfwSetWindowUserPointer(), set it to the engine class, and create
    everything inside of main to make sure it there isn't global singleton issues.
  */

  //glfw init
  if (!glfwInit())
  {
    std::cerr << "GLFW Failed to initialize\n";
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  const GLFWvidmode* monitor{ glfwGetVideoMode(glfwGetPrimaryMonitor()) };

  std::cout << "Monitor pointer: " << monitor << '\n';

  //finding ratio related to screen width
  int windowWidth{ static_cast<int>(monitor->width / widthRatio) };
  int windowHeight{ static_cast<int>(windowWidth - (windowWidth / heightRatio)) };

  //centering window
  int finalPosX{ (monitor->width / 2) - (windowWidth / 2) };
  int finalPosY{ (monitor->height / 2) - (windowHeight / 2) };

  glfwWindowHint(GLFW_POSITION_X, finalPosX);
  glfwWindowHint(GLFW_POSITION_Y, finalPosY);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  //creating window
  window_ = glfwCreateWindow(windowWidth, windowHeight, title, screenMode, share);

  //checking if window was created successfully
  if (!window_)
  {
    std::cerr << "ERROR::MAIN.CPP::WINDOW()::FAILURE_IN_WINDOW_CREATION\n";
    glfwTerminate();
    return;
  }
  
  //setting current context to window
  glfwMakeContextCurrent(window_);

  //v-sync turned off
  glfwSwapInterval(0);

  //loading GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "ERROR::MAIN.CPP::WINDOW()::FAILURE_IN_INITIALIZING_GLAD\n";
    glfwTerminate();
  }

  //setting window icon
  GLFWimage images[1]{};
  images[0].pixels = stbi_load(path_to_icon, &images[0].width, &images[0].height, 0, 4); //rgba channels 
  glfwSetWindowIcon(window_, 1, images);
  stbi_image_free(images[0].pixels);

  //setting callbacks
  glfwSetFramebufferSizeCallback(window_, Callback::framebuffer);
  glfwSetWindowRefreshCallback(window_, Callback::refresh);
}

void Window::title(const char* title)
{
  glfwSetWindowTitle(window_, title);
}

void Window::size(Dimensions dimensions)
{
  dimensions_ = dimensions;
  glViewport(0, 0, dimensions.width, dimensions.height);
}
