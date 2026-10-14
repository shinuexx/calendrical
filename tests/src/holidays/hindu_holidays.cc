#include <gtest/gtest.h>
#include "holiday_csv_loader.hpp"
#include "holiday_providers/modern_hindu_lunar_calendar_holiday_provider.hpp"
#include "holiday_providers/modern_hindu_solar_calendar_holiday_provider.hpp"
#include "calendars/modern_hindu_lunar_calendar.hpp"
#include "calendars/modern_hindu_solar_calendar.hpp"
#include <algorithm>

namespace tests {

using namespace calendrical;

class hindu_holidays_test : public ::testing::Test {
protected:
    calendars::modern_hindu_lunar_calendar m_lunar{};
    calendars::modern_hindu_solar_calendar m_solar{};
    holiday_providers::modern_hindu_lunar_calendar_holiday_provider m_lunar_provider{std::addressof(m_lunar)};
    holiday_providers::modern_hindu_solar_calendar_holiday_provider m_solar_provider{std::addressof(m_solar)};
    const holiday_csv& m_csv = holiday_csv::instance();
};

TEST_F(hindu_holidays_test, diwali) {
    m_csv.verify_holiday("diwali", [this](int32_t y) { return m_lunar_provider.diwali(y); });
}

TEST_F(hindu_holidays_test, sacred_wednesdays) {
    m_csv.verify_holiday("sacred_wednesday", [this](int32_t y) { return m_lunar_provider.sacred_wednesdays(y); });
}

TEST_F(hindu_holidays_test, lunar_in_year) {
    for (int32_t y = 2000; y <= 2010; ++y) {
        const auto holidays = m_lunar_provider.in_year(y);
        EXPECT_FALSE(holidays.empty());
        EXPECT_TRUE(std::is_sorted(holidays.begin(), holidays.end(), calendrical::holiday_t::date_only_less_t{}));
    }
}

TEST_F(hindu_holidays_test, solar_in_year) {
    for (int32_t y = 2000; y <= 2010; ++y) {
        const auto holidays = m_solar_provider.in_year(y);
        EXPECT_FALSE(holidays.empty());
        EXPECT_TRUE(std::is_sorted(holidays.begin(), holidays.end(), calendrical::holiday_t::date_only_less_t{}));
    }
}

TEST_F(hindu_holidays_test, solar_observances) {
    for (int32_t y = 2000; y <= 2010; ++y) {
        EXPECT_NE(m_solar_provider.sowramana_ugadi(y), fixed_date_t{0});
        EXPECT_FALSE(m_solar_provider.ayyappa_jyothi_darshanam(y).empty());
    }
}

} // namespace tests
