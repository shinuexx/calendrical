#include <calendars/french_calendar.hpp>


#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"

#include "time_and_astronomy.hpp"
#include "local_helpers.hpp"
#include "location.hpp"

namespace tests {
    using namespace calendrical::time_and_astronomy;

    static constexpr auto is_bogus(moment_t value) { return value.days() <=> 0 == std::partial_ordering::unordered; }

    static constexpr auto results = std::array<moment_t, 33>{
        -214131.147334_m,
        -61371.053052_m,
        25556.789090_m,
        49297.584568_m,
        171378.539292_m,
        210187.908137_m,
        253468.413001_m,
        369798.559125_m,
        400113.618511_m,
        434446.328561_m,
        452615.134851_m,
        470240.017391_m,
        473892.432571_m,
        507859.911980_m,
        524202.622400_m,
        544749.318739_m,
        567122.512168_m,
        569492.673245_m,
        601727.018725_m,
        613508.259061_m,
        626656.970915_m,
        645556.001954_m,
        664276.907718_m,
        671488.166885_m,
        694863.628311_m,
        704453.536160_m,
        708926.239052_m,
        709473.300039_m,
        709656.728163_m,
        727277.366581_m,
        728738.335351_m,
        744354.792807_m,
        764718.468836_m
    };

    class next_solstice_test : public helpers::input_output_test<fixed_date_t, moment_t>{};

    TEST_P(next_solstice_test, calculate) {
        auto &[rd, output] = GetParam();
        const auto actual = std::min({solar_longitude_after(season::spring, moment_t{ rd }),
            solar_longitude_after(season::summer, moment_t{ rd }),
            solar_longitude_after(season::autumn, moment_t{ rd }),
            solar_longitude_after(season::winter, moment_t{ rd })});
        if (is_bogus(output)){
            EXPECT_TRUE(is_bogus(actual)) << output << " : " << actual<< " : " << rd << std::endl;
        } else {
            EXPECT_NEAR(actual.days(), output.days(), 1e-6) << output << " : " << actual << " : " << rd << std::endl;
        }
    }


    INSTANTIATE_TEST_SUITE_P(next_solstice, next_solstice_test, testing::ValuesIn(helpers::make_test_params_values(dates, results)));
}