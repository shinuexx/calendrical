#include "calendars/pawukon_calendar.hpp"
#include "helpers.hpp"
#include <ranges>
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
        // Reduce day modulo period first: it may be any int64_t, and `%` cannot overflow here.
        const auto day_in_period = (day % period + period) % period;
        // CC's cap-Delta: the position of RD 0 in the cycle.
        const auto delta = static_cast<int64_t>(day_from_fixed(fixed_date_t{ 0 })) % period;
        // The helper scans the half-open range [min, max); map the boundary policy of range onto it.
        const int64_t first = range.contains(range.min) ? range.min : range.min + 1;
        const int64_t last = range.contains(range.max) ? range.max + 1 : range.max;
        const auto positions = calendrical::positions_in_range<int64_t>(
            day_in_period, period, delta, interval_left_closed_between(first, last));
        auto dates = positions | std::views::transform(bind_constructor<fixed_date_t>{});
        return { dates.begin(), dates.end() };
    }

    std::vector<fixed_date_t> pawukon_calendar::in_range(const pawukon_date& date, const interval<fixed_date_t>& range) const {
        if (!date.is_valid())
            throw std::invalid_argument("pawukon_calendar::in_range: invalid date " + date.to_string());
        return positions_in_range(date.day(), pawukon_date::cycle_length, range);
    }
}
