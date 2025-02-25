#include <calendars/persian_calendar.hpp>

#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"

#include "time_and_astronomy.hpp"
#include "local_helpers.hpp"
#include "location.hpp"

namespace tests {
    using namespace calendrical::time_and_astronomy;

    static constexpr auto bogus = std::numeric_limits<double>::quiet_NaN();
    static constexpr auto is_bogus(double value) { return value <=> 0 == std::partial_ordering::unordered; }

    static constexpr auto results = std::array<moment_t, 33>{
        0.780556,
        0.697324,
        0.734889,
        0.728514,
        0.708788,
        0.774201,
        0.700877,
        0.762736,
        0.776777,
        0.748088,
        0.742536,
        0.753102,
        0.764402,
        0.784041,
        0.722535,
        0.756134,
        0.739658,
        0.739663,
        0.784057,
        0.787086,
        0.781969,
        0.786308,
        0.778132,
        0.749687,
        0.760231,
        0.761909,
        0.730647,
        0.760390,
        0.724012,
        0.745361,
        0.734750,
        0.699821,
        0.784480
    };

    class sunset_in_jerusalem_test : public helpers::input_output_test<fixed_date_t, moment_t>{};

    TEST_P(sunset_in_jerusalem_test, calculate) {
        //GTEST_SKIP();
        constexpr location jerusalem = locations::jerusalem;
        auto &[rd, output] = GetParam();
        const auto actual = sunset(rd, jerusalem);
        if (is_bogus(output)){
            EXPECT_TRUE(is_bogus(actual)) << output << " : " << actual << " : " << rd << std::endl;
        } else {
            EXPECT_NEAR(actual.time(), output, 1e-6) << output << " : " << actual.time() << " : " << rd << std::endl;
        }
    }


    INSTANTIATE_TEST_CASE_P(sunset_in_jerusalem, sunset_in_jerusalem_test, testing::ValuesIn(helpers::make_test_params_values(dates, results)));
}