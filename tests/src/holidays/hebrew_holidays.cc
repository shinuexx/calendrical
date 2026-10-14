#include <gtest/gtest.h>
#include "holiday_csv_loader.hpp"
#include "holiday_providers/hebrew_based_calendar_holiday_provider.hpp"
#include "calendars/hebrew_calendar.hpp"
#include <algorithm>

namespace tests {

using namespace calendrical;

class hebrew_holidays_test : public ::testing::Test {
protected:
    calendars::hebrew_calendar m_hebrew{};
    holiday_providers::hebrew_based_calendar_holiday_provider m_provider{std::addressof(m_hebrew)};
    const holiday_csv& m_csv = holiday_csv::instance();
};

TEST_F(hebrew_holidays_test, yom_kippur) {
    m_csv.verify_holiday("yom_kippur", [this](int32_t y) { return m_provider.yom_kippur(y); });
}

TEST_F(hebrew_holidays_test, passover) {
    m_csv.verify_holiday("passover", [this](int32_t y) { return m_provider.passover(y); });
}

TEST_F(hebrew_holidays_test, purim) {
    m_csv.verify_holiday("purim", [this](int32_t y) { return m_provider.purim(y); });
}

TEST_F(hebrew_holidays_test, ta_anit_esther) {
    m_csv.verify_holiday("ta_anit_esther", [this](int32_t y) { return m_provider.ta_anit_esther(y); });
}

TEST_F(hebrew_holidays_test, tishah_be_av) {
    m_csv.verify_holiday("tishah_be_av", [this](int32_t y) { return m_provider.tishah_be_av(y); });
}

TEST_F(hebrew_holidays_test, yom_ha_zikkaron) {
    m_csv.verify_holiday("yom_ha_zikkaron", [this](int32_t y) { return m_provider.yom_ha_zikkaron(y); });
}

TEST_F(hebrew_holidays_test, sh_ela) {
    m_csv.verify_holiday("sh_ela", [this](int32_t y) { return m_provider.sh_ela(y); });
}

TEST_F(hebrew_holidays_test, hanukkah) {
    m_csv.verify_holiday("hanukkah", [this](int32_t y) { return m_provider.hanukkah(y); });
}

TEST_F(hebrew_holidays_test, birkath_ha_hama) {
    m_csv.verify_holiday("birkath_ha_hama", [this](int32_t y) { return m_provider.birhath_ha_hama(y); });
}

TEST_F(hebrew_holidays_test, in_year) {
    for (int32_t y = 2000; y <= 2010; ++y) {
        const auto holidays = m_provider.in_year(y);
        EXPECT_FALSE(holidays.empty());
        EXPECT_TRUE(std::is_sorted(holidays.begin(), holidays.end(), calendrical::holiday_t::date_only_less_t{}));
    }
}

} // namespace tests
