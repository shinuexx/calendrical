#include "calendars/simple_calendar.hpp"
#include "primitives.hpp"

namespace calendrical::calendars{
    const simple_calendar egyptian_calendar { primitives::fixed_from_jd(1448638) };
    const simple_calendar armenian_calendar{ 201443 };

    simple_date simple_calendar::to_date(const fixed_date_t& date) const {
        const auto days = date - m_epoch;
        auto year = get_year(date);
        auto month = get_month(date);
        const auto day = days - 365 * (year - 1LL) - 30 * (month - 1LL) + 1;
        return {year, month, static_cast<int32_t>(day)};
    }
    fixed_date_t simple_calendar::to_fixed(const simple_date& date) const {
        auto& [year, month, day] = date;
        return m_epoch
        + 365 * (year - 1LL)
        + 30 * (month - 1LL)
        + day - 1;
    }

    fixed_date_t simple_calendar::get_new_year(int32_t year) const{
        return m_epoch + 365 * (year - 1LL);
    }
    int32_t simple_calendar::get_year(const fixed_date_t& date) const {
        const auto days = date - m_epoch;
        return static_cast<int32_t>(floor_div(days, 365) + 1);
    }

    int32_t simple_calendar::get_month(const fixed_date_t& date) const {
        const auto days = date - m_epoch;
        return static_cast<int32_t>(floor_div(mod(days, 365), 30) + 1);
    }
    int32_t simple_calendar::get_day(const fixed_date_t& date) const {
        return to_date(date).day;
    }
}