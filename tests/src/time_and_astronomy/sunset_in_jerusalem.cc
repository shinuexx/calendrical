#include <calendars/persian_calendar.hpp>

#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"

#include "time_and_astronomy.hpp"
#include "local_helpers.hpp"
#include "location.hpp"

namespace tests {
    using namespace calendrical::time_and_astronomy;

    static constexpr auto bogus = moment_t{ std::numeric_limits<double>::quiet_NaN() };
    static constexpr auto is_bogus(moment_t value) { return static_cast<double>(value) <=> 0 == std::partial_ordering::unordered; }

    static constexpr auto results = std::array<moment_t, 33>{
        0.780556_m,
        0.697324_m,
        0.734889_m,
        0.728514_m,
        0.708788_m,
        0.774201_m,
        0.700877_m,
        0.762736_m,
        0.776777_m,
        0.748088_m,
        0.742536_m,
        0.753102_m,
        0.764402_m,
        0.784041_m,
        0.722535_m,
        0.756134_m,
        0.739658_m,
        0.739663_m,
        0.784057_m,
        0.787086_m,
        0.781969_m,
        0.786308_m,
        0.778132_m,
        0.749687_m,
        0.760231_m,
        0.761909_m,
        0.730647_m,
        0.760390_m,
        0.724012_m,
        0.745361_m,
        0.734750_m,
        0.699821_m,
        0.784480_m
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
            EXPECT_NEAR(actual.time(), output.time(), 1e-6) << output << " : " << actual.time() << " : " << rd << std::endl;
        }
    }


    INSTANTIATE_TEST_CASE_P(sunset_in_jerusalem, sunset_in_jerusalem_test, testing::ValuesIn(helpers::make_test_params_values(dates, results)));
}