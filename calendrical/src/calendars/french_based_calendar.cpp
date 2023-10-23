#include "calendars/french_based_calendar.hpp"

namespace calendrical::calendars {
    int32_t french_based_calendar::get_month(const fixed_date_t& date, const fixed_date_t& new_year) const
    {
        return static_cast<int32_t>(floor_div(date - new_year, 30) + 1);
    }
    int32_t french_based_calendar::get_day(const fixed_date_t& date, const fixed_date_t& new_year) const
    {
        return static_cast<int32_t>(mod(date - new_year, 30) + 1);
    }
}