#include <calendars/modern_hindu_solar_calendar.hpp>
#include <calendars/old_hindu_solar_calendar.hpp>
#include <iostream>
#include <time_and_astronomy.hpp>

#include <calendars/samaritan_calendar.hpp>

using calendrical::fixed_date_t;

int main() {
    calendrical::calendars::modern_hindu_solar_calendar calendar{};

    const auto date = 10_rd;
    const auto sdate = calendar.to_date(date);
    const auto sdate2 = calendrical::datetypes::simple_date{1, 2, 3};
    const auto rdate = calendar.to_fixed(sdate2);

    std::cout << date << " => " << sdate.to_string() << std::endl;
    std::cout << sdate2.to_string() << " => " << rdate << std::endl;
}
