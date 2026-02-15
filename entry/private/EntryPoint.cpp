#include "Engine.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>

int main()
{
  if (!glfwInit()) 
    throw std::runtime_error{ "ERROR::MAIN.CPP::MAIN()::GLFW_FAILED_TO_INITIALIZE" };

  //window is responsible for initializing GLAD
  Window window{ "LearnOpenGL", "assets/icon/container.jpg" };
  window.vsync(false);

  //Engine MUST have a window object in order to run
  Engine engine{ window };
  engine.run();

  return 0;
}