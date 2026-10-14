#include <gtest/gtest.h>
#include "holiday_csv_loader.hpp"
#include "holiday_providers/coptic_based_calendar_holiday_provider.hpp"
#include "holiday_providers/julian_calendar_holiday_provider.hpp"
#include "calendars/coptic_based_calendar.hpp"
#include <algorithm>

namespace tests {

using namespace calendrical;

class coptic_holidays_test : public ::testing::Test {
protected:
    holiday_providers::coptic_based_calendar_holiday_provider m_provider{std::addressof(calendars::coptic_calendar)};
    const holiday_csv& m_csv = holiday_csv::instance();
};

TEST_F(coptic_holidays_test, christmas) {
    m_csv.verify_holiday("coptic_christmas", [this](int32_t y) { return m_provider.christmas(y); });
}

TEST_F(coptic_holidays_test, other_observances_in_year) {
    for (int32_t y = 2000; y <= 2010; ++y) {
        EXPECT_FALSE(m_provider.building_of_the_cross(y).empty());
        EXPECT_FALSE(m_provider.jesus_circumcision(y).empty());
        EXPECT_FALSE(m_provider.epiphany(y).empty());
        EXPECT_FALSE(m_provider.mary_announcement(y).empty());
        EXPECT_FALSE(m_provider.jesus_transfiguration(y).empty());
    }
}

TEST_F(coptic_holidays_test, in_year) {
    for (int32_t y = 2000; y <= 2010; ++y) {
        const auto holidays = m_provider.in_year(y);
        EXPECT_FALSE(holidays.empty());
        EXPECT_TRUE(std::is_sorted(holidays.begin(), holidays.end(), calendrical::holiday_t::date_only_less_t{}));
    }
}

class julian_holidays_test : public ::testing::Test {
protected:
    holiday_providers::julian_calendar_holiday_provider m_provider{};
    const holiday_csv& m_csv = holiday_csv::instance();
};

TEST_F(julian_holidays_test, orthodox_christmas) {
    m_csv.verify_holiday("orthodox_christmas", [this](int32_t y) { return m_provider.orthodox_christmas(y); });
}

TEST_F(julian_holidays_test, orthodox_easter) {
    m_csv.verify_holiday("orthodox_easter", [this](int32_t y) { return m_provider.orthodox_easter(y); });
}

TEST_F(julian_holidays_test, orthodox_pentecost) {
    for (int32_t y = 2000; y <= 2010; ++y) {
        const auto easter = m_provider.orthodox_easter(y);
        const auto pentecost = m_provider.orthodox_pentecost(y);
        EXPECT_EQ(pentecost, easter + 49);
    }
}

TEST_F(julian_holidays_test, in_year) {
    for (int32_t y = 2000; y <= 2010; ++y) {
        const auto holidays = m_provider.in_year(y);
        EXPECT_FALSE(holidays.empty());
        EXPECT_TRUE(std::is_sorted(holidays.begin(), holidays.end(), calendrical::holiday_t::date_only_less_t{}));
    }
}

} // namespace tests
