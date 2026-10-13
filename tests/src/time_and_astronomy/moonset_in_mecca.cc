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
        0.084225_m,
        0.627607_m,
        0.842646_m,
        0.030307_m,
        0.419886_m,
        0.965784_m,
        0.252852_m,
        bogus,
        0.528119_m,
        0.052493_m,
        0.037996_m,
        0.493177_m,
        0.060322_m,
        0.856017_m,
        bogus,
        0.908706_m,
        0.818009_m,
        0.714185_m,
        0.416862_m,
        0.932503_m,
        0.956378_m,
        0.952629_m,
        0.070965_m,
        0.200419_m,
        0.489200_m,
        0.429962_m,
        0.031414_m,
        0.224521_m,
        bogus,
        0.214825_m,
        bogus,
        0.211858_m,
        0.963174_m
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
            EXPECT_NEAR(actual.time(), output.time(), 1e-6) << output << " : " << actual.time() << " : " << rd << std::endl;
        }
    }


    INSTANTIATE_TEST_CASE_P(moonset_in_mecca, moonset_in_mecca_test, testing::ValuesIn(helpers::make_test_params_values(dates, results)));
}