#pragma once

#include <iostream>
#include <chrono>
#include <ratio>
#include <type_traits>

namespace Nuke
{
  namespace Phase
  {
    struct Time
    {
      using Hour = std::chrono::hours;
      using Minute = std::chrono::minutes;

      Hour hour{};
      Minute minute{};
    };

    struct Duration
    {
      int years{};
      int months{};
      int days{};
      int hours{};
      int minutes{};
      int seconds{};
    };

    std::ostream &operator<<(std::ostream &out, const Duration &duration);

    class Interval
    {
    public:
      Interval() = default;

      Interval(std::chrono::year_month_day date, std::chrono::hours hour, std::chrono::minutes minute);

      std::chrono::year_month_day date() const noexcept(std::is_nothrow_copy_constructible_v<std::chrono::year_month_day>) { return date_; }
      Time time() const noexcept(std::is_nothrow_copy_constructible_v<std::chrono::hours> &&
                                std::is_nothrow_copy_constructible_v<std::chrono::minutes>) { return time_; }
      constexpr Duration dur() const noexcept { return duration_; }
      void setDate(std::chrono::year_month_day date, std::chrono::hours hour, std::chrono::minutes minute);
      void recomp();

    private:
      using Date = std::chrono::year_month_day;

    private:
      Date date_{};
      Time time_{};
      Duration duration_{};
    };

    class Timer
    {
    public:
      void reset() noexcept { beg_ = Steady_clock::now(); }

      double elapsed() const noexcept { return std::chrono::duration_cast<Second>(Steady_clock::now() - beg_).count(); }

    private:
      using Steady_clock = std::chrono::steady_clock;
      using Second = std::chrono::duration<double, std::ratio<1>>;

    private:
      std::chrono::time_point<Steady_clock> beg_{Steady_clock::now()};
    };
  }
}
