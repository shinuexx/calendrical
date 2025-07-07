#include "calendars/samaritan_calendar.hpp"

#include <vector>
#include "time_and_astronomy.hpp"

namespace calendrical::calendars {
    using namespace calendrical::time_and_astronomy;

    samaritan_calendar::samaritan_calendar() {
        m_julian = std::make_unique<julian_calendar>();
        m_epoch = m_julian->to_fixed({-1639, month::march, 15});
    }

    moment_t samaritan_calendar::noon(const fixed_date_t& date)
    {
        return midday(date, samaritan_location);
    }

    fixed_date_t samaritan_calendar::new_moon_after(const moment_t& t)
    {
        const auto a = apparent_from_universal(new_moon_on_or_after(t), samaritan_location) - 0.5;
        return static_cast<int64_t>(std::ceil(a));
    }

    fixed_date_t samaritan_calendar::new_moon_at_or_before(const moment_t& t)
    {
        const auto a = apparent_from_universal(new_moon_before(t), samaritan_location) - 0.5;
        return static_cast<int64_t>(std::ceil(a));
    }
    fixed_date_t samaritan_calendar::new_year_on_or_before(const fixed_date_t& date) const
    {
        const auto gyear = gregorian_calendar::standard.get_year(date);
        std::vector<fixed_date_t> dates = m_julian_provider.in_gregorian(month::march, 11, gyear - 1);
        std::ranges::copy(m_julian_provider.in_gregorian(month::march, 11, gyear), std::back_inserter(dates));
        dates.push_back(date.add(1));
        dates.push_back(fixed_date_t::end_of_time);
        std::ranges::sort(dates);
        const auto begin = dates.begin();
        const auto n = last_passing([&](const int64_t& i) {
            return new_moon_after(noon(*std::next(begin, i))) <= date;
            }, 0);
        return new_moon_after(noon(*std::next(begin, n)));
    }
    int32_t samaritan_calendar::get_year(const fixed_date_t& date) const
    {
        return to_date(date).year;
    }
    simple_date samaritan_calendar::to_date(const fixed_date_t& date) const
    {
        const auto moon = new_moon_at_or_before(noon(date));
        const auto new_year = new_year_on_or_before(moon);
        auto month = std::round(static_cast<double>(moon - new_year) / 29.5) + 1;
        auto year = std::round(static_cast<double>(new_year - m_epoch) / 365.25) + std::ceil((month - 5) / 8.0);
        const auto day = date - moon + 1;
        return {
            static_cast<int32_t>(year),
            static_cast<int32_t>(month),
            static_cast<int32_t>(day)
        };
    }
    fixed_date_t samaritan_calendar::to_fixed(const simple_date& date) const
    {
        auto [year, month, day] = date;
        const auto a = std::floor(static_cast<double>(m_epoch) + 50 + 365.25 * (year - std::ceil((month - 5.0) / 8)));
        const auto ny = new_year_on_or_before(static_cast<int64_t>(a));
        const auto nm = new_moon_at_or_before(static_cast<double>(ny) + 29.5 * (month - 1) + 15);
        return nm + day - 1;
    }
    int32_t samaritan_calendar::get_month(const fixed_date_t& date) const
    {
        return to_date(date).month;
    }
    int32_t samaritan_calendar::get_day(const fixed_date_t& date) const
    {
        return to_date(date).day;
    }
}