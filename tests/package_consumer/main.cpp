#include "calendars/gregorian_calendar.hpp"
#include "calendars/hebrew_calendar.hpp"

#include <iostream>

int main() {
    using namespace calendrical;
    const auto rd = calendars::gregorian_calendar::standard.to_fixed({2026, month::february, 18});
    const auto hebrew = calendars::hebrew_calendar{}.to_date(rd);
    std::cout << rd << " " << hebrew.to_string() << '\n';
    return hebrew.year == 5786 ? 0 : 1;
}
