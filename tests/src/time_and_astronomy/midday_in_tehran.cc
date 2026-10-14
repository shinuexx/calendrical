#include <calendars/persian_calendar.hpp>

#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"

#include "time_and_astronomy.hpp"
#include "local_helpers.hpp"
#include "location.hpp"

namespace tests {
    using namespace calendrical::time_and_astronomy;

    static constexpr auto is_bogus(moment_t value) { return static_cast<double>(value) <=> 0 == std::partial_ordering::unordered; }

    static constexpr auto results = std::array<moment_t, 33>{
        0.504216_m,
        0.499952_m,
        0.497556_m,
        0.496143_m,
        0.510331_m,
        0.498595_m,
        0.493137_m,
        0.500958_m,
        0.499538_m,
        0.507865_m,
        0.510516_m,
        0.505289_m,
        0.500814_m,
        0.501853_m,
        0.513383_m,
        0.503941_m,
        0.498637_m,
        0.511368_m,
        0.502105_m,
        0.505088_m,
        0.507082_m,
        0.503594_m,
        0.507129_m,
        0.506931_m,
        0.502551_m,
        0.504488_m,
        0.496387_m,
        0.502516_m,
        0.494746_m,
        0.508771_m,
        0.512176_m,
        0.491786_m,
        0.507483_m
    };

    class midday_in_tehran_test : public helpers::input_output_test<fixed_date_t, moment_t>{};

    TEST_P(midday_in_tehran_test, calculate) {
        constexpr location tehran = calendars::persian_based_calendar::tehran;
        auto &[rd, output] = GetParam();
        const auto actual = standard_from_universal(midday(rd, tehran), tehran);
        if (is_bogus(output)){
            EXPECT_TRUE(is_bogus(actual)) << output << " : " << actual << " : " << rd << std::endl;
        } else {
            EXPECT_NEAR(actual.time(), output.time(), 1e-6) << output << " : " << actual.time() << " : " << rd << std::endl;
        }
    }


    INSTANTIATE_TEST_SUITE_P(midday_in_tehran, midday_in_tehran_test, testing::ValuesIn(helpers::make_test_params_values(dates, results)));
}