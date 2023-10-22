#include "calendars/gregorian_calendar.hpp"

namespace calendrical::calendars {
    const gregorian_calendar gregorian_calendar::standard{};

    month gregorian_calendar::get_month(const fixed_date_t& date, int32_t year) const {
        auto prior_days = date - get_new_year(year);
        auto correction = date < to_fixed({ year, month::march, 1 }) ? 0 : (is_leap_year(year) ? 1 : 2);
        return static_cast<month>(floor_div(12 * (prior_days + correction) + 373, 367));
    }

    fixed_date_t gregorian_calendar::get_new_year(int32_t year) const {
        return to_fixed({ year, month::january, 1 });
    }
    int32_t gregorian_calendar::get_year(const fixed_date_t& date) const {
        auto d0 = date - m_epoch;
        auto n400 = floor_div(d0, 146097);
        auto d1 = mod(d0, 146097);
        auto n100 = floor_div(d1, 36524);
        auto d2 = mod(d1, 36524);
        auto n4 = floor_div(d2, 1461);
        auto d3 = mod(d2, 1461);
        auto n1 = floor_div(d3, 365);
        auto year = 400 * n400 + 100 * n100 + n4 * 4 + n1;
        if ((n100 == 4) || (n1 == 4))
            return static_cast<int32_t>(year);
        else
            return static_cast<int32_t>(year + 1);
    }


    gregorian_date gregorian_calendar::to_date(const fixed_date_t& date) const {
        auto year = get_year(date);
        auto month = get_month(date, year);
        auto day = date - to_fixed({ year, month, 1 }) + 1;
        return { year, month, static_cast<int32_t>(day) };
    }
    fixed_date_t gregorian_calendar::to_fixed(const gregorian_date& date) const {
        const auto& [year, month, day] = date.tie();
        auto m = static_cast<int64_t>(month);
        int64_t y1 = year - 1LL;
        return m_epoch - 1
            + 365 * y1
            + floor_div(y1, 4)
            - floor_div(y1, 100)
            + floor_div(y1, 400)
            + floor_div(367 * m - 362, 12)
            + (month <= calendrical::month::february ? 0 : (is_leap_year(year) ? -1 : -2))
            + day;
    }

    month gregorian_calendar::get_month(const fixed_date_t& date) const {
        auto year = get_year(date);
        return get_month(date, year);
    }

    int32_t gregorian_calendar::get_day(const fixed_date_t& date) const {
        return to_date(date).day;
    }

    bool gregorian_calendar::is_leap_year(int32_t year) const {
        static constexpr std::array<int64_t, 3> non_leap_100_years = { 100, 200, 300 };
        return (0 == mod(year, 4LL)) && !in(non_leap_100_years, mod(year, 400LL));
    }

    fixed_date_t gregorian_calendar::get_year_end(int32_t year) const {
        return to_fixed({ year, month::december, 31 });
    }
    range<fixed_date_t> gregorian_calendar::get_year_range(int32_t year) const {
        return { range_type::left_closed, get_new_year(year), get_new_year(year + 1) };
    }

    int32_t gregorian_calendar::get_remaining_days(const gregorian_date& date) const {
        return static_cast<int32_t>(to_fixed({ date.year, month::december, 31 }) - to_fixed(date));
    }

    fixed_date_t gregorian_calendar::nth_weekday(int64_t n, weekday k, const gregorian_date& date) const {
        if (n > 0)
            return 7 * n + k.before(to_fixed(date));
        else if (n < 0)
            return 7 * n + k.after(to_fixed(date));
        else
            throw std::range_error("n == 0");
    }

    fixed_date_t gregorian_calendar::first_weekday(weekday k, const gregorian_date& date) const {
        return nth_weekday(1, k, date);
    }
    fixed_date_t gregorian_calendar::last_weekday(weekday k, const gregorian_date& date) const {
        return nth_weekday(-1, k, date);
    }

    std::list<moment_t> gregorian_calendar::cycle_in_gregorian(const angle_t& season, int year, double L, const moment_t& start) const {
        auto year_range = get_year_range(year).cast<double>();
        auto pos = season / 360_deg * L;
        auto D = pos - mod(start, L);
        auto t = positions_in_range(pos, L, D, year_range);
        auto moments = std::list<moment_t>();
        std::transform(t.begin(), t.end(), std::back_inserter(moments), [](const auto& x) { return moment_t{ x }; });
        return moments;
    }
    std::list<rational_moment_t> gregorian_calendar::cycle_in_gregorian(const angle_t& season, int year, const rational& L, const rational_moment_t& start) const
    {
        auto year_range = get_year_range(year).cast<int64_t>().cast<rational>();
        rational pos = season / 360_deg * L;
        auto D = pos - mod(start, L);
        auto t = positions_in_range<rational>(pos, L, D, year_range);
        auto moments = std::list<rational_moment_t>();
        std::transform(t.begin(), t.end(), std::back_inserter(moments), [](const auto& x) { return rational_moment_t{ x }; });
        return moments;
    }
}