#include "sample_fixed_dates.hpp"
#include <gtest/gtest.h>

#include "time_and_astronomy.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::time_and_astronomy;

    static constexpr auto results = std::array<angle_t, 33>{
        -13.163184_deg,
        -7.281426_deg,
        -77.149900_deg,
        -30.401178_deg,
        71.848578_deg,
        -43.798579_deg,
        40.653204_deg,
        -40.278725_deg,
        29.611156_deg,
        -19.973178_deg,
        -23.740743_deg,
        30.956688_deg,
        -18.888690_deg,
        -32.161162_deg,
        -45.680919_deg,
        -50.292110_deg,
        -54.345305_deg,
        -34.566000_deg,
        44.131989_deg,
        -57.539862_deg,
        -62.082439_deg,
        -54.072091_deg,
        -16.120452_deg,
        23.864594_deg,
        32.950146_deg,
        72.691651_deg,
        -29.849481_deg,
        31.610644_deg,
        -42.219689_deg,
        28.647809_deg,
        -38.950553_deg,
        27.601977_deg,
        -54.854681_deg,
    };

    class lunar_altitude_test : public helpers::input_output_test<fixed_date_t, angle_t>{};

    TEST_P(lunar_altitude_test, calculate) {
        constexpr location loc = locations::mecca;
        auto &[rd, output] = GetParam();
        const moment_t input{ rd };
        EXPECT_NEAR(lunar_altitude(input, loc).degrees(), output.degrees(), 1e-6) << output << " : " << lunar_altitude(input, loc) << " : " << input << std::endl;
    }


    INSTANTIATE_TEST_SUITE_P(lunar_altitude, lunar_altitude_test, testing::ValuesIn(helpers::make_test_params_values(dates, results)));
}