#include <gtest/gtest.h>
#include "holiday_providers/samaritan_calendar_holiday_provider.hpp"
#include "holiday_providers/simple_calendar_holiday_provider.hpp"
#include "calendars/samaritan_calendar.hpp"
#include "calendars/simple_calendar.hpp"
#include <algorithm>

namespace tests {

using namespace calendrical;

class samaritan_holidays_test : public ::testing::Test {
protected:
    calendars::samaritan_calendar m_calendar{};
    holiday_providers::samaritan_calendar_holiday_provider m_provider{std::addressof(m_calendar)};
};

TEST_F(samaritan_holidays_test, observances_and_ranges) {
    for (int32_t y = 2000; y <= 2010; ++y) {
        EXPECT_FALSE(m_provider.passover(y).empty());
        EXPECT_FALSE(m_provider.festival_of_unleavened_bread(y).empty());
        EXPECT_FALSE(m_provider.festival_of_the_seventh_month(y).empty());
        EXPECT_FALSE(m_provider.day_of_atonement(y).empty());
        EXPECT_FALSE(m_provider.festival_of_tabernacles(y).empty());
        EXPECT_FALSE(m_provider.eighth_day(y).empty());
        EXPECT_NE(m_provider.festival_of_pentecost(y), fixed_date_t{0});
        EXPECT_NE(m_provider.simmut_of_passover(y), fixed_date_t{0});
        EXPECT_NE(m_provider.simmut_of_tabernacles(y), fixed_date_t{0});
    }
}

TEST_F(samaritan_holidays_test, in_year) {
    for (int32_t y = 2000; y <= 2010; ++y) {
        const auto holidays = m_provider.in_year(y);
        EXPECT_FALSE(holidays.empty());
        EXPECT_TRUE(std::is_sorted(holidays.begin(), holidays.end(), calendrical::holiday_t::date_only_less_t{}));
    }
}

class simple_holidays_test : public ::testing::Test {
protected:
    holiday_providers::simple_calendar_holiday_provider m_provider{std::addressof(calendars::armenian_calendar)};
};

TEST_F(simple_holidays_test, navasard) {
    for (int32_t y = 2000; y <= 2010; ++y) {
        EXPECT_FALSE(m_provider.navasard(y).empty());
    }
}

TEST_F(simple_holidays_test, in_year) {
    for (int32_t y = 2000; y <= 2010; ++y) {
        const auto holidays = m_provider.in_year(y);
        EXPECT_FALSE(holidays.empty());
        EXPECT_TRUE(std::is_sorted(holidays.begin(), holidays.end(), calendrical::holiday_t::date_only_less_t{}));
    }
}

} // namespace tests
