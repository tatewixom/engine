#include "Phase.h"

#include <iostream>
#include <chrono>

namespace Nuke
{
  namespace Phase
  {
    std::ostream &operator<<(std::ostream &out, const Duration &duration)
    {
      {
        out << duration.years << ' '
            << duration.months << ' '
            << duration.days << ' '
            << duration.hours << ' '
            << duration.minutes << ' '
            << duration.seconds;

        return out;
      }
    }

    Interval::Interval(std::chrono::year_month_day date, std::chrono::hours hour, std::chrono::minutes minute)
        : date_{date}, time_{hour, minute}
    {
      recomp();
    }

    void Interval::setDate(std::chrono::year_month_day date, std::chrono::hours hour, std::chrono::minutes minute)
    {
      date_ = date;
      time_.hour = hour;
      time_.minute = minute;
      recomp();
    }

    void Interval::recomp()
    {
      auto now{std::chrono::system_clock::now()};

      std::chrono::zoned_time local_now{std::chrono::current_zone(), now};

      std::chrono::local_time<std::chrono::seconds> end_date{std::chrono::local_days{date_} + time_.hour + time_.minute};

      std::chrono::zoned_time local_end_date{std::chrono::current_zone(), end_date};

      auto diff{local_end_date.get_local_time() - local_now.get_local_time()};

      auto y = duration_cast<std::chrono::years>(diff);
      auto m = duration_cast<std::chrono::months>(diff - y);
      auto d = duration_cast<std::chrono::days>(diff - y - m);
      auto h = duration_cast<std::chrono::hours>(diff - y - m - d);
      auto min = duration_cast<std::chrono::minutes>(diff - y - m - d - h);
      auto s = duration_cast<std::chrono::seconds>(diff - y - m - d - h - min);

      duration_.years = y.count();
      duration_.months = m.count();
      duration_.days = d.count();
      duration_.hours = h.count();
      duration_.minutes = min.count();
      duration_.seconds = static_cast<int>(s.count());
    }
  }
}