#include <gtest/gtest.h>
#include "holiday_csv_loader.hpp"
#include "calendars/icelandic_calendar.hpp"

namespace tests {

using namespace calendrical;

class icelandic_holidays_test : public ::testing::Test {
protected:
    calendars::icelandic_calendar m_calendar{};
    const holiday_csv& m_csv = holiday_csv::instance();
};

TEST_F(icelandic_holidays_test, summer) {
    m_csv.verify_holiday("icelandic_summer", [this](int32_t y) { return m_calendar.summer(y); });
}

TEST_F(icelandic_holidays_test, winter) {
    m_csv.verify_holiday("icelandic_winter", [this](int32_t y) { return m_calendar.winter(y); });
}

TEST_F(icelandic_holidays_test, seasons_order) {
    for (int32_t y = 2000; y <= 2103; ++y) {
        const auto summer = m_calendar.summer(y);
        const auto winter = m_calendar.winter(y);
        EXPECT_LT(summer, winter) << "Summer should precede winter in year " << y;
    }
}

} // namespace tests
