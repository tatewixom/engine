#include "Engine.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>

int main()
{
  if (!glfwInit())
    return -1;

  //initialize window (glfw needs initialized and valid)
  Nuke::Window window{ "LearnOpenGL", "assets/icon/container.jpg" };
  window.vsync(false);

  //initialize glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    glfwTerminate();
    return -1;
  }

  //window couldn't make gl calls until glad was valid
  window.refresh();

  //Engine MUST have a window object and glad MUST be valid
  Nuke::Engine engine{ window };
  engine.run();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}