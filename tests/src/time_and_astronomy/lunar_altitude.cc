#include "sample_fixed_dates.hpp"
#include <gtest/gtest.h>

#include "time_and_astronomy.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::time_and_astronomy;

    static constexpr auto results = std::array<angle_t, 33>{
        -13.163184,
        -7.281426,
        -77.149900,
        -30.401178,
        71.848578,
        -43.798579,
        40.653204,
        -40.278725,
        29.611156,
        -19.973178,
        -23.740743,
        30.956688,
        -18.888690,
        -32.161162,
        -45.680919,
        -50.292110,
        -54.345305,
        -34.566000,
        44.131989,
        -57.539862,
        -62.082439,
        -54.072091,
        -16.120452,
        23.864594,
        32.950146,
        72.691651,
        -29.849481,
        31.610644,
        -42.219689,
        28.647809,
        -38.950553,
        27.601977,
        -54.854681,
    };

    class lunar_altitude_test : public helpers::input_output_test<fixed_date_t, angle_t>{};

    TEST_P(lunar_altitude_test, calculate) {
        constexpr location loc = locations::mecca;
        auto &[rd, output] = GetParam();
        const moment_t input = rd;
        EXPECT_NEAR(lunar_altitude(input, loc), output, 1e-6) << output << " : " << lunar_altitude(input, loc) << " : " << input << std::endl;
    }


    INSTANTIATE_TEST_CASE_P(lunar_altitude, lunar_altitude_test, testing::ValuesIn(helpers::make_test_params_values(dates, results)));
}