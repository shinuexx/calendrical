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
        0.645260,
        0.146650,
        0.365595,
        0.582553,
        0.926722,
        0.391565,
        0.737227,
        0.434271,
        0.028119,
        0.501712,
        0.494050,
        0.013196,
        0.519306,
        0.259826,
        0.452190,
        0.343276,
        0.300431,
        0.231763,
        0.973015,
        0.394417,
        0.450518,
        0.416008,
        0.657391,
        0.686259,
        0.008243,
        0.916779,
        0.586590,
        0.742587,
        0.555246,
        0.719220,
        0.466211,
        0.705999,
        0.436806
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
            EXPECT_NEAR(actual.time(), output, 1e-6) << output << " : " << actual.time() << " : " << rd << std::endl;
        }
    }


    INSTANTIATE_TEST_CASE_P(moonrise_in_mecca, moonrise_in_mecca_test, testing::ValuesIn(helpers::make_test_params_values(dates, results)));
}