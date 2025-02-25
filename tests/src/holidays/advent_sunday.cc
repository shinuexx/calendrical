#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"
#include "local_helpers.hpp"

#include "holiday_providers/gregorian_calendar_holiday_provider.hpp"

namespace tests{
    static constexpr std::array month_day_dates{
        std::make_tuple<int32_t,int32_t>(12, 3),
        std::make_tuple<int32_t,int32_t>(12, 2),
        std::make_tuple<int32_t,int32_t>(12, 1),
        std::make_tuple<int32_t,int32_t>(11, 30),
        std::make_tuple<int32_t,int32_t>(11, 28),
        std::make_tuple<int32_t,int32_t>(11, 27),
        std::make_tuple<int32_t,int32_t>(12, 3),
        std::make_tuple<int32_t,int32_t>(12, 2),
        std::make_tuple<int32_t,int32_t>(11, 30),
        std::make_tuple<int32_t,int32_t>(11, 29),
        std::make_tuple<int32_t,int32_t>(11, 28),
        std::make_tuple<int32_t,int32_t>(11, 27),
        std::make_tuple<int32_t,int32_t>(12, 2),
        std::make_tuple<int32_t,int32_t>(12, 1),
        std::make_tuple<int32_t,int32_t>(11, 30),
        std::make_tuple<int32_t,int32_t>(11, 29),
        std::make_tuple<int32_t,int32_t>(11, 27),
        std::make_tuple<int32_t,int32_t>(12, 3),
        std::make_tuple<int32_t,int32_t>(12, 2),
        std::make_tuple<int32_t,int32_t>(12, 1),
        std::make_tuple<int32_t,int32_t>(11, 29),
        std::make_tuple<int32_t,int32_t>(11, 28),
        std::make_tuple<int32_t,int32_t>(11, 27),
        std::make_tuple<int32_t,int32_t>(12, 3),
        std::make_tuple<int32_t,int32_t>(12, 1),
        std::make_tuple<int32_t,int32_t>(11, 30),
        std::make_tuple<int32_t,int32_t>(11, 29),
        std::make_tuple<int32_t,int32_t>(11, 28),
        std::make_tuple<int32_t,int32_t>(12, 3),
        std::make_tuple<int32_t,int32_t>(12, 2),
        std::make_tuple<int32_t,int32_t>(12, 1),
        std::make_tuple<int32_t,int32_t>(11, 30),
        std::make_tuple<int32_t,int32_t>(11, 28),
        std::make_tuple<int32_t,int32_t>(11, 27),
        std::make_tuple<int32_t,int32_t>(12, 3),
        std::make_tuple<int32_t,int32_t>(12, 2),
        std::make_tuple<int32_t,int32_t>(11, 30),
        std::make_tuple<int32_t,int32_t>(11, 29),
        std::make_tuple<int32_t,int32_t>(11, 28),
        std::make_tuple<int32_t,int32_t>(11, 27),
        std::make_tuple<int32_t,int32_t>(12, 2),
        std::make_tuple<int32_t,int32_t>(12, 1),
        std::make_tuple<int32_t,int32_t>(11, 30),
        std::make_tuple<int32_t,int32_t>(11, 29),
        std::make_tuple<int32_t,int32_t>(11, 27),
        std::make_tuple<int32_t,int32_t>(12, 3),
        std::make_tuple<int32_t,int32_t>(12, 2),
        std::make_tuple<int32_t,int32_t>(12, 1),
        std::make_tuple<int32_t,int32_t>(11, 29),
        std::make_tuple<int32_t,int32_t>(11, 28),
        std::make_tuple<int32_t,int32_t>(11, 27),
        std::make_tuple<int32_t,int32_t>(12, 3),
        std::make_tuple<int32_t,int32_t>(12, 1),
        std::make_tuple<int32_t,int32_t>(11, 30),
        std::make_tuple<int32_t,int32_t>(11, 29),
        std::make_tuple<int32_t,int32_t>(11, 28),
        std::make_tuple<int32_t,int32_t>(12, 3),
        std::make_tuple<int32_t,int32_t>(12, 2),
        std::make_tuple<int32_t,int32_t>(12, 1),
        std::make_tuple<int32_t,int32_t>(11, 30),
        std::make_tuple<int32_t,int32_t>(11, 28),
        std::make_tuple<int32_t,int32_t>(11, 27),
        std::make_tuple<int32_t,int32_t>(12, 3),
        std::make_tuple<int32_t,int32_t>(12, 2),
        std::make_tuple<int32_t,int32_t>(11, 30),
        std::make_tuple<int32_t,int32_t>(11, 29),
        std::make_tuple<int32_t,int32_t>(11, 28),
        std::make_tuple<int32_t,int32_t>(11, 27),
        std::make_tuple<int32_t,int32_t>(12, 2),
        std::make_tuple<int32_t,int32_t>(12, 1),
        std::make_tuple<int32_t,int32_t>(11, 30),
        std::make_tuple<int32_t,int32_t>(11, 29),
        std::make_tuple<int32_t,int32_t>(11, 27),
        std::make_tuple<int32_t,int32_t>(12, 3),
        std::make_tuple<int32_t,int32_t>(12, 2),
        std::make_tuple<int32_t,int32_t>(12, 1),
        std::make_tuple<int32_t,int32_t>(11, 29),
        std::make_tuple<int32_t,int32_t>(11, 28),
        std::make_tuple<int32_t,int32_t>(11, 27),
        std::make_tuple<int32_t,int32_t>(12, 3),
        std::make_tuple<int32_t,int32_t>(12, 1),
        std::make_tuple<int32_t,int32_t>(11, 30),
        std::make_tuple<int32_t,int32_t>(11, 29),
        std::make_tuple<int32_t,int32_t>(11, 28),
        std::make_tuple<int32_t,int32_t>(12, 3),
        std::make_tuple<int32_t,int32_t>(12, 2),
        std::make_tuple<int32_t,int32_t>(12, 1),
        std::make_tuple<int32_t,int32_t>(11, 30),
        std::make_tuple<int32_t,int32_t>(11, 28),
        std::make_tuple<int32_t,int32_t>(11, 27),
        std::make_tuple<int32_t,int32_t>(12, 3),
        std::make_tuple<int32_t,int32_t>(12, 2),
        std::make_tuple<int32_t,int32_t>(11, 30),
        std::make_tuple<int32_t,int32_t>(11, 29),
        std::make_tuple<int32_t,int32_t>(11, 28),
        std::make_tuple<int32_t,int32_t>(11, 27),
        std::make_tuple<int32_t,int32_t>(12, 2),
        std::make_tuple<int32_t,int32_t>(12, 1),
        std::make_tuple<int32_t,int32_t>(11, 30),
        std::make_tuple<int32_t,int32_t>(11, 29),
        std::make_tuple<int32_t,int32_t>(11, 28),
        std::make_tuple<int32_t,int32_t>(11, 27),
        std::make_tuple<int32_t,int32_t>(12, 3),
        std::make_tuple<int32_t,int32_t>(12, 2),
    };

    class advent_sunday_test : public helpers::input_output_test<int32_t, std::tuple<int32_t,int32_t>> {
    protected:
        holiday_providers::gregorian_calendar_holiday_provider* m_provider;
        const calendars::gregorian_calendar* m_gregorian;

        void SetUp() override {
            m_provider = new holiday_providers::gregorian_calendar_holiday_provider();
            m_gregorian = std::addressof(calendars::gregorian_calendar::standard);
        }
        void TearDown() override {
            delete m_provider;
            m_provider = nullptr;
            m_gregorian = nullptr;
        }
    };

    TEST_P(advent_sunday_test, calculate) {
        auto &[year, month_day] = GetParam();
        auto &[month, day] = month_day;
        const auto actual = m_gregorian->to_date(m_provider->advent(year));
        const auto expected = datetypes::gregorian_date{year, calendrical::month{month}, day};
        EXPECT_EQ(actual, expected) << year << " : " << actual.to_string() << " : " << expected.to_string() << std::endl;
    }

    INSTANTIATE_TEST_SUITE_P(advent_sunday, advent_sunday_test, testing::ValuesIn(helpers::make_test_params_values(years, month_day_dates)));
}