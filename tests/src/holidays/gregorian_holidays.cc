#include <gtest/gtest.h>
#include "holiday_csv_loader.hpp"
#include "holiday_providers/gregorian_calendar_holiday_provider.hpp"
#include <algorithm>

namespace tests {

using namespace calendrical;

class gregorian_holidays_test : public ::testing::Test {
protected:
    holiday_providers::gregorian_calendar_holiday_provider m_provider{};
    const holiday_csv& m_csv = holiday_csv::instance();
};

TEST_F(gregorian_holidays_test, christmas) {
    m_csv.verify_holiday("christmas", [this](int32_t y) { return m_provider.christmas(y); });
}

TEST_F(gregorian_holidays_test, epiphany) {
    m_csv.verify_holiday("epiphany", [this](int32_t y) { return m_provider.epiphany(y); });
}

TEST_F(gregorian_holidays_test, easter) {
    m_csv.verify_holiday("easter", [this](int32_t y) { return m_provider.easter(y); });
}

TEST_F(gregorian_holidays_test, astronomical_easter) {
    m_csv.verify_holiday("astronomical_easter", [this](int32_t y) { return m_provider.astronomical_easter(y); });
}

TEST_F(gregorian_holidays_test, pentecost) {
    m_csv.verify_holiday("pentecost", [this](int32_t y) { return m_provider.pentecost(y); });
}

TEST_F(gregorian_holidays_test, advent_sunday) {
    m_csv.verify_holiday("advent_sunday", [this](int32_t y) { return m_provider.advent(y); });
}

TEST_F(gregorian_holidays_test, unlucky_fridays) {
    m_csv.verify_holiday("unlucky_fridays", [this](int32_t y) { return m_provider.unlucky_fridays(y); });
}

TEST_F(gregorian_holidays_test, in_year) {
    for (int32_t y = 2000; y <= 2010; ++y) {
        const auto holidays = m_provider.in_year(y);
        EXPECT_FALSE(holidays.empty());
        EXPECT_TRUE(std::is_sorted(holidays.begin(), holidays.end(), calendrical::holiday_t::date_only_less_t{}));
    }
}

} // namespace tests
