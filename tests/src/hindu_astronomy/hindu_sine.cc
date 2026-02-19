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

    static constexpr auto angles = std::array{
        angle_t{},
        angle_t::dms(0,225),
        angle_t::dms(0,450),
        angle_t::dms(0,675),
        angle_t::dms(0,900),
        angle_t::dms(0,1125),
        angle_t::dms(0,1350),
        angle_t::dms(0,1575),
        angle_t::dms(0,1800),
        angle_t::dms(0,2025),
        angle_t::dms(0,2250),
        angle_t::dms(0,2475),
        angle_t::dms(0,2700),
        angle_t::dms(0,2925),
        angle_t::dms(0,3150),
        angle_t::dms(0,3375),
        angle_t::dms(0,3600),
        angle_t::dms(0,3825),
        angle_t::dms(0,4050),
        angle_t::dms(0,4275),
        angle_t::dms(0,4500),
        angle_t::dms(0,4725),
        angle_t::dms(0,4950),
        angle_t::dms(0,5175),
        angle_t::dms(0,5400)
    };

    static constexpr auto sines = std::array{
        double{0},
        double{225 / 3438.0},
        double{449 / 3438.0},
        double{671 / 3438.0},
        double{890 / 3438.0},
        double{1105 / 3438.0},
        double{1315 / 3438.0},
        double{1520 / 3438.0},
        double{1719 / 3438.0},
        double{1910 / 3438.0},
        double{2093 / 3438.0},
        double{2267 / 3438.0},
        double{2431 / 3438.0},
        double{2585 / 3438.0},
        double{2728 / 3438.0},
        double{2859 / 3438.0},
        double{2978 / 3438.0},
        double{3084 / 3438.0},
        double{3177 / 3438.0},
        double{3256 / 3438.0},
        double{3321 / 3438.0},
        double{3372 / 3438.0},
        double{3409 / 3438.0},
        double{3431 / 3438.0},
        double{3438 / 3438.0}
    };


    class hindu_sine_entry_test : public helpers::input_output_test<int32_t, double>{};

    TEST_P(hindu_sine_entry_test, calculate) {
        auto &[rd, output] = GetParam();
        const auto actual = calendars::modern_hindu_base::hindu_sine_table(rd);
        EXPECT_EQ(actual, output) << output << " : " << actual << " : " << rd << std::endl;
    }

    class hindu_sine_test : public helpers::input_output_test<angle_t, double>{};

    TEST_P(hindu_sine_test, calculate) {
        auto &[rd, output] = GetParam();
        const auto actual = calendars::modern_hindu_base::hindu_sine(rd);
        EXPECT_EQ(actual, output) << output << " : " << actual << " : " << rd << std::endl;
    }

    class hindu_arcsine_test : public helpers::input_output_test<double, angle_t>{};

    TEST_P(hindu_arcsine_test, calculate) {
        auto &[rd, output] = GetParam();
        const auto actual = calendars::modern_hindu_base::hindu_arcsin(rd);
        EXPECT_EQ(actual, output) << output << " : " << actual << " : " << rd << std::endl;
    }

    INSTANTIATE_TEST_CASE_P(hindu_sine_table, hindu_sine_entry_test, testing::ValuesIn(helpers::make_test_params_values(inputs, sines)));
    INSTANTIATE_TEST_CASE_P(hindu_sine, hindu_sine_test, testing::ValuesIn(helpers::make_test_params_values(angles, sines)));
    INSTANTIATE_TEST_CASE_P(hindu_arcsine, hindu_arcsine_test, testing::ValuesIn(helpers::make_test_params_values(sines, angles)));
}