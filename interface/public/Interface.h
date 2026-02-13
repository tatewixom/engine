#pragma once

#include "State.h"
#include "Buffer.h"

#include <gtc/matrix_transform.hpp>

class Window;
class Camera;
class Object;
class Shader;
class Mouse;

class Interface : public IState
{
public:
  class Element
  {
  public:
    struct Space
    {
      glm::mat4 model{};
      glm::mat4 ortho{};
      glm::mat4 mvp{};
    };

    class Dimension
    {
    public:
      Dimension() = default;

      Dimension(float x, float y)
        : m_x{ x }
        , m_y{ y }
      { }

      float width() const { return scalar.x * m_x; }
      float height() const { return scalar.y * m_y; }

      void width(const float width) { m_x = width; }
      void height(const float height) { m_y = height; }

      glm::vec3 scalar{ 1.f };

    private:
      float m_x{};
      float m_y{};
    };

    struct Hitbox
    {
      struct Area
      {
        float x1{};
        float y1{};
        float x2{};
        float y2{};
      };

      Hitbox() = default;

      Hitbox(const Area& hitbox)
        : area{ hitbox }
      { }

      Hitbox(Element& element)
        : area{ Area{ element.m_position.x, element.m_position.y, element.m_position.x + element.m_dimensions.width(), element.m_position.y + element.m_dimensions.height() } }
      { }

      bool isIntersecting(const glm::vec2& point) const;
      bool isIntersecting(const float x, const float y) const;
      bool isIntersecting(const Element& element);

      //void target(const Mouse& mouse); //make a function for when the mouse is pressed, the element is targetted on the mouse position 

      Area area{};
    };

    Element() = default;

    Element(glm::vec2 position, const Dimension& dimensions)
      : m_position{ position }
      , m_dimensions{ dimensions }
      , m_hitbox{ Hitbox::Area{ m_position.x, m_position.y, m_position.x + m_dimensions.width(), m_position.y + m_dimensions.height() } }
    { }

    void setVAO(const Buffer& buffer) { m_VAO = buffer.getVAO(); }

    Dimension dimensions() const { return m_dimensions; }
    void dimensions(const Dimension& dimensions);

    glm::vec3& scalar() { return m_dimensions.scalar; }
    void scalar(const glm::vec2& scalar) { m_dimensions.scalar.x = scalar.x; m_dimensions.scalar.y = scalar.y; }

    void draw(const Shader& shader) const;

    //orthographic functions
    glm::vec2 position() const { return m_position; }
    void position(const glm::vec2& position);
    void position(Element& element, const glm::vec2& relativePosition);
    void center(Element& object);

    Hitbox hitbox() const { return m_hitbox; }
    void hitbox(const Hitbox& hitbox) { m_hitbox = hitbox; }

  public:
    static void update(Window& window);

  private:
    glm::vec2 m_position{};
    GLuint m_VAO{};
    Dimension m_dimensions{};
    Hitbox m_hitbox{};

  private:
    static Space s_spaces;
  };

  Interface(State& state, Window& window, Camera& camera, Mouse& mouse);

  ~Interface() override
  {
    clean();
  }

  void loop() override;

  void initialize();
  void input();
  void update();
  void render();
  void clean();

private:
  Window& m_window;
  Camera& m_camera;
  Mouse& m_mouse;
};

