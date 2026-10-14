#include "sample_fixed_dates.hpp"
#include <gtest/gtest.h>
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical;

    static constexpr std::array expected_weekdays{
        weekday::sunday,
        weekday::wednesday,
        weekday::wednesday,
        weekday::sunday,
        weekday::wednesday,
        weekday::monday,
        weekday::saturday,
        weekday::sunday,
        weekday::sunday,
        weekday::friday,
        weekday::saturday,
        weekday::friday,
        weekday::sunday,
        weekday::sunday,
        weekday::wednesday,
        weekday::saturday,
        weekday::saturday,
        weekday::saturday,
        weekday::wednesday,
        weekday::sunday,
        weekday::friday,
        weekday::sunday,
        weekday::monday,
        weekday::wednesday,
        weekday::sunday,
        weekday::sunday,
        weekday::monday,
        weekday::monday,
        weekday::thursday,
        weekday::tuesday,
        weekday::sunday,
        weekday::wednesday,
        weekday::sunday,
    };

    class weekday_test : public helpers::input_output_test<fixed_date_t, weekday> {};

    TEST_P(weekday_test, valid_weekday) {
        auto& [input, expected] = GetParam();
        EXPECT_EQ(input.get_weekday(), expected) << input << " : " << input.get_weekday() << " : " << expected << std::endl;
    }

    INSTANTIATE_TEST_SUITE_P(weekdays, weekday_test, testing::ValuesIn(helpers::make_test_params_values(dates, expected_weekdays)));
}

