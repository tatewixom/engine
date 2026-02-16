#pragma once

#include "Object.h"

namespace Nuke
{
  class Camera;

  class Collection
  {
  public:
    Collection(int count, const Object &object);

    Object &operator[](int i) { return container_[i]; }

    void add();
    void add(int amount);

    void sort(const Camera &camera);

    glm::vec3 getRandomPosition(int min, int max);

    auto begin() { return container_.begin(); }
    auto end() { return container_.end(); }

    int size() const { return static_cast<int>(container_.size()); }

    void draw(const Shader &shader);

    // object manipulation
    void object(const Object &object);
    Object object() const { return object_; }

  private:
    Object createObject();

    Object object_{};
    std::vector<Object> container_{};
  };
}
