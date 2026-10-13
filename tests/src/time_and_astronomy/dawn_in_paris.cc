#include <calendars/french_calendar.hpp>


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
        0.095285_m,
        0.277372_m,
        0.203569_m,
        0.212231_m,
        0.286370_m,
        0.096280_m,
        0.253730_m,
        0.149472_m,
        0.088486_m,
        0.209217_m,
        0.228552_m,
        0.189859_m,
        0.143920_m,
        bogus,
        0.272361_m,
        0.178067_m,
        0.196839_m,
        0.236578_m,
        0.045748_m,
        bogus,
        0.105595_m,
        bogus,
        0.122462_m,
        0.202856_m,
        0.162579_m,
        0.163289_m,
        0.208698_m,
        0.162020_m,
        0.216912_m,
        0.217687_m,
        0.247980_m,
        0.251890_m,
        0.094955_m
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
            EXPECT_NEAR(actual.time(), output.time(), 1e-6) << output << " : " << actual.time() << " : " << rd << std::endl;
        }
    }


    INSTANTIATE_TEST_CASE_P(dawn_in_paris, dawn_in_paris_test, testing::ValuesIn(helpers::make_test_params_values(dates, results)));
}