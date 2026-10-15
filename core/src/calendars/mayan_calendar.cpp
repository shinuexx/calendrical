#include "calendars/mayan_calendar.hpp"

#include <array>
#include <stdexcept>

namespace calendrical::calendars {
    fixed_date_t mayan_calendar::to_fixed(const mayan_long_count& date) const {
        if (!is_valid_date(date)) {
            throw std::invalid_argument("Invalid or out-of-range Mayan Long Count");
        }
        // Every partial sum is representable: validation reserves a whole alautun
        // of headroom, and the nonnegative lower fields total at most one block - 1.
        const int64_t days = static_cast<int64_t>(date.alautun) * mayan_long_count::days_per_alautun
            + static_cast<int64_t>(date.kinchiltun) * 1152000000LL
            + static_cast<int64_t>(date.cakabtun) * 57600000LL
            + static_cast<int64_t>(date.pictun) * 2880000LL
            + static_cast<int64_t>(date.baktun) * 144000LL
            + static_cast<int64_t>(date.katun) * 7200LL
            + static_cast<int64_t>(date.tun) * 360LL
            + static_cast<int64_t>(date.uinal) * 20LL
            + date.kin;
        return epoch().add(days);
    }

    mayan_long_count mayan_calendar::to_date(const fixed_date_t& date) const {
        if (date < min_date || date > max_date) {
            throw std::out_of_range("Fixed date outside the Mayan Long Count domain");
        }
        auto remaining = date - epoch();
        constexpr std::array<int64_t, 9> weights{
            mayan_long_count::days_per_alautun, 1152000000LL, 57600000LL,
            2880000LL, 144000LL, 7200LL, 360LL, 20LL, 1LL};
        std::array<int32_t, 9> fields{};
        for (std::size_t i = 0; i < weights.size(); ++i) {
            fields[i] = static_cast<int32_t>(floor_div(remaining, weights[i]));
            remaining = mod(remaining, weights[i]);
        }
        return {fields[0], fields[1], fields[2], fields[3], fields[4],
                fields[5], fields[6], fields[7], fields[8]};
    }
}
