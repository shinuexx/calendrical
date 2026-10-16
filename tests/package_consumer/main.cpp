#include "angle.hpp"
#include "calendars/gregorian_calendar.hpp"
#include "calendars/hebrew_calendar.hpp"
#include "fixed_date.hpp"
#include "moment.hpp"
#include "version.hpp"

#include <iostream>

int main() {
    using namespace calendrical;
    const auto rd = calendars::gregorian_calendar::standard.to_fixed({2026, month::february, 18});
    const auto hebrew = calendars::hebrew_calendar{}.to_date(rd);
    std::cout << rd << " " << hebrew.to_string() << '\n' << libinfo();

    // Typed arithmetic with explicit scalar access (issue #4).
    const fixed_date_t next = rd + 1;
    const moment_t noon = moment_t{rd} + 0.5;
    const angle_t quarter = 90_deg;
    const bool typed_ok = next - rd == 1 && floor(noon) == rd && noon.days() == static_cast<double>(rd.value()) + 0.5 &&
                          quarter / angle_t::full_circle == 0.25 && quarter.degrees() == 90.0;

    // The installed headers (version_config.hpp) must match the installed binary.
    return hebrew.year == 5786 && typed_ok && library_version() == version() ? 0 : 1;
}
