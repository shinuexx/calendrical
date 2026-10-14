#include "sample_fixed_dates.hpp"
#include <gtest/gtest.h>

#include "time_and_astronomy.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::time_and_astronomy;

    static constexpr auto results= std::array{
        0.214169,
        0.143632,
        0.114444,
        0.107183,
        0.069498,
        0.057506,
        0.044758,
        0.017397,
        0.012796,
        0.008869,
        0.007262,
        0.005979,
        0.005740,
        0.003875,
        0.003157,
        0.002393,
        0.001731,
        0.001669,
        0.000615,
        0.000177,
        0.000101,
        0.000171,
        0.000136,
        0.000061,
        0.000014,
        0.000276,
        0.000296,
        0.000302,
        0.000302,
        0.000675,
        0.000712,
        0.000963,
        0.002913
    };

    class ephemeris_correction_test : public helpers::input_output_test<fixed_date_t, double>{};

    TEST_P(ephemeris_correction_test, calculate) {
        auto &[input, output] = GetParam();
        EXPECT_NEAR(ephemeris_correction(moment_t{ input }), output, 1e-6) << output << " : " << ephemeris_correction(moment_t{ input }) << " : " << input << std::endl;
    }


    INSTANTIATE_TEST_SUITE_P(ephemeris_correction, ephemeris_correction_test, testing::ValuesIn(helpers::make_test_params_values(dates, results)));
}