#include "sample_fixed_dates.hpp"
#include <gtest/gtest.h>

#include "time_and_astronomy.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::time_and_astronomy;

    static constexpr auto results = std::array<angle_t, 33>{
        244.853905_deg,
        208.856738_deg,
        213.746842_deg,
        292.046243_deg,
        156.819014_deg,
        108.055632_deg,
        39.356097_deg,
        98.565851_deg,
        332.958296_deg,
        92.259651_deg,
        78.132029_deg,
        274.946995_deg,
        128.362844_deg,
        89.518450_deg,
        24.607322_deg,
        53.485956_deg,
        187.898520_deg,
        320.172362_deg,
        314.042566_deg,
        145.474065_deg,
        185.030507_deg,
        142.189132_deg,
        253.743375_deg,
        151.648685_deg,
        287.987743_deg,
        25.626707_deg,
        290.288300_deg,
        189.913142_deg,
        284.931730_deg,
        152.339044_deg,
        51.662265_deg,
        26.682060_deg,
        175.500822_deg
    };

    class lunar_longitude_test : public helpers::input_output_test<fixed_date_t, angle_t>{};

    TEST_P(lunar_longitude_test, calculate) {
        auto &[rd, output] = GetParam();
        const moment_t input{ rd };
        EXPECT_NEAR(lunar_longitude(input).degrees(), output.degrees(), 1e-6) << output << " : " << lunar_longitude(input) << " : " << input << std::endl;
    }


    INSTANTIATE_TEST_SUITE_P(lunar_longitude, lunar_longitude_test, testing::ValuesIn(helpers::make_test_params_values(dates, results)));
}