#include <gtest/gtest.h>
#include "holiday_csv_loader.hpp"
#include "holiday_providers/us_holiday_provider.hpp"
#include <algorithm>

namespace tests {

using namespace calendrical;

class us_holidays_test : public ::testing::Test {
protected:
    holiday_providers::us_holiday_provider m_provider{};
    const holiday_csv& m_csv = holiday_csv::instance();
};

TEST_F(us_holidays_test, independence_day) {
    m_csv.verify_holiday("us_independence_day", [this](int32_t y) { return m_provider.independence_day(y); });
}

TEST_F(us_holidays_test, labor_day) {
    m_csv.verify_holiday("us_labor_day", [this](int32_t y) { return m_provider.labor_day(y); });
}

TEST_F(us_holidays_test, memorial_day) {
    m_csv.verify_holiday("us_memorial_day", [this](int32_t y) { return m_provider.memorial_day(y); });
}

TEST_F(us_holidays_test, election_day) {
    m_csv.verify_holiday("us_election_day", [this](int32_t y) { return m_provider.election_day(y); });
}

TEST_F(us_holidays_test, daylight_saving_start) {
    m_csv.verify_holiday("us_daylight_saving_start", [this](int32_t y) { return m_provider.daylight_saving_start(y); });
}

TEST_F(us_holidays_test, daylight_saving_end) {
    m_csv.verify_holiday("us_daylight_saving_end", [this](int32_t y) { return m_provider.daylight_saving_end(y); });
}

TEST_F(us_holidays_test, calendar_interface) {
    EXPECT_NE(m_provider.calendar(), nullptr);
    EXPECT_EQ(m_provider.calendar(), std::addressof(calendars::gregorian_calendar::standard));
}

TEST_F(us_holidays_test, in_year) {
    for (int32_t y = 2000; y <= 2103; ++y) {
        const auto holidays = m_provider.in_year(y);
        EXPECT_EQ(holidays.size(), 6U);
        EXPECT_TRUE(std::is_sorted(holidays.begin(), holidays.end()));

        // Verify all expected holiday names are present in in_year
        std::vector<std::string> names;
        names.reserve(holidays.size());
        for (const auto& h : holidays) {
            names.push_back(h.name);
        }
        std::ranges::sort(names);
        const std::vector<std::string> expected_names = {
            "daylight saving end",
            "daylight saving start",
            "election day",
            "independence day",
            "labor day",
            "memorial day"
        };
        EXPECT_EQ(names, expected_names);
    }
}

} // namespace tests
