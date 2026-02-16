#pragma once

#include "Object.h"

namespace Nuke
{
  class Camera;

  class Collection
  {
  public:
    Collection(int count, const Object &object);

    Object &operator[](int i) { return m_container[i]; }

    void add();
    void add(int amount);

    void sort(const Camera &camera);

    glm::vec3 getRandomPosition(int min, int max);

    auto begin() { return m_container.begin(); }
    auto end() { return m_container.end(); }

    int size() const { return static_cast<int>(m_container.size()); }

    void draw(const Shader &shader);

    // object manipulation
    void object(const Object &object);
    Object object() const { return m_object; }

  private:
    Object createObject();

    Object m_object{};
    std::vector<Object> m_container{};
  };
}
