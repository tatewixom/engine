#include "Ground.h"
#include "Model.h"
#include "Shader.h"
#include "Spaces.h"
#include "Keyboard.h"
#include "Object.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <filesystem>

namespace Nuke
{
  Model torus{};
  Model alien{};
  Shader model_basic_loader{};
  Shader model_loader{};

  float deltaTime{};
  float lastFrame{};

  Ground::Ground(States &states, Engine &engine)
      : State{ states }, engine_{engine}
  {
    initialize();
  }

  Ground::~Ground()
  {
    clean();
  }

  void Ground::initialize()
  {
    std::string root{std::filesystem::current_path().string() + '/'};
    std::cout << "File path: " << root << '\n';

    model_basic_loader.initialize(root + "../shaders/model_basic_loading.vs", root + "../shaders/model_basic_loading.fs");
    model_loader.initialize(root + "../shaders/model_loading.vs", root + "../shaders/model_loading.fs");

    torus.initialize_t(root + "assets/models/torus/basic_torus_00.gltf", glm::vec3{20.f, 0.f, 0.f});
    alien.initialize_t(root + "../assets/models/alien/t7t_terapod.glb", glm::vec3{40.f, 0.f, 0.f});
  }

  void Ground::input()
  {
    Window &window{engine_.getWindow()};
    Keyboard::processWindowEscape(window);

    // delta time
    float currentFrame{static_cast<float>(glfwGetTime())};
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // moving camera
    Camera &camera{engine_.getCamera()};
    camera.speed = 20.f; // adjust accordingly

    if (Keyboard::isKeyPressed(window, GLFW_KEY_W))
      camera.move(Camera::FORWARD, deltaTime);
    if (Keyboard::isKeyPressed(window, GLFW_KEY_S))
      camera.move(Camera::BACKWARD, deltaTime);
    if (Keyboard::isKeyPressed(window, GLFW_KEY_A))
      camera.move(Camera::LEFT, deltaTime);
    if (Keyboard::isKeyPressed(window, GLFW_KEY_D))
      camera.move(Camera::RIGHT, deltaTime);
    if (Keyboard::isKeyPressed(window, GLFW_KEY_SPACE))
      camera.move(Camera::UP, deltaTime);
    if (Keyboard::isKeyPressed(window, GLFW_KEY_LEFT_SHIFT))
      camera.move(Camera::DOWN, deltaTime);

    // handling view mode
    Mouse &mouse{engine_.getMouse()};

    if (Keyboard::isKeyJustPressed(window, GLFW_KEY_0))
    {
      if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
        mouse.selectionMode();
      else
        mouse.viewMode();
    }
  }

  void Ground::clear()
  {
    // glClearColor(1.f, 1.f, 1.0f, 0.f); //white
    glClearColor(0.06f, 0.06f, 0.06f, 0.f); // signature gray
    // glClearColor(0.f, 0.f, 0.f, 0.f); //black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void Ground::update()
  {
    Window &window{engine_.getWindow()};
    Camera &camera{engine_.getCamera()};

    Spaces &spaces{engine_.getSpaces()};
    model_basic_loader.activate();
    model_basic_loader.set("view", spaces.view);
    model_basic_loader.set("projection", spaces.projection);

    model_loader.activate();
    model_loader.set("view", spaces.view);
    model_loader.set("projection", spaces.projection);
  }

  void Ground::render()
  {
    torus.draw(model_basic_loader);
    alien.draw(model_loader);

    glfwSwapBuffers(engine_.getWindow());
  }

  void Ground::clean()
  {
  }

  void Ground::loop()
  {
    input();
    update();

    clear();
    render();
  }
}