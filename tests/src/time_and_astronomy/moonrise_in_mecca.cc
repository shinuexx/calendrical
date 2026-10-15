#include <calendars/persian_calendar.hpp>

#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"

#include "time_and_astronomy.hpp"
#include "local_helpers.hpp"
#include "location.hpp"

namespace tests {
    using namespace calendrical::time_and_astronomy;

    static constexpr auto is_bogus(moment_t value) { return value.days() <=> 0 == std::partial_ordering::unordered; }

    static constexpr auto results = std::array<moment_t, 33>{
        0.645260_m,
        0.146650_m,
        0.365595_m,
        0.582553_m,
        0.926722_m,
        0.391565_m,
        0.737227_m,
        0.434271_m,
        0.028119_m,
        0.501712_m,
        0.494050_m,
        0.013196_m,
        0.519306_m,
        0.259826_m,
        0.452190_m,
        0.343276_m,
        0.300431_m,
        0.231763_m,
        0.973015_m,
        0.394417_m,
        0.450518_m,
        0.416008_m,
        0.657391_m,
        0.686259_m,
        0.008243_m,
        0.916779_m,
        0.586590_m,
        0.742587_m,
        0.555246_m,
        0.719220_m,
        0.466211_m,
        0.705999_m,
        0.436806_m
    };

    class moonrise_in_mecca_test : public helpers::input_output_test<fixed_date_t, moment_t>{};

    TEST_P(moonrise_in_mecca_test, calculate) {
        //GTEST_SKIP();
        constexpr location mecca = locations::mecca;
        auto &[rd, output] = GetParam();
        const auto actual = moonrise(rd, mecca);
        if (is_bogus(output)){
            EXPECT_TRUE(is_bogus(actual)) << output << " : " << actual << " : " << rd << std::endl;
        } else {
            EXPECT_NEAR(actual.time(), output.time(), 1e-6) << output << " : " << actual.time() << " : " << rd << std::endl;
        }
    }


    INSTANTIATE_TEST_SUITE_P(moonrise_in_mecca, moonrise_in_mecca_test, testing::ValuesIn(helpers::make_test_params_values(dates, results)));
}