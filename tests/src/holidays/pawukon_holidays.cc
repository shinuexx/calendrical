#include <gtest/gtest.h>
#include "holiday_csv_loader.hpp"
#include "holiday_providers/pawukon_calendar_holiday_provider.hpp"
#include "calendars/gregorian_calendar.hpp"
#include "calendars/pawukon_calendar.hpp"

#include <algorithm>
#include <cstdint>
#include <vector>

namespace tests {

using namespace calendrical;

class pawukon_holidays_test : public ::testing::Test {
protected:
    calendars::pawukon_calendar m_pawukon{};
    holiday_providers::pawukon_calendar_holiday_provider m_provider{std::addressof(m_pawukon)};
    const holiday_csv& m_csv = holiday_csv::instance();

    /** Checks that @p dates are every date in @p gyear spaced @p period days apart, each matching @p is_holiday. */
    template <typename Predicate>
    void expect_all_occurrences(const std::vector<fixed_date_t>& dates, int32_t gyear, int64_t period, Predicate is_holiday) const {
        const auto range = calendars::gregorian_calendar::standard.get_year_range(gyear);
        ASSERT_FALSE(dates.empty()) << gyear;
        EXPECT_LT(dates.front() - range.min, period) << gyear;
        EXPECT_LE(range.max - dates.back(), period) << gyear;
        for (size_t i = 0; i < dates.size(); ++i) {
            EXPECT_TRUE(range.contains(dates[i])) << gyear << ": " << dates[i];
            EXPECT_TRUE(is_holiday(m_pawukon.to_date(dates[i]))) << gyear << ": " << dates[i];
            if (i > 0) {
                EXPECT_EQ(dates[i] - dates[i - 1], period) << gyear;
            }
        }
        // No other day of the year qualifies.
        size_t count = 0;
        for (auto rd = range.min; range.contains(rd); ++rd)
            count += is_holiday(m_pawukon.to_date(rd)) ? 1 : 0;
        EXPECT_EQ(count, dates.size()) << gyear;
    }
};

TEST_F(pawukon_holidays_test, kajeng_keliwon_first) {
    m_csv.verify_holiday("Kajeng Keliwon (first)", [this](int32_t y) { return m_provider.kajeng_keliwon(y); });
}

TEST_F(pawukon_holidays_test, tumpek_first) {
    m_csv.verify_holiday("Tumpek (first)", [this](int32_t y) { return m_provider.tumpek(y); });
}

TEST_F(pawukon_holidays_test, kajeng_keliwon_every_15_days) {
    for (const int32_t y : {-500, 1, 1999, 2000, 2024, 2103, 3000}) {
        expect_all_occurrences(m_provider.kajeng_keliwon(y), y, 15,
            [](const calendars::pawukon_date& d) { return d.triwara == 3 && d.pancawara == 5; });
    }
}

TEST_F(pawukon_holidays_test, tumpek_every_35_days) {
    for (const int32_t y : {-500, 1, 1999, 2000, 2024, 2103, 3000}) {
        expect_all_occurrences(m_provider.tumpek(y), y, 35,
            [](const calendars::pawukon_date& d) { return d.pancawara == 5 && d.saptawara == 7; });
        for (const auto& rd : m_provider.tumpek(y))
            EXPECT_EQ(rd.get_weekday(), weekday::saturday) << rd;
    }
}

TEST_F(pawukon_holidays_test, tumpek_kuningan_2024) {
    // Kuningan, ten days after Galungan (2024-02-28), is Tumpek Kuningan.
    const auto kuningan = calendars::gregorian_calendar::standard.to_fixed({2024, month::march, 9});
    const auto tumpek = m_provider.tumpek(2024);
    EXPECT_NE(std::find(tumpek.begin(), tumpek.end(), kuningan), tumpek.end());
    const auto kajeng_keliwon = m_provider.kajeng_keliwon(2024);
    EXPECT_NE(std::find(kajeng_keliwon.begin(), kajeng_keliwon.end(), kuningan), kajeng_keliwon.end());
}

TEST_F(pawukon_holidays_test, in_year) {
    EXPECT_EQ(m_provider.calendar(), &m_pawukon);
    for (int32_t y = 2000; y <= 2010; ++y) {
        const auto holidays = m_provider.in_year(y);
        EXPECT_TRUE(std::is_sorted(holidays.begin(), holidays.end()));
        const auto kk = std::count_if(holidays.begin(), holidays.end(), [](const holiday_t& h) { return h.name == "kajeng keliwon"; });
        const auto tumpek = std::count_if(holidays.begin(), holidays.end(), [](const holiday_t& h) { return h.name == "tumpek"; });
        EXPECT_EQ(static_cast<size_t>(kk), m_provider.kajeng_keliwon(y).size());
        EXPECT_EQ(static_cast<size_t>(tumpek), m_provider.tumpek(y).size());
        EXPECT_EQ(holidays.size(), static_cast<size_t>(kk + tumpek));
    }
}

} // namespace tests
