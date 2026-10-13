#include "sample_fixed_dates.hpp"
#include <gtest/gtest.h>

#include "time_and_astronomy.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::time_and_astronomy;

    static constexpr auto results = std::array{
        -0.001190,
        0.003159,
        0.005373,
        0.006791,
        -0.007231,
        0.004410,
        0.009897,
        0.001987,
        0.003478,
        -0.004947,
        -0.007588,
        -0.002369,
        0.002140,
        0.001195,
        -0.010356,
        -0.001013,
        0.004277,
        -0.008429,
        0.000943,
        -0.002039,
        -0.004076,
        -0.000541,
        -0.004143,
        -0.004008,
        0.000392,
        -0.001555,
        0.006529,
        0.000427,
        0.008180,
        -0.005843,
        -0.009214,
        0.011238,
        -0.004466
    };

    class equation_of_time_test : public helpers::input_output_test<fixed_date_t, double>{};

    TEST_P(equation_of_time_test, calculate) {
        auto &[input, output] = GetParam();
        EXPECT_NEAR(equation_of_time(moment_t{ input }), output, 1e-6) << output << " : " << equation_of_time(moment_t{ input }) << " : " << input << std::endl;
    }


    INSTANTIATE_TEST_CASE_P(equation_of_time, equation_of_time_test, testing::ValuesIn(helpers::make_test_params_values(dates, results)));
}