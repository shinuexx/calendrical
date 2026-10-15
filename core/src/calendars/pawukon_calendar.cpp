#include "calendars/pawukon_calendar.hpp"
#include "helpers.hpp"
#include <stdexcept>
#include <string>

namespace calendrical::calendars {
    int32_t pawukon_calendar::day_from_fixed(const fixed_date_t& date) const {
        return static_cast<int32_t>(mod(date - ms_epoch, pawukon_date::cycle_length));
    }

    pawukon_date pawukon_calendar::to_date(const fixed_date_t& date) const {
        return pawukon_date::from_day(date - ms_epoch);
    }

    fixed_date_t pawukon_calendar::on_or_before(const pawukon_date& date, const fixed_date_t& reference) const {
        if (!date.is_valid())
            throw std::invalid_argument("pawukon_calendar::on_or_before: invalid date " + date.to_string());
        return reference.subtract(mod(reference - ms_epoch - date.day(), pawukon_date::cycle_length));
    }

    std::vector<fixed_date_t> pawukon_calendar::positions_in_range(int64_t day, int64_t period, const interval<fixed_date_t>& range) const {
        if (period <= 0 || pawukon_date::cycle_length % period != 0)
            throw std::invalid_argument("pawukon_calendar::positions_in_range: period " + std::to_string(period) + " does not divide 210");
        std::vector<fixed_date_t> result{};
        // First matching date on or after range.min; an open lower bound is filtered below.
        auto date = range.min + mod(day - (range.min - ms_epoch), period);
        for (; date <= range.max; date += period)
            if (range.contains(date))
                result.push_back(date);
        return result;
    }

    std::vector<fixed_date_t> pawukon_calendar::in_range(const pawukon_date& date, const interval<fixed_date_t>& range) const {
        if (!date.is_valid())
            throw std::invalid_argument("pawukon_calendar::in_range: invalid date " + date.to_string());
        return positions_in_range(date.day(), pawukon_date::cycle_length, range);
    }
}
