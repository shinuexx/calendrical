#include "sample_fixed_dates.hpp"
#include <gtest/gtest.h>

namespace tests {
    using namespace calendrical;

    static constexpr std::array<weekday, 33> expected_weekdays{
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

    TEST(basics, valid_weekday) {
        for (std::size_t i = 0; i < dates.size(); ++i) {
            EXPECT_EQ(dates[i].get_weekday(), expected_weekdays[i]) << i << " : " << dates[i] << " : " << dates[i].get_weekday() << " : " << expected_weekdays[i] << std::endl;
        }
    }
}

