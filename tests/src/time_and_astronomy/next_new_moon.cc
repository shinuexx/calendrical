#include "sample_fixed_dates.hpp"
#include <gtest/gtest.h>

#include "time_and_astronomy.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::time_and_astronomy;

    static constexpr auto results = std::array<moment_t, 33>{
        -214174.605828_m,
        -61382.995328_m,
        25495.809776_m,
        49238.502448_m,
        171318.435313_m,
        210180.691849_m,
        253442.859367_m,
        369763.746413_m,
        400091.578343_m,
        434376.578106_m,
        452627.191972_m,
        470167.578360_m,
        473858.853276_m,
        507878.666842_m,
        524179.247062_m,
        544702.753873_m,
        567146.513181_m,
        569479.203258_m,
        601727.033557_m,
        613449.762129_m,
        626620.369801_m,
        645579.076748_m,
        664242.886718_m,
        671418.970538_m,
        694807.563371_m,
        704433.491182_m,
        708863.597000_m,
        709424.404929_m,
        709602.082686_m,
        727291.209400_m,
        728737.447691_m,
        744329.573999_m,
        764676.191273_m
    };

    class next_new_moon_test : public helpers::input_output_test<fixed_date_t, moment_t>{};

    TEST_P(next_new_moon_test, calculate) {
        constexpr moment_t epsilon = moment_t::hms(0,1);
        auto &[rd, output] = GetParam();
        const moment_t input{ rd };
        EXPECT_NEAR(new_moon_on_or_after(input).days(), output.days(), epsilon.days()) << moment_t::to_hh_mm_ss(output) << " : " << moment_t::to_hh_mm_ss(new_moon_on_or_after(input)) << " : " << input << std::endl;
    }


    INSTANTIATE_TEST_SUITE_P(next_new_moon, next_new_moon_test, testing::ValuesIn(helpers::make_test_params_values(dates, results)));
}