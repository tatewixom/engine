#pragma once

#include <deque>
#include <memory>
#include <type_traits>

namespace Nuke
{
  class States;

  class State
  {
  public:
    explicit State(States& states)
      : state_{ states }
    { }

    virtual ~State() = default;

    virtual void loop() = 0;

  protected:
    States& state_;
  };

  class States
  {
  public:
    States() = default;

    //base functions
    void loop();

    template <typename S, typename... Args>
    void push(Args&&... args)
    {
      static_assert(std::is_base_of_v<State, S>);
      states_.emplace_back(std::make_unique<S>(std::forward<Args>(args)...));
    }

    void pop();

    //specialty functions
    void loopBelow();
  private:
    std::deque<std::unique_ptr<State>> states_{};
  };

  class Temporary final : public State
  {
  public:
    Temporary(States& states)
      : State{ states }
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

