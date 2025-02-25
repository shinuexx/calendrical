#include "sample_fixed_dates.hpp"
#include <gtest/gtest.h>

#include "time_and_astronomy.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::time_and_astronomy;

    static constexpr auto results = std::array<moment_t, 33>{
        -214174.605828,
        -61382.995328,
        25495.809776,
        49238.502448,
        171318.435313,
        210180.691849,
        253442.859367,
        369763.746413,
        400091.578343,
        434376.578106,
        452627.191972,
        470167.578360,
        473858.853276,
        507878.666842,
        524179.247062,
        544702.753873,
        567146.513181,
        569479.203258,
        601727.033557,
        613449.762129,
        626620.369801,
        645579.076748,
        664242.886718,
        671418.970538,
        694807.563371,
        704433.491182,
        708863.597000,
        709424.404929,
        709602.082686,
        727291.209400,
        728737.447691,
        744329.573999,
        764676.191273
    };

    class next_new_moon_test : public helpers::input_output_test<fixed_date_t, moment_t>{};

    TEST_P(next_new_moon_test, calculate) {
        constexpr moment_t epsilon = moment_t::hms(0,1);
        auto &[rd, output] = GetParam();
        const moment_t input = rd;
        EXPECT_NEAR(new_moon_on_or_after(input), output, epsilon) << moment_t::to_hh_mm_ss(output) << " : " << moment_t::to_hh_mm_ss(new_moon_on_or_after(input)) << " : " << input << std::endl;
    }


    INSTANTIATE_TEST_CASE_P(next_new_moon, next_new_moon_test, testing::ValuesIn(helpers::make_test_params_values(dates, results)));
}