#include "Ground.h"
#include "Model.h"
#include "Shader.h"
#include "Spaces.h"
#include "Keyboard.h"
#include "Object.h"
#include "Callback.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <filesystem>

namespace Nuke
{
  Model torus{};
  Model alien{};
  Model tigerhumvee{};

  Shader model_basic_loader{};
  Shader model_loader{};
  Shader model_light{};
  Shader model_reflector{};

  float deltaTime{};
  float lastFrame{};

  Ground::Ground(Engine& engine)
    : engine_{ engine }
  {
    initialize();
  }

  Ground::~Ground()
  {
    clean();
  }

  void Ground::initialize()
  {
    model_basic_loader.initialize("shaders/model_basic_loading.vs", "shaders/model_basic_loading.fs");
    model_loader.initialize("shaders/model_loading.vs", "shaders/model_loading.fs");
    model_light.initialize("shaders/model_light.vs", "shaders/model_light.fs");
    model_reflector.initialize("shaders/model_reflector.vs", "shaders/model_reflector.fs");

    std::string root{ std::filesystem::current_path().string() + '/' };

    torus.initialize_t("assets/models/torus/basic_torus_00.gltf", glm::vec3{ 0.f, 0.f, 0.f });
    tigerhumvee.initialize_t("assets/models/vehicles/tigerhumvee1k.glb", glm::vec3{ -20.f, 0.f, 0.f });
    alien.initialize_t("assets/models/alien/t7t_terapod.glb", glm::vec3{ 20.f, 0.f, 0.f });

    //shader constants
    model_reflector.activate();
    model_reflector.set("u_light.ambient", glm::vec3{ 0.2f });
    model_reflector.set("u_light.diffuse", glm::vec3{ 2.0f });
    model_reflector.set("u_light.specular", glm::vec3{ 1.0f });
    model_reflector.set("u_light.constant", 1.f);
    model_reflector.set("u_light.linear", 0.00022f);
    model_reflector.set("u_light.quadratic", 0.000019f);
  }

  void Ground::input()
  {
    // delta time
    float currentFrame{ static_cast<float>(glfwGetTime()) };
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // moving camera
    Camera& camera{ engine_.getCamera() };
    camera.speed = 20.f; // adjust accordingly

    //keyboard input
    Keyboard& keyboard{ engine_.getKeyboard() };

    if (keyboard.now(Keyboard::Keys::ESCAPE))
      glfwSetWindowShouldClose(engine_.getWindow(), true);
    if (keyboard.is(Keyboard::Keys::W))
      camera.move(Camera::FORWARD, deltaTime);
    if (keyboard.is(Keyboard::Keys::S))
      camera.move(Camera::BACKWARD, deltaTime);
    if (keyboard.is(Keyboard::Keys::A))
      camera.move(Camera::LEFT, deltaTime);
    if (keyboard.is(Keyboard::Keys::D))
      camera.move(Camera::RIGHT, deltaTime);
    if (keyboard.is(Keyboard::Keys::SPACE))
      camera.move(Camera::UP, deltaTime);
    if (keyboard.is(Keyboard::Keys::LEFT_SHIFT))
      camera.move(Camera::DOWN, deltaTime);

    // handling view mode
    Mouse& mouse{ engine_.getMouse() };

    if (keyboard.now(Keyboard::Keys::ZERO))
    {
      if (glfwGetInputMode(engine_.getWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
        mouse.selectionMode();
      else
        mouse.viewMode();
    }

    //moving light source
    if (keyboard.is(Keyboard::Keys::UP))
      torus.move(Model::Movement::BACKWARD, deltaTime);
    if (keyboard.is(Keyboard::Keys::DOWN))
      torus.move(Model::Movement::FORWARD, deltaTime);
    if (keyboard.is(Keyboard::Keys::LEFT))
      torus.move(Model::Movement::LEFT, deltaTime);
    if (keyboard.is(Keyboard::Keys::RIGHT))
      torus.move(Model::Movement::RIGHT, deltaTime);
    if (keyboard.is(Keyboard::Keys::R))
      torus.move(Model::Movement::UP, deltaTime);
    if (keyboard.is(Keyboard::Keys::F))
      torus.move(Model::Movement::DOWN, deltaTime);
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
    Window& window{ engine_.getWindow() };
    Camera& camera{ engine_.getCamera() };
    Spaces& spaces{ engine_.getSpaces() };

    model_basic_loader.activate();
    model_basic_loader.set("view", spaces.view);
    model_basic_loader.set("projection", spaces.projection);

    model_loader.activate();
    model_loader.set("view", spaces.view);
    model_loader.set("projection", spaces.projection);

    model_light.activate();
    model_light.set("view", spaces.view);
    model_light.set("projection", spaces.projection);

    model_reflector.activate();
    model_reflector.set("view", spaces.view);
    model_reflector.set("projection", spaces.projection);
    model_reflector.set("u_viewPos", camera.position);
    model_reflector.set("u_light.position", camera.position);

  }

  void Ground::render()
  {
    torus.draw(model_light);
    alien.draw(model_reflector);
    tigerhumvee.draw(model_reflector);

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