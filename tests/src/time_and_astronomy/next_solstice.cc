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
        -214131.147334,
        -61371.053052,
        25556.789090,
        49297.584568,
        171378.539292,
        210187.908137,
        253468.413001,
        369798.559125,
        400113.618511,
        434446.328561,
        452615.134851,
        470240.017391,
        473892.432571,
        507859.911980,
        524202.622400,
        544749.318739,
        567122.512168,
        569492.673245,
        601727.018725,
        613508.259061,
        626656.970915,
        645556.001954,
        664276.907718,
        671488.166885,
        694863.628311,
        704453.536160,
        708926.239052,
        709473.300039,
        709656.728163,
        727277.366581,
        728738.335351,
        744354.792807,
        764718.468836
    };

    class next_solstice_test : public helpers::input_output_test<fixed_date_t, moment_t>{};

    TEST_P(next_solstice_test, calculate) {
        auto &[rd, output] = GetParam();
        const auto actual = std::min({solar_longitude_after(season::spring, rd),
            solar_longitude_after(season::summer, rd),
            solar_longitude_after(season::autumn, rd),
            solar_longitude_after(season::winter, rd)});
        if (is_bogus(output)){
            EXPECT_TRUE(is_bogus(actual)) << output << " : " << actual<< " : " << rd << std::endl;
        } else {
            EXPECT_NEAR(actual, output, 1e-6) << output << " : " << actual << " : " << rd << std::endl;
        }
    }


    INSTANTIATE_TEST_CASE_P(next_solstice, next_solstice_test, testing::ValuesIn(helpers::make_test_params_values(dates, results)));
}