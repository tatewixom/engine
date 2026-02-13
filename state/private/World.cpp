#include "World.h"
#include "Keyboard.h"
#include "Window.h"
#include "Camera.h"
#include "Random.h"
#include "Mouse.h"
#include "Texture.h"
#include "Model.h"
#include "Spaces.h"

namespace
{
  //input processing
  float deltaTime{};
  float lastFrame{};

  Buffer buffer;

  Shader basic{};
  Shader color{};
  Shader light{};
  Shader test{};
  Shader instancing_spotlight{};
  Shader model_loader{};
  Shader model_basic_loader{};

  Model blender_monkey{};
  Model blender_cube{};
  Model blender_torus{};
  Model blender_metacubemonkey{};
  Model sketchfab_backpack{};

  Ditto ditto{};

  Collection cubes
  {
    10,
    Object
    {
      buffer, 
      glm::vec3{ 0.0f },
      Object::Material{ glm::vec3{ 1.0f, 0.5f, 0.31f }, glm::vec3{ 1.0f, 0.5f, 0.31f }, glm::vec3{ 0.5f, 0.5f, 0.5f }, 32.f }
    }
  };

  Object lights{ buffer, glm::vec3{ 0.0f }, Object::Material{} };

  Texture container{};
  Texture awesomeFace{};
  Texture steelbox{};
  Texture steelbox_specular{};

  //glm::vec3 randomColor{ Random::get(10, 90), Random::get(10, 90), Random::get(10, 90), };
}

World::World(State& state, Window& window, Camera& camera, Mouse& mouse)
  : IState{ state }
  , m_window{ window }
  , m_camera{ camera }
  , m_mouse{ mouse }
{
  initialize();
}

