#pragma once

#include "State.h"
#include "Buffer.h"
#include "Engine.h"

#include <gtc/matrix_transform.hpp>

namespace Nuke
{
  struct Space
  {
    glm::mat4 model{};
    glm::mat4 ortho{};
    glm::mat4 mvp{};
  };

  class Element
  {
  public:
    Element() = default;

    Element(glm::vec2 position, const Dimension &dimensions)
        : position_{ position }
        , dimensions_{ dimensions }
        , hitbox_{ Hitbox::Area{ position.x, position.y, position.x + dimensions.width(), position.y + dimensions.height() } }
    { }

    void setVAO(const Buffer &buffer) { VAO_ = buffer.getVAO(); }

    Dimension dimensions() const noexcept { return dimensions_; }
    void dimensions(const Dimension &dimensions);

    glm::vec3 &scalar() { return dimensions_.scalar; }
    void scalar(const glm::vec2 &scalar)
    {
      dimensions_.scalar.x = scalar.x;
      dimensions_.scalar.y = scalar.y;
    }

    void draw(const Shader &shader) const;

    // orthographic functions
    glm::vec2 position() const noexcept { return position_; }
    void position(const glm::vec2 &position);
    void position(Element &element, const glm::vec2 &relativePosition);
    void center(Element &object);

    Hitbox hitbox() const { return hitbox_; }
    void hitbox(const Hitbox &hitbox) { hitbox_ = hitbox; }

  public:
    static void update(Window &window);

  private:
    glm::vec2 position_{};
    GLuint VAO_{};
    Dimension dimensions_{};
    Hitbox hitbox_{};

  private:
    static Space s_spaces;
  };

  class Dimension
  {
  public:
    Dimension() = default;

    Dimension(float x, float y)
        : x_{ x }
        , y_{ y }
    { }

    float width() const { return scalar.x * x_; }
    float height() const { return scalar.y * y_; }

    void width(const float width) { x_ = width; }
    void height(const float height) { y_ = height; }

    glm::vec3 scalar{1.f};

  private:
    float x_{};
    float y_{};
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

    Hitbox(const Area &hitbox)
        : area{ hitbox }
    { }

    Hitbox(Element &element)
        : area{ Area{ element.position().x, element.position().y, element.position().x + element.dimensions().width(), element.position().y + element.dimensions().height() } } 
    { }

    bool isIntersecting(const glm::vec2 &point) const;
    bool isIntersecting(const float x, const float y) const;
    bool isIntersecting(const Element &element);

    // void target(const Mouse& mouse); //make a function for when the mouse is pressed, the element is targetted on the mouse position

    Area area{};
  };

  class Interface : public IState
  {
  public:
    Interface(State& state, Engine& engine);

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
    Engine& engine;
  };
}