#include "sample_fixed_dates.hpp"
#include <gtest/gtest.h>

#include "time_and_astronomy.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::time_and_astronomy;

    static constexpr auto results = std::array<angle_t, 33>{
        2.452759_deg,
        -4.902230_deg,
        -2.939469_deg,
        5.001904_deg,
        -3.208909_deg,
        0.894361_deg,
        -3.863335_deg,
        -2.522444_deg,
        1.032069_deg,
        3.005689_deg,
        1.613843_deg,
        4.766741_deg,
        4.899203_deg,
        4.838474_deg,
        2.301475_deg,
        -0.890563_deg,
        4.765784_deg,
        -2.737358_deg,
        -4.035652_deg,
        -3.157214_deg,
        -1.879614_deg,
        -3.379519_deg,
        -4.398341_deg,
        2.099198_deg,
        5.268746_deg,
        -1.672299_deg,
        4.682012_deg,
        3.705518_deg,
        2.493964_deg,
        -4.167774_deg,
        -2.873757_deg,
        -4.667251_deg,
        5.138562_deg
    };

    class lunar_latitude_test : public helpers::input_output_test<fixed_date_t, angle_t>{};

    TEST_P(lunar_latitude_test, calculate) {
        auto &[rd, output] = GetParam();
        const moment_t input{ rd };
        EXPECT_NEAR(lunar_latitude(input), output, 1e-6) << output << " : " << lunar_latitude(input) << " : " << input << std::endl;
    }


    INSTANTIATE_TEST_CASE_P(lunar_latitude, lunar_latitude_test, testing::ValuesIn(helpers::make_test_params_values(dates, results)));
}