#include <gtest/gtest.h>
#include "holiday_csv_loader.hpp"
#include "holiday_providers/chinese_calendar_holiday_provider.hpp"
#include "calendars/chinese_calendar.hpp"
#include "calendars/korean_calendar.hpp"
#include "calendars/vietnamese_calendar.hpp"
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

TEST(chinese_family_holidays_test, new_year_uses_calendar_location) {
    const calendars::chinese_calendar chinese{};
    const calendars::korean_calendar korean{};
    const calendars::vietnamese_calendar vietnamese{};
    const holiday_providers::chinese_calendar_holiday_provider chinese_provider{std::addressof(chinese)};
    const holiday_providers::chinese_calendar_holiday_provider korean_provider{std::addressof(korean)};
    const holiday_providers::chinese_calendar_holiday_provider vietnamese_provider{std::addressof(vietnamese)};
    const auto& gregorian = calendars::gregorian_calendar::standard;

    // Usually all three coincide.
    EXPECT_EQ(chinese_provider.new_year(2026), gregorian.to_fixed({2026, month::february, 17}));
    EXPECT_EQ(korean_provider.new_year(2026), gregorian.to_fixed({2026, month::february, 17}));
    EXPECT_EQ(vietnamese_provider.new_year(2026), gregorian.to_fixed({2026, month::february, 17}));

    // Vietnam (UTC+7) sees the new moon a day earlier than China (UTC+8).
    EXPECT_EQ(chinese_provider.new_year(2007), gregorian.to_fixed({2007, month::february, 18}));
    EXPECT_EQ(vietnamese_provider.new_year(2007), gregorian.to_fixed({2007, month::february, 17}));

    // 1985: in UTC+7 a new moon falls on the other side of the winter solstice, so the leap
    // month lands differently and Tet comes a whole month before the Chinese New Year.
    EXPECT_EQ(chinese_provider.new_year(1985), gregorian.to_fixed({1985, month::february, 20}));
    EXPECT_EQ(vietnamese_provider.new_year(1985), gregorian.to_fixed({1985, month::january, 21}));

    for (const auto* provider : {&korean_provider, &vietnamese_provider}) {
        const auto* calendar = static_cast<const calendars::chinese_based_calendar*>(provider->calendar());
        for (int32_t y = 2000; y <= 2010; ++y) {
            const auto date = calendar->to_date(provider->new_year(y));
            EXPECT_EQ(date.month, 1);
            EXPECT_FALSE(date.leap_month);
            EXPECT_EQ(date.day, 1);
        }
    }
}

} // namespace tests
