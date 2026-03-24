#pragma once

#include <vector>
#include <array>
#include <cstdint>

namespace Nuke
{
  template <typename T>
  class Revolver
  {
  public:
    void unload();
    void load();
    void spin();
    void shoot();

  private:
    std::vector<T> bullets_{};
  };

  template <typename Key, typename Value>
  class Itemizer
  {
  public:
    Value& operator[](Key index) noexcept
    {
      if ((index <= Key::none) || (index >= Key::max))
        return items_[static_cast<int>(Key::none)];

      return items_[static_cast<int>(index)];
    }

    const Value& operator[](Key index) const noexcept
    {
      if ((index <= Key::none) || (index >= Key::max))
        return items_[static_cast<int>(Key::none)];

      return items_[static_cast<int>(index)];
    }

    inline auto begin() noexcept { return items_.begin(); }
    inline auto end() noexcept { return items_.end(); }

    inline const auto begin() const noexcept { return items_.begin(); }
    inline const auto end() const noexcept { return items_.end(); }
  private:
    std::array<Value, static_cast<int>(Key::max)> items_{};
  };
}