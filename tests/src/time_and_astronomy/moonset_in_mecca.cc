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
        0.084225,
        0.627607,
        0.842646,
        0.030307,
        0.419886,
        0.965784,
        0.252852,
        bogus,
        0.528119,
        0.052493,
        0.037996,
        0.493177,
        0.060322,
        0.856017,
        bogus,
        0.908706,
        0.818009,
        0.714185,
        0.416862,
        0.932503,
        0.956378,
        0.952629,
        0.070965,
        0.200419,
        0.489200,
        0.429962,
        0.031414,
        0.224521,
        bogus,
        0.214825,
        bogus,
        0.211858,
        0.963174
    };

    class moonset_in_mecca_test : public helpers::input_output_test<fixed_date_t, moment_t>{};

    TEST_P(moonset_in_mecca_test, calculate) {
        //GTEST_SKIP();
        constexpr location mecca = locations::mecca;
        auto &[rd, output] = GetParam();
        const auto actual = moonset(rd, mecca);
        if (is_bogus(output)){
            EXPECT_TRUE(is_bogus(actual)) << output << " : " << actual << " : " << rd << std::endl;
        } else {
            EXPECT_NEAR(actual.time(), output, 1e-6) << output << " : " << actual.time() << " : " << rd << std::endl;
        }
    }


    INSTANTIATE_TEST_CASE_P(moonset_in_mecca, moonset_in_mecca_test, testing::ValuesIn(helpers::make_test_params_values(dates, results)));
}