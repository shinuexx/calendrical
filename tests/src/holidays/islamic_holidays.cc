#include <gtest/gtest.h>
#include "holiday_csv_loader.hpp"
#include "holiday_providers/islamic_based_calendar_holiday_provider.hpp"
#include "calendars/islamic_calendar.hpp"
#include <algorithm>

namespace tests {

using namespace calendrical;

class islamic_holidays_test : public ::testing::Test {
protected:
    calendars::islamic_calendar m_islamic{};
    holiday_providers::islamic_based_calendar_holiday_provider m_provider{std::addressof(m_islamic)};
    const holiday_csv& m_csv = holiday_csv::instance();
};

TEST_F(islamic_holidays_test, mawlid) {
    m_csv.verify_holiday("mawlid", [this](int32_t y) { return m_provider.mawlid(y); });
}

TEST_F(islamic_holidays_test, other_observances_in_year) {
    for (int32_t y = 2000; y <= 2010; ++y) {
        EXPECT_FALSE(m_provider.islamic_new_year(y).empty());
        EXPECT_FALSE(m_provider.ashura(y).empty());
        EXPECT_FALSE(m_provider.lailat_al_miraj(y).empty());
        EXPECT_FALSE(m_provider.lailat_al_baraa(y).empty());
        EXPECT_FALSE(m_provider.ramadan(y).empty());
        EXPECT_FALSE(m_provider.lailat_al_qadr(y).empty());
        EXPECT_FALSE(m_provider.eid_ul_fitr(y).empty());
        EXPECT_FALSE(m_provider.eid_ul_adha(y).empty());
    }
}

TEST_F(islamic_holidays_test, in_year) {
    for (int32_t y = 2000; y <= 2010; ++y) {
        const auto holidays = m_provider.in_year(y);
        EXPECT_FALSE(holidays.empty());
        EXPECT_TRUE(std::is_sorted(holidays.begin(), holidays.end(), calendrical::holiday_t::date_only_less_t{}));
    }
}

} // namespace tests
