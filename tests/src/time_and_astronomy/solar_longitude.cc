#include "sample_fixed_dates.hpp"
#include <gtest/gtest.h>

#include "time_and_astronomy.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::time_and_astronomy;

    static constexpr auto results = std::array<angle_t, 33>{
        119.473431_deg,
        254.248961_deg,
        181.435996_deg,
        188.663922_deg,
        289.091566_deg,
        59.119741_deg,
        228.314554_deg,
        34.460769_deg,
        63.187995_deg,
        2.457591_deg,
        350.475934_deg,
        13.498220_deg,
        37.403920_deg,
        81.028130_deg,
        313.860498_deg,
        19.954430_deg,
        176.059431_deg,
        344.922951_deg,
        79.964921_deg,
        99.302317_deg,
        121.535304_deg,
        88.567428_deg,
        129.289884_deg,
        6.146910_deg,
        28.251993_deg,
        151.780633_deg,
        185.945867_deg,
        28.555607_deg,
        193.347892_deg,
        357.151254_deg,
        336.170692_deg,
        228.184879_deg,
        116.439352_deg
    };

    class solar_longitude_test : public helpers::input_output_test<fixed_date_t, angle_t>{};

    TEST_P(solar_longitude_test, calculate) {
        auto &[rd, output] = GetParam();
        const moment_t input = moment_t{ rd } + 0.5;
        EXPECT_NEAR(solar_longitude(input).degrees(), output.degrees(), 1e-6) << output << " : " << solar_longitude(input) << " : " << input << std::endl;
    }


    INSTANTIATE_TEST_SUITE_P(solar_longitude, solar_longitude_test, testing::ValuesIn(helpers::make_test_params_values(dates, results)));
}