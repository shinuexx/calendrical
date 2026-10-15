#include "calendars/akan_calendar.hpp"
#include "helpers.hpp"
#include <stdexcept>

namespace calendrical::calendars {
    akan_name akan_calendar::to_date(const fixed_date_t& date) const {
        return akan_name::from(date - ms_epoch);
    }

    fixed_date_t akan_calendar::on_or_before(const akan_name& name, const fixed_date_t& reference) const {
        if (!name.is_valid())
            throw std::invalid_argument("akan_calendar::on_or_before: invalid name " + name.to_string());
        // Days from RD 0 to the nearest occurrence of name, reduced into (reference - 42, reference].
        return fixed_date_t{ interval_modulus(static_cast<int64_t>(ms_name_of_rd_zero.difference(name)), reference, reference.subtract(akan_name::cycle_length)) };
    }
}
