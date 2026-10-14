#include <gtest/gtest.h>
#include "holiday_csv_loader.hpp"
#include "holiday_providers/chinese_calendar_holiday_provider.hpp"
#include "calendars/chinese_calendar.hpp"
#include <algorithm>

namespace tests {

using namespace calendrical;

class chinese_holidays_test : public ::testing::Test {
protected:
    calendars::chinese_calendar m_chinese{};
    holiday_providers::chinese_calendar_holiday_provider m_provider{std::addressof(m_chinese)};
    const holiday_csv& m_csv = holiday_csv::instance();
};

TEST_F(chinese_holidays_test, chinese_new_year) {
    m_csv.verify_holiday("chinese_new_year", [this](int32_t y) { return m_provider.new_year(y); });
}

TEST_F(chinese_holidays_test, qing_ming) {
    m_csv.verify_holiday("qing_ming", [this](int32_t y) { return m_provider.qing_ming(y); });
}

TEST_F(chinese_holidays_test, dragon_festival) {
    m_csv.verify_holiday("dragon_festival", [this](int32_t y) { return m_provider.dragon_festival(y); });
}

TEST_F(chinese_holidays_test, new_year_is_first_day_of_first_month) {
    for (int32_t y = 2000; y <= 2010; ++y) {
        const auto date = m_chinese.to_date(m_provider.new_year(y));
        EXPECT_EQ(date.month, 1);
        EXPECT_FALSE(date.leap_month);
        EXPECT_EQ(date.day, 1);
    }
}

TEST_F(chinese_holidays_test, in_year) {
    for (int32_t y = 2000; y <= 2010; ++y) {
        const auto holidays = m_provider.in_year(y);
        EXPECT_EQ(holidays.size(), 3u);
        EXPECT_TRUE(std::is_sorted(holidays.begin(), holidays.end()));
    }
}

} // namespace tests
