#include <gtest/gtest.h>
#include "holiday_csv_loader.hpp"
#include "holiday_providers/tibetan_calendar_holiday_provider.hpp"
#include "calendars/tibetan_calendar.hpp"
#include <algorithm>

namespace tests {

using namespace calendrical;

class tibetan_holidays_test : public ::testing::Test {
protected:
    calendars::tibetan_calendar m_tibetan{};
    holiday_providers::tibetan_calendar_holiday_provider m_provider{std::addressof(m_tibetan)};
    const holiday_csv& m_csv = holiday_csv::instance();
};

TEST_F(tibetan_holidays_test, losar) {
    m_csv.verify_holiday("losar", [this](int32_t y) { return m_provider.losar(y); });
}

TEST_F(tibetan_holidays_test, other_observances_in_year) {
    for (int32_t y = 2000; y <= 2010; ++y) {
        EXPECT_FALSE(m_provider.monlam_chenmo(y).empty());
        EXPECT_FALSE(m_provider.chotrul_duchen(y).empty());
        EXPECT_FALSE(m_provider.saga_dawa_duchen(y).empty());
        EXPECT_FALSE(m_provider.chokhor_duchen(y).empty());
        EXPECT_FALSE(m_provider.lhabab_duchen(y).empty());
        EXPECT_FALSE(m_provider.ganden_ngamcho(y).empty());
    }
}

TEST_F(tibetan_holidays_test, in_year) {
    for (int32_t y = 2000; y <= 2010; ++y) {
        const auto holidays = m_provider.in_year(y);
        EXPECT_FALSE(holidays.empty());
        EXPECT_TRUE(std::is_sorted(holidays.begin(), holidays.end(), calendrical::holiday_t::date_only_less_t{}));
    }
}

} // namespace tests
