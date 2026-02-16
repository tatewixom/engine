#pragma once

#include <deque>
#include <memory>
#include <type_traits>

namespace Nuke
{
  class State;

  class IState
  {
  public:
    explicit IState(State& state)
      : state_{ state }
    { }

    virtual ~IState() = default;

    virtual void loop() = 0;

  protected:
    State& state_;
  };

  class State
  {
  public:
    State() = default;

    //base functions
    void loop();

    template <typename S, typename... Args>
    void push(Args&&... args)
    {
      static_assert(std::is_base_of_v<IState, S>);
      states_.emplace_back(std::make_unique<S>(std::forward<Args>(args)...));
    }

    void pop();

    //specialty functions
    void loopBelow();
  private:
    std::deque<std::unique_ptr<IState>> states_{};
  };

  class Temporary final : public IState
  {
  public:
    Temporary(State& state)
      : IState{ state }
    { }

    ~Temporary() override = default;

    void loop() override {}
  };

  /*
  Example derived class:

  class Example final : public IState
  {
  public:
    Example(State& state)
      : IState{ state }
    {
      initialize();
    }

    ~Example() override = default;

    void initialize() override {}
    void update() override {}
    void render() override {}
    void clean() override {}
  };
  */
}

