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
        0.504216,
        0.499952,
        0.497556,
        0.496143,
        0.510331,
        0.498595,
        0.493137,
        0.500958,
        0.499538,
        0.507865,
        0.510516,
        0.505289,
        0.500814,
        0.501853,
        0.513383,
        0.503941,
        0.498637,
        0.511368,
        0.502105,
        0.505088,
        0.507082,
        0.503594,
        0.507129,
        0.506931,
        0.502551,
        0.504488,
        0.496387,
        0.502516,
        0.494746,
        0.508771,
        0.512176,
        0.491786,
        0.507483
    };

    class midday_in_tehran_test : public helpers::input_output_test<fixed_date_t, moment_t>{};

    TEST_P(midday_in_tehran_test, calculate) {
        constexpr location tehran = calendars::persian_based_calendar::tehran;
        auto &[rd, output] = GetParam();
        const auto actual = standard_from_universal(midday(rd, tehran), tehran);
        if (is_bogus(output)){
            EXPECT_TRUE(is_bogus(actual)) << output << " : " << actual << " : " << rd << std::endl;
        } else {
            EXPECT_NEAR(actual.time(), output, 1e-6) << output << " : " << actual.time() << " : " << rd << std::endl;
        }
    }


    INSTANTIATE_TEST_CASE_P(midday_in_tehran, midday_in_tehran_test, testing::ValuesIn(helpers::make_test_params_values(dates, results)));
}