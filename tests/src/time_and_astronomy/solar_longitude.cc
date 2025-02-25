#include "sample_fixed_dates.hpp"
#include <gtest/gtest.h>

#include "time_and_astronomy.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::time_and_astronomy;

    static constexpr auto results = std::array<angle_t, 33>{
        119.473431,
        254.248961,
        181.435996,
        188.663922,
        289.091566,
        59.119741,
        228.314554,
        34.460769,
        63.187995,
        2.457591,
        350.475934,
        13.498220,
        37.403920,
        81.028130,
        313.860498,
        19.954430,
        176.059431,
        344.922951,
        79.964921,
        99.302317,
        121.535304,
        88.567428,
        129.289884,
        6.146910,
        28.251993,
        151.780633,
        185.945867,
        28.555607,
        193.347892,
        357.151254,
        336.170692,
        228.184879,
        116.439352
    };

    class solar_longitude_test : public helpers::input_output_test<fixed_date_t, angle_t>{};

    TEST_P(solar_longitude_test, calculate) {
        auto &[rd, output] = GetParam();
        const moment_t input = moment_t{0.5}.add(rd);
        EXPECT_NEAR(solar_longitude(input), output, 1e-6) << output << " : " << solar_longitude(input) << " : " << input << std::endl;
    }


    INSTANTIATE_TEST_CASE_P(solar_longitude, solar_longitude_test, testing::ValuesIn(helpers::make_test_params_values(dates, results)));
}