void World::initialize()
{
  //stbi_set_flip_vertically_on_load(true);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  //buffer init
  //buffer.initialize(Points::Cube::vertices, Points::Cube::indices);

  //cubes init
  //for (auto& c : cubes)
    //c.initialize(buffer);
  
  std::vector<Vertex> vertex
  {
    Vertex{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
    Vertex{ { 0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
    Vertex{ { 0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, -1.0f } },
    Vertex{ { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f } },

    Vertex{ { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
    Vertex{ { 0.5f, -0.5f, 0.5f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
    Vertex{ { 0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
    Vertex{ { -0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },

    Vertex{ { -0.5f, 0.5f, 0.5f }, { 1.0f, 0.0f }, { -1.0f, 0.0f, 0.0f } },
    Vertex{ { -0.5f, 0.5f, -0.5f }, { 1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f } },
    Vertex{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f }, { -1.0f, 0.0f, 0.0f } },
    Vertex{ { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f } },

    Vertex{ { 0.5f, 0.5f, 0.5f }, { 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
    Vertex{ { 0.5f, 0.5f, -0.5f }, { 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
    Vertex{ { 0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
    Vertex{ { 0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },

    Vertex{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f }, { 0.0f, -1.0f, 0.0f } },
    Vertex{ { 0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f }, { 0.0f, -1.0f, 0.0f } },
    Vertex{ { 0.5f, -0.5f, 0.5f }, { 1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f } },
    Vertex{ { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f }, { 0.0f, -1.0f, 0.0f } },

    Vertex{ { -0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },
    Vertex{ { 0.5f, 0.5f, -0.5f }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },
    Vertex{ { 0.5f, 0.5f, 0.5f }, { 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
    Vertex{ { -0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
  };

  std::vector<unsigned int> indices
  {
    0, 1, 2,
    2, 3, 0,

    4, 5, 6,
    6, 7, 4,

    8, 9, 10,
    10, 11, 8,

    12, 13, 14,
    14, 15, 12,

    16, 17, 18,
    18, 19, 16,

    20, 21, 22,
    22, 23, 20,
  };

  //ditto init
  ditto.initialize
  (
    vertex,
    indices,
    std::vector<Attribute>
    {
      Attribute{ 0u, 3, sizeof(Vertex), offsetof(Vertex, Vertex::position) },
      Attribute{ 2u, 2, sizeof(Vertex), offsetof(Vertex, Vertex::texture) },
      Attribute{ 3u, 3, sizeof(Vertex), offsetof(Vertex, Vertex::normal) }
    }
  );

  blender_monkey.initialize_t("assets/monkey/glb/basic_monkey_00.glb", glm::vec3{ 0.f, 0.f, 0.f });
  blender_torus.initialize_t("assets/torus/basic_torus_00.gltf", glm::vec3{ 20.f, 0.f, 0.f });
  blender_cube.initialize_t("assets/vehicle/tiger_russian_military_vehicle.glb", glm::vec3{ 40.f, 0.f, 0.f });
  blender_metacubemonkey.initialize_t("assets/alien/t7t_terapod.glb", glm::vec3{ -20.f, 0.f, 0.f });
  sketchfab_backpack.initialize_t("assets/backpack/backpack_01.glb", glm::vec3{ -40.f, 0.f, 0.f });
 
  //shader init
  basic.initialize("basic.vs", "basic.fs");
  color.initialize("color.vs", "color.fs");
  light.initialize("light.vs", "light.fs");
  test.initialize("test.vs", "test.fs");
  instancing_spotlight.initialize("instancing_spotlight.vs", "instancing_spotlight.fs");
  model_loader.initialize("model_loading.vs", "model_loading.fs");
  model_basic_loader.initialize("model_basic_loading.vs", "model_basic_loading.fs");

  ditto.setShader(instancing_spotlight);

  //texture init
  container.initialize("assets/container.jpg", Texture::Type::base);
  awesomeFace.initialize("assets/awesomeface.png", Texture::Type::base);
  steelbox.initialize("assets/container2.png", Texture::Type::base);
  steelbox_specular.initialize("assets/container2_specular.png", Texture::Type::base);

  //shader uniforms
  basic.activate();
  basic.set("texture1", container.use());
  basic.set("texture2", awesomeFace.use());
  basic.set("opacity", 0.2f);

  light.activate();
  light.set("color", glm::vec3{ 1.0, 1.0, 1.0 });

  color.activate();
  color.set("objectColor", glm::vec3{ 1.0f, 1.0f, 1.0f });
  
  color.set("material.diffuse", steelbox.use());
  color.set("material.specular", steelbox_specular.use());
  color.set("material.shininess", cubes.object().material().shininess);

  color.set("lightPoint.position", lights.position());
  color.set("lightPoint.ambient", glm::vec3{ 0.5f, 0.5f, 0.5f });
  color.set("lightPoint.diffuse", glm::vec3{ 0.5f, 0.5f, 0.5f });
  color.set("lightPoint.specular", glm::vec3{ 1.0f, 1.0f, 1.0f });
  color.set("lightPoint.constant", 1.0f);
  color.set("lightPoint.linear", 0.09f);
  color.set("lightPoint.quadratic", 0.032f);

  color.set("lightCaster.direction", glm::vec3{ -0.2f, -1.0f, -0.3f });
  color.set("lightCaster.ambient", glm::vec3{ 0.2f, 0.2f, 0.2f });
  color.set("lightCaster.diffuse", glm::vec3{ 0.5f, 0.5f, 0.5f });
  color.set("lightCaster.specular", glm::vec3{ 1.0f, 1.0f, 1.0f });

  instancing_spotlight.activate();
  instancing_spotlight.set("objectColor", glm::vec3{ 1.0f, 1.0f, 1.0f });

  instancing_spotlight.set("material.diffuse", steelbox.use());
  instancing_spotlight.set("material.specular", steelbox_specular.use());
  instancing_spotlight.set("material.shininess", cubes.object().material().shininess);

  instancing_spotlight.set("spotlight.position", lights.position());
  instancing_spotlight.set("spotlight.ambient", glm::vec3{ 0.1f, 0.1f, 0.1f });
  instancing_spotlight.set("spotlight.diffuse", glm::vec3{ 0.5f, 0.5f, 0.5f });
  instancing_spotlight.set("spotlight.specular", glm::vec3{ 1.0f, 1.0f, 1.0f });
  instancing_spotlight.set("spotlight.constant", 0.5f);
  instancing_spotlight.set("spotlight.linear", 0.09f);
  instancing_spotlight.set("spotlight.quadratic", 0.032f);
  instancing_spotlight.set("spotlight.direction", glm::vec3{ -0.2f, -1.0f, -0.3f });
  instancing_spotlight.set("spotlight.cutoff", glm::cos(glm::radians(35.0f)));
  instancing_spotlight.set("spotlight.outerCutoff", glm::cos(glm::radians(45.0f)));

  test.activate();
  test.set("texture1", steelbox.use());

  lights.scale(glm::vec3{ 0.2f });

  //instancing
  std::size_t amount{ 10 };
  std::vector<glm::mat4> matrices(amount);
  std::vector<Ditto::Motions> motions{};
  motions.reserve(amount);
  for (std::size_t i{ 0 }; i < amount; ++i)
  {
    motions.push_back(
    Ditto::Motions
    {
      glm::vec3{ Random::get(-10, 10), Random::get(-10, 10), Random::get(-10, 10) },
      glm::vec3{ 1.f },
      Ditto::Rotation{ glm::vec3{ Random::get(0, 10), Random::get(0, 10), Random::get(0, 10) }, static_cast<float>(Random::get(0, 36000)) / 100.f }
    });
  }

  ditto.sendInstances(matrices, motions, 4u);
}

void World::input()
{
  Keyboard::processWindowEscape(m_window);

  //delta time
  float currentFrame{ static_cast<float>(glfwGetTime()) };
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  //moving camera
  const float cameraSpeed = 20.f * deltaTime; // adjust accordingly
  if (Keyboard::isKeyPressed(m_window, GLFW_KEY_W))
    m_camera.position += cameraSpeed * m_camera.front;
  if (Keyboard::isKeyPressed(m_window, GLFW_KEY_S))
    m_camera.position -= cameraSpeed * m_camera.front;
  if (Keyboard::isKeyPressed(m_window, GLFW_KEY_A))
    m_camera.position -= glm::normalize(glm::cross(m_camera.front, m_camera.up)) * cameraSpeed;
  if (Keyboard::isKeyPressed(m_window, GLFW_KEY_D))
    m_camera.position += glm::normalize(glm::cross(m_camera.front, m_camera.up)) * cameraSpeed;
  if (Keyboard::isKeyPressed(m_window, GLFW_KEY_SPACE))
    m_camera.position += m_camera.up * cameraSpeed;
  if (Keyboard::isKeyPressed(m_window, GLFW_KEY_LEFT_SHIFT))
    m_camera.position -= m_camera.up * cameraSpeed;

  //cubes manipulation
  if (Keyboard::isKeyJustPressed(m_window, GLFW_KEY_G))
    cubes.add(5);
  if (Keyboard::isKeyJustPressed(m_window, GLFW_KEY_9))
    std::cout << cubes.size() << '\n';

  //moving light position
  const float objectSpeed{ 5.f * deltaTime };
  if (Keyboard::isKeyPressed(m_window, GLFW_KEY_UP))
    lights.position(lights.position() - glm::vec3{ 0.0f, 0.0f, 1.0f } * objectSpeed);
  if (Keyboard::isKeyPressed(m_window, GLFW_KEY_RIGHT))
    lights.position(lights.position() + glm::vec3{ 1.0f, 0.0f, 0.0f } * objectSpeed);
  if (Keyboard::isKeyPressed(m_window, GLFW_KEY_LEFT))
    lights.position(lights.position() - glm::vec3{ 1.0f, 0.0f, 0.0f } * objectSpeed);
  if (Keyboard::isKeyPressed(m_window, GLFW_KEY_DOWN))
    lights.position(lights.position() + glm::vec3{ 0.0f, 0.0f, 1.0f } * objectSpeed);
  if (Keyboard::isKeyPressed(m_window, GLFW_KEY_R))
    lights.position(lights.position() + glm::vec3{ 0.0f, 1.0f, 0.0f } * objectSpeed);
  if (Keyboard::isKeyPressed(m_window, GLFW_KEY_F))
    lights.position(lights.position() - glm::vec3{ 0.0f, 1.0f, 0.0f } * objectSpeed);

  //states
  //if (Keyboard::isKeyJustPressed(m_window, GLFW_KEY_1))
    //state_.push(std::make_unique<World>(state_, m_window, m_camera, m_mouse));
  if (Keyboard::isKeyJustPressed(m_window, GLFW_KEY_2))
    state_.pop();
  if (Keyboard::isKeyJustPressed(m_window, GLFW_KEY_3))
    state_.loopBelow();
}

void World::clear()
{
  //clearing screen and rendering
  glClearColor(1.f, 1.f, 1.0f, 0.f);
  //glClearColor(0.06f, 0.06f, 0.06f, 0.f); //signature gray
  //glClearColor(0.f, 0.f, 0.f, 0.f); //black
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void World::update()
{
  Object::update(m_camera, m_window);
  Ditto::update(m_camera, m_window);
  Spaces::update(m_camera, m_window);

  model_loader.activate();
  Spaces& spaces{ Spaces::getInstance() };
  model_loader.set("view", spaces.view);
  model_loader.set("projection", spaces.projection);

  model_basic_loader.activate();
  model_basic_loader.set("view", spaces.view);
  model_basic_loader.set("projection", spaces.projection);

  color.activate();
  //color.set("lightPoint.position", m_camera.position);
  //color.set("viewPos", m_camera.position);

  instancing_spotlight.activate();
  instancing_spotlight.set("spotlight.position", m_camera.position);
  instancing_spotlight.set("spotlight.direction", m_camera.front);
  instancing_spotlight.set("viewPos", m_camera.position);

  //color.set("objectColor", glm::vec3{sin(randomColor.x + time), sin(randomColor.y + time), sin(randomColor.z + time)});

  cubes.sort(m_camera);

  for (auto& cube : cubes)
  {
    cube.rotate(Object::Rotation{ glm::vec3{ cube.position().y * 2.f, cube.position().z * 3.f, cube.position().x * 9.f }, static_cast<float>(glfwGetTime() * glm::radians(100.f) * 50) });
  }
}

void World::render()
{
  //cubes.draw(color);
  //lights.draw(light);
  //ditto.draw();

  blender_monkey.draw(model_basic_loader);
  blender_torus.draw(model_basic_loader);
  blender_cube.draw(model_loader);
  blender_metacubemonkey.draw(model_loader);
  sketchfab_backpack.draw(model_loader);

  m_interface->input();
  m_interface->loop();

  //swaping buffers
  glfwSwapBuffers(m_window);
}

/*
  should start working on textures asap. 
*/

void World::loop()
{
  update();

  clear();
  render();
}

void World::clean()
{
  
}
