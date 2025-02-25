#include <calendars/french_calendar.hpp>


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
        0.095285,
        0.277372,
        0.203569,
        0.212231,
        0.286370,
        0.096280,
        0.253730,
        0.149472,
        0.088486,
        0.209217,
        0.228552,
        0.189859,
        0.143920,
        bogus,
        0.272361,
        0.178067,
        0.196839,
        0.236578,
        0.045748,
        bogus,
        0.105595,
        bogus,
        0.122462,
        0.202856,
        0.162579,
        0.163289,
        0.208698,
        0.162020,
        0.216912,
        0.217687,
        0.247980,
        0.251890,
        0.094955
    };

    class dawn_in_paris_test : public helpers::input_output_test<fixed_date_t, moment_t>{};

    TEST_P(dawn_in_paris_test, calculate) {
        constexpr auto paris = calendars::french_based_calendar::paris;
        constexpr auto alpha = angle_t::dms(18);
        auto &[rd, output] = GetParam();
        const auto actual = dawn(rd, paris, alpha);
        if (is_bogus(output)){
            EXPECT_TRUE(is_bogus(actual)) << output << " : " << actual<< " : " << rd << std::endl;
        } else {
            EXPECT_NEAR(actual.time(), output, 1e-6) << output << " : " << actual.time() << " : " << rd << std::endl;
        }
    }


    INSTANTIATE_TEST_CASE_P(dawn_in_paris, dawn_in_paris_test, testing::ValuesIn(helpers::make_test_params_values(dates, results)));
}