#include <gtest/gtest.h>
#include "holiday_csv_loader.hpp"
#include "holiday_providers/persian_based_calendar_holiday_provider.hpp"
#include "calendars/persian_calendar.hpp"
#include "calendars/arithmetic_persian_calendar.hpp"
#include <algorithm>

namespace tests {

using namespace calendrical;

class persian_holidays_test : public ::testing::Test {
protected:
    calendars::persian_calendar m_persian{};
    holiday_providers::persian_based_calendar_holiday_provider m_provider{std::addressof(m_persian)};
    const holiday_csv& m_csv = holiday_csv::instance();
};

TEST_F(persian_holidays_test, nowruz) {
    m_csv.verify_holiday("nowruz", [this](int32_t y) { return m_provider.nawruz(y); });
}

TEST_F(persian_holidays_test, other_observances_in_year) {
    for (int32_t y = 2000; y <= 2010; ++y) {
        EXPECT_FALSE(m_provider.sizdah_bedar(y).empty());
        EXPECT_FALSE(m_provider.tirgan(y).empty());
        EXPECT_FALSE(m_provider.mehregan(y).empty());
        EXPECT_FALSE(m_provider.yalda_night(y).empty());
        EXPECT_FALSE(m_provider.sepandarmazgan(y).empty());
    }
}

TEST_F(persian_holidays_test, in_year) {
    for (int32_t y = 2000; y <= 2010; ++y) {
        const auto holidays = m_provider.in_year(y);
        EXPECT_FALSE(holidays.empty());
        EXPECT_TRUE(std::is_sorted(holidays.begin(), holidays.end(), calendrical::holiday_t::date_only_less_t{}));
    }
}

} // namespace tests
