#include "calendars/hindu_base.hpp"

namespace calendrical::calendars{
    int64_t hindu_base::day_count(const fixed_date_t& date) const {
        return date - ms_epoch;
    }

    int32_t hindu_base::jovian_year(const fixed_date_t& date) const {
        const auto a = static_cast<int64_t>(floor_div(static_cast<double>(day_count(date)), arya_jovian_period / 12)) + 27;
        return static_cast<int32_t>(adjusted_remainder(a, 60));
    }
}