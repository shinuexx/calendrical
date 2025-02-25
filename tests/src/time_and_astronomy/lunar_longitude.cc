#include "sample_fixed_dates.hpp"
#include <gtest/gtest.h>

#include "time_and_astronomy.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::time_and_astronomy;

    static constexpr auto results = std::array<angle_t, 33>{
        244.853905,
        208.856738,
        213.746842,
        292.046243,
        156.819014,
        108.055632,
        39.356097,
        98.565851,
        332.958296,
        92.259651,
        78.132029,
        274.946995,
        128.362844,
        89.518450,
        24.607322,
        53.485956,
        187.898520,
        320.172362,
        314.042566,
        145.474065,
        185.030507,
        142.189132,
        253.743375,
        151.648685,
        287.987743,
        25.626707,
        290.288300,
        189.913142,
        284.931730,
        152.339044,
        51.662265,
        26.682060,
        175.500822
    };

    class lunar_longitude_test : public helpers::input_output_test<fixed_date_t, angle_t>{};

    TEST_P(lunar_longitude_test, calculate) {
        auto &[rd, output] = GetParam();
        const moment_t input = rd;
        EXPECT_NEAR(lunar_longitude(input), output, 1e-6) << output << " : " << lunar_longitude(input) << " : " << input << std::endl;
    }


    INSTANTIATE_TEST_CASE_P(lunar_longitude, lunar_longitude_test, testing::ValuesIn(helpers::make_test_params_values(dates, results)));
}