#include "Interface.h"
#include "Callback.h"
#include "Keyboard.h"
#include "Collection.h"
#include "Window.h"
#include "Camera.h"
#include "Shader.h"
#include "Mouse.h"

Interface::Element::Space Interface::Element::s_spaces{};

namespace Points
{
  Object::Dimension bd{ 700.f, 400.f };

  std::vector<float> backdrop
  {
     0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     bd.x, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     bd.x, bd.y, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
     0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     bd.x, bd.y, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
     0.0f, bd.y, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
  };

  Object::Dimension cb{ 500.f, 50.f };

  std::vector<float> controlBar
  {
     0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     cb.x, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     cb.x, cb.y, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
     0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     cb.x, cb.y, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
     0.0f, cb.y, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
  };
}

namespace
{
  Buffer backgroundBuffer;

  Shader shader{};

  Interface::Element background{ glm::vec2{ 0.0f }, Interface::Element::Dimension{ Points::bd.x,  Points::bd.y } };
  Interface::Element grabBar{ glm::vec2{ 0.0f }, Interface::Element::Dimension{ Points::bd.x,  Points::bd.y } };
}

Interface::Interface(State& state, Window& window, Camera& camera, Mouse& mouse)
  : IState{ state }
  , m_window{ window }
  , m_camera{ camera }
  , m_mouse{ mouse }
{
  initialize();
}

void Interface::initialize()
{
  backgroundBuffer.initialize(Points::backdrop); //initializing buffer
  background.setVAO(backgroundBuffer); //setting VAO's 
  grabBar.setVAO(backgroundBuffer);

  shader.initialize("interface.vs", "interface.fs");

  grabBar.scalar(glm::vec2{ 1.f, 0.1f });

  Window::Dimensions dimensions{ m_window.dimensions() };

  background.position(glm::vec2{ 0.0f , dimensions.height - background.dimensions().height() });
  grabBar.position(background, glm::vec2{ 0.0f, background.dimensions().height() - grabBar.dimensions().height() });
}

void Interface::input()
{
  if (Keyboard::isKeyJustPressed(m_window, GLFW_KEY_0))
  {
    if (glfwGetInputMode(m_window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
      m_mouse.selectionMode();
    else
      m_mouse.viewMode();
  }

  Window::Dimensions dimensions{ m_window.dimensions() };

  if (!m_mouse.isDisabled() && 
    m_mouse.isButtonPressed(GLFW_MOUSE_BUTTON_LEFT) &&
    grabBar.hitbox().isIntersecting(static_cast<float>(m_mouse.position().x), dimensions.height - static_cast<float>(m_mouse.position().y)))
  {
    //TODO: grabbar stopped working after setting up new mouse system, fix it

    static Mouse::Position direction{};
    direction = m_mouse.position() - m_mouse.lposition();

    static Mouse::Position lastMousePosition{};

    if (lastMousePosition != m_mouse.position())
    {
      background.position(glm::vec2{ background.position().x + direction.x, background.position().y - direction.y });
      grabBar.position(background, glm::vec2{ 0.0f, background.dimensions().height() - grabBar.dimensions().height() });
      //need to make a set of logic that has the window move in whatever direction the mouse moves in the same amount of units and doesn't lag behind 
    }

    lastMousePosition = m_mouse.position();
  }
}

void Interface::update()
{
  Element::update(m_window);
}

void Interface::render()
{
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  shader.activate();
  shader.set("color", glm::vec4{ 0.f, 0.f, 0.f, 0.5f });

  background.draw(shader);

  shader.activate();
  shader.set("color", glm::vec4{ 0.28f, 0.29f, 0.30f, 1.0f });

  grabBar.draw(shader);

  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
}

void Interface::loop()
{
  update();

  render();
}

void Interface::clean()
{

}

void Interface::Element::dimensions(const Dimension& dimensions)
{
  m_dimensions = dimensions;

  m_hitbox.area.x2 = m_dimensions.width();
  m_hitbox.area.y2 = m_dimensions.height();
}

void Interface::Element::draw(const Shader& shader) const
{
  s_spaces.model = glm::mat4{ 1.f };

  s_spaces.model = glm::translate(s_spaces.model, glm::vec3{ m_position.x, m_position.y, 0.f });
  s_spaces.model = glm::scale(s_spaces.model, m_dimensions.scalar);

  s_spaces.mvp = glm::mat4{ s_spaces.ortho * s_spaces.model };

  shader.activate();

  shader.set("mvp", s_spaces.mvp);

  glBindVertexArray(m_VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6); // 36 is the amount of vertices 
}

void Interface::Element::position(const glm::vec2& position)
{
  m_position = position;

  m_hitbox.area = Hitbox::Area{ m_position.x, m_position.y, m_position.x + m_dimensions.width(), m_position.y + m_dimensions.height() };
}

void Interface::Element::position(Element& element, const glm::vec2& relativePosition)
{
  m_position.x = element.position().x + relativePosition.x;
  m_position.y = element.position().y + relativePosition.y;

  m_hitbox.area = Hitbox::Area{ m_position.x, m_position.y, m_position.x + m_dimensions.width(), m_position.y + m_dimensions.height() };
}

void Interface::Element::center(Element& element)
{
  position(glm::vec2{ element.position().x + ((element.dimensions().width() / 2) * element.scalar().x) - ((dimensions().width() / 2) * m_dimensions.scalar.x),
    element.position().y + ((element.dimensions().height() / 2) * element.scalar().y) - ((dimensions().height() / 2) * m_dimensions.scalar.y) });
}

void Interface::Element::update(Window& window)
{
  Window::Dimensions dimensions{ window.dimensions() };

  s_spaces.ortho = glm::ortho(0.0f, static_cast<float>(dimensions.width), 0.0f, static_cast<float>(dimensions.height), -1.0f, 1.0f);
}

bool Interface::Element::Hitbox::isIntersecting(const glm::vec2& point) const
{
  return (point.x > area.x1 && point.x < area.x2) && (point.y > area.y1 && point.y < area.y2); 
}

bool Interface::Element::Hitbox::isIntersecting(const float x, const float y) const
{
  return (x > area.x1 && x < area.x2) && (y > area.y1 && y < area.y2);
}

bool Interface::Element::Hitbox::isIntersecting(const Element& element)
{
  return false;
}
