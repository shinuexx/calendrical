#include <calendars/modern_hindu_base.hpp>


#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"

#include "time_and_astronomy.hpp"
#include "local_helpers.hpp"
#include "location.hpp"

namespace tests {
    using namespace calendrical::time_and_astronomy;

    static constexpr auto bogus = std::numeric_limits<double>::quiet_NaN();
    static constexpr auto is_bogus(double value) { return value <=> 0 == std::partial_ordering::unordered; }

    static constexpr auto inputs = std::array{
        0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24
    };

    static const auto angles = std::array{
        rational_angle_t{},
        rational_angle_t::dms(0,225),
        rational_angle_t::dms(0,450),
        rational_angle_t::dms(0,675),
        rational_angle_t::dms(0,900),
        rational_angle_t::dms(0,1125),
        rational_angle_t::dms(0,1350),
        rational_angle_t::dms(0,1575),
        rational_angle_t::dms(0,1800),
        rational_angle_t::dms(0,2025),
        rational_angle_t::dms(0,2250),
        rational_angle_t::dms(0,2475),
        rational_angle_t::dms(0,2700),
        rational_angle_t::dms(0,2925),
        rational_angle_t::dms(0,3150),
        rational_angle_t::dms(0,3375),
        rational_angle_t::dms(0,3600),
        rational_angle_t::dms(0,3825),
        rational_angle_t::dms(0,4050),
        rational_angle_t::dms(0,4275),
        rational_angle_t::dms(0,4500),
        rational_angle_t::dms(0,4725),
        rational_angle_t::dms(0,4950),
        rational_angle_t::dms(0,5175),
        rational_angle_t::dms(0,5400)
    };

    static const auto sines = std::array{
        rational{0},
        rational{225,3438},
        rational{449,3438},
        rational{671,3438},
        rational{890,3438},
        rational{1105,3438},
        rational{1315,3438},
        rational{1520,3438},
        rational{1719,3438},
        rational{1910,3438},
        rational{2093,3438},
        rational{2267,3438},
        rational{2431,3438},
        rational{2585,3438},
        rational{2728,3438},
        rational{2859,3438},
        rational{2978,3438},
        rational{3084,3438},
        rational{3177,3438},
        rational{3256,3438},
        rational{3321,3438},
        rational{3372,3438},
        rational{3409,3438},
        rational{3431,3438},
        rational{3438,3438}
    };


    class hindu_sine_entry_test : public helpers::input_output_test<int32_t, rational>{};

    TEST_P(hindu_sine_entry_test, calculate) {
        auto &[rd, output] = GetParam();
        const auto actual = calendars::modern_hindu_base::hindu_sine_table(rd);
        EXPECT_EQ(actual, output) << output << " : " << actual << " : " << rd << std::endl;
    }

    class hindu_sine_test : public helpers::input_output_test<rational_angle_t, rational>{};

    TEST_P(hindu_sine_test, calculate) {
        auto &[rd, output] = GetParam();
        const auto actual = calendars::modern_hindu_base::hindu_sine(rd);
        EXPECT_EQ(actual, output) << output << " : " << actual << " : " << rd << std::endl;
    }

    class hindu_arcsine_test : public helpers::input_output_test<rational, rational_angle_t>{};

    TEST_P(hindu_arcsine_test, calculate) {
        auto &[rd, output] = GetParam();
        const auto actual = calendars::modern_hindu_base::hindu_arcsin(rd);
        EXPECT_EQ(actual, output) << output << " : " << actual << " : " << rd << std::endl;
    }

    INSTANTIATE_TEST_CASE_P(hindu_sine_table, hindu_sine_entry_test, testing::ValuesIn(helpers::make_test_params_values(inputs, sines)));
    INSTANTIATE_TEST_CASE_P(hindu_sine, hindu_sine_test, testing::ValuesIn(helpers::make_test_params_values(angles, sines)));
    INSTANTIATE_TEST_CASE_P(hindu_arcsine, hindu_arcsine_test, testing::ValuesIn(helpers::make_test_params_values(sines, angles)));
}