#include <string_view>
#include "weekday.hpp"
#include "fixed_date.hpp"
#include "helpers.hpp"

namespace calendrical {
    fixed_date_t weekday::on_or_before(const fixed_date_t& date) const
    {
        return date - fixed_date_t{ date - m_v }.get_weekday().m_v;
    }
    fixed_date_t weekday::on_or_after(const fixed_date_t& date) const
    {
        return on_or_before(date + 6);
    }
    fixed_date_t weekday::nearest(const fixed_date_t& date) const
    {
        return on_or_before(date + 3);
    }
    fixed_date_t weekday::before(const fixed_date_t& date) const
    {
        return on_or_before(date - 1);
    }
    fixed_date_t weekday::after(const fixed_date_t& date) const
    {
        return on_or_before(date + 7);
    }
    void shift_days(std::vector<weekday>& l, int64_t D)
    {
        std::transform(l.begin(), l.end(), l.begin(), [D](const weekday& wd)
            { return static_cast<int32_t>(mod(static_cast<int32_t>(wd) + D, 7)); });
    }
    std::vector<weekday> shift_days(const std::vector<weekday>& l, int64_t D)
    {
        std::vector<weekday> r{ l };
        shift_days(r, D);
        return r;
    }
    std::string weekday::to_string() const
    {
        using namespace std::string_view_literals;
        constexpr std::array<std::string_view, 7> names{ "sunday"sv, "monday"sv, "tuesday"sv, "wednesday"sv, "thursday"sv, "friday"sv, "saturday"sv };
        if (valid())
        {
            return std::string{ names[m_v] };
        }
        else
        {
            return "weekday{" + std::to_string(m_v) + "}";
        }
    }
}

std::ostream& operator<<(std::ostream& out, const calendrical::weekday& wd) {
    return out << wd.to_string();
}