#include "calendars/samaritan_calendar.hpp"
#include "time_and_astronomy.hpp"

namespace calendrical::calendars {
    
    using namespace calendrical::time_and_astronomy;

    moment_t samaritan_calendar::noon(const fixed_date_t& date)
    {
        return midday(date, samaritan_location);
    }

    fixed_date_t samaritan_calendar::new_moon_after(const moment_t& t)
    {
        auto a = apparent_from_universal(new_moon_on_or_after(t), samaritan_location) - 0.5;
        return static_cast<int64_t>(std::ceil(a));
    }

    fixed_date_t samaritan_calendar::new_moon_at_or_before(const moment_t& t)
    {
        auto a = apparent_from_universal(new_moon_before(t), samaritan_location) - 0.5;
        return static_cast<int64_t>(std::ceil(a));
    }
    fixed_date_t samaritan_calendar::new_year_on_or_before(const fixed_date_t& date) const
    {
        auto gyear = gregorian_calendar::standard.get_year(date);
        std::list<fixed_date_t> dates = m_julian_provider.in_gregorian(month::march, 11, gyear - 1);
        join_in_place(dates, m_julian_provider.in_gregorian(month::march, 11, gyear));
        dates.push_back(date.add(1));
        dates.push_back(fixed_date_t::end_of_time);
        auto begin = dates.begin();
        auto n = last_passing([this, begin, date](int64_t i) {
            return new_moon_after(noon(*std::next(begin, i)) <= date);
            }, 0);
        return new_moon_after(noon(*std::next(begin, n)));
    }
    int32_t samaritan_calendar::get_year(const fixed_date_t& date) const
    {
        return to_date(date).year;
    }
    simple_date samaritan_calendar::to_date(const fixed_date_t& date) const
    {
        auto moon = new_moon_at_or_before(noon(date));
        auto new_year = new_year_on_or_before(moon);
        auto month = std::round((moon - new_year) / 29.5) + 1;
        auto year = std::round((new_year - ms_epoch) / 365.25) + floor_div(month - 5, 8.0);
        auto day = date - moon + 1;
        return {
            static_cast<int32_t>(year),
            static_cast<int32_t>(month),
            static_cast<int32_t>(day)
        };
    }
    fixed_date_t samaritan_calendar::to_fixed(const simple_date& date) const
    {
        auto [year, month, day] = date.tie();
        auto a = std::floor(ms_epoch + 50 + 365.25 * (year - std::ceil((month - 5.0) / 8)));
        auto ny = new_year_on_or_before(static_cast<int64_t>(a));
        auto nm = new_moon_at_or_before(ny + 29.5 * (month - 1) + 15);
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