#include "Collection.h"
#include "Random.h"
#include "Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm.hpp>
#include <gtx/norm.hpp>

#include <algorithm>
#include <iostream>

Collection::Collection(int count, const Object& object)
  : m_object{ object }
{
  if (count != 0 && count > 0)
  {
    float extra{ 1.5f };
    m_container.reserve(static_cast<std::size_t>(count * extra));

    add(count);
  }
  else
    std::cerr << "ERROR::COLLECTION.CPP::COLLECTION::INVALID_AMOUNT_OF_OBJECTS\n";
}

void Collection::add()
{
  m_container.push_back(createObject());
}

void Collection::add(int amount)
{
  for (std::size_t i{ 0 }; i < amount; ++i)
  {
    m_container.push_back(createObject());
  }
}

void Collection::sort(const Camera& camera)
{
  std::sort(m_container.begin(), m_container.end(), [&](Object& obj1, Object& obj2)
    {
      float d1{ glm::length2(camera.position - obj1.position()) };
      float d2{ glm::length2(camera.position - obj2.position()) };

      return d1 > d2;
    });
}

glm::vec3 Collection::getRandomPosition(int min, int max)
{
  return glm::vec3{ static_cast<float>(Random::get(min * 1000, max * 1000)) / 1000.0f, static_cast<float>(Random::get(min * 1000, max * 1000)) / 1000.0f, static_cast<float>(Random::get(min * 1000, max * 1000)) / 1000.0f };
}

void Collection::draw(const Shader& shader)
{
  for (auto& object : m_container)
    object.draw(shader);
}

void Collection::object(const Object& object)
{
  m_object = object;

  for (auto& obj : m_container)
    obj = m_object;
}

Object Collection::createObject()
{
  Object object{ m_object };
  object.position(getRandomPosition(-10, 10));
  return object;
}
