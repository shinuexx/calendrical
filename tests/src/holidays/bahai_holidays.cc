#include <gtest/gtest.h>
#include "holiday_csv_loader.hpp"
#include "holiday_providers/bahai_based_calendar_holiday_provider.hpp"
#include "calendars/astro_bahai_calendar.hpp"
#include "calendars/bahai_calendar.hpp"
#include <algorithm>

namespace tests {

using namespace calendrical;

class bahai_holidays_test : public ::testing::Test {
protected:
    calendars::astro_bahai_calendar m_astro{};
    calendars::bahai_calendar m_bahai{};
    holiday_providers::bahai_based_calendar_holiday_provider m_provider{std::addressof(m_astro), std::addressof(m_bahai)};
    const holiday_csv& m_csv = holiday_csv::instance();
};

TEST_F(bahai_holidays_test, bahai_new_year) {
    m_csv.verify_holiday("bahai_new_year", [this](int32_t y) { return m_provider.new_year(y); });
}

TEST_F(bahai_holidays_test, naw_ruz) {
    m_csv.verify_holiday("naw_ruz", [this](int32_t y) { return m_provider.naw_ruz(y); });
}

TEST_F(bahai_holidays_test, feast_of_ridvan) {
    m_csv.verify_holiday("feast_of_ridvan", [this](int32_t y) { return m_provider.feast_of_ridvan(y); });
}

TEST_F(bahai_holidays_test, birth_of_the_bab) {
    m_csv.verify_holiday("birth_of_the_bab", [this](int32_t y) { return m_provider.birth_of_the_bab(y); });
}

TEST_F(bahai_holidays_test, in_year) {
    for (int32_t y = 2000; y <= 2010; ++y) {
        const auto holidays = m_provider.in_year(y);
        EXPECT_FALSE(holidays.empty());
        EXPECT_TRUE(std::is_sorted(holidays.begin(), holidays.end(), calendrical::holiday_t::date_only_less_t{}));
    }
}

} // namespace tests
