#include "sample_fixed_dates.hpp"
#include <gtest/gtest.h>

#include "time_and_astronomy.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::time_and_astronomy;

    static constexpr auto results = std::array<angle_t, 33>{
        2.452759,
        -4.902230,
        -2.939469,
        5.001904,
        -3.208909,
        0.894361,
        -3.863335,
        -2.522444,
        1.032069,
        3.005689,
        1.613843,
        4.766741,
        4.899203,
        4.838474,
        2.301475,
        -0.890563,
        4.765784,
        -2.737358,
        -4.035652,
        -3.157214,
        -1.879614,
        -3.379519,
        -4.398341,
        2.099198,
        5.268746,
        -1.672299,
        4.682012,
        3.705518,
        2.493964,
        -4.167774,
        -2.873757,
        -4.667251,
        5.138562
    };

    class lunar_latitude_test : public helpers::input_output_test<fixed_date_t, angle_t>{};

    TEST_P(lunar_latitude_test, calculate) {
        auto &[rd, output] = GetParam();
        const moment_t input = rd;
        EXPECT_NEAR(lunar_latitude(input), output, 1e-6) << output << " : " << lunar_latitude(input) << " : " << input << std::endl;
    }


    INSTANTIATE_TEST_CASE_P(lunar_latitude, lunar_latitude_test, testing::ValuesIn(helpers::make_test_params_values(dates, results)));
}