#include "sample_fixed_dates.hpp"
#include <gtest/gtest.h>

#include "primitives.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::primitives;

    static constexpr std::array expected_julian_days{
        1507231.5,
        1660037.5,
        1746893.5,
        1770641.5,
        1892731.5,
        1931579.5,
        1974851.5,
        2091164.5,
        2121509.5,
        2155779.5,
        2174029.5,
        2191584.5,
        2195261.5,
        2229274.5,
        2245580.5,
        2266100.5,
        2288542.5,
        2290901.5,
        2323140.5,
        2334848.5,
        2348020.5,
        2366978.5,
        2385648.5,
        2392825.5,
        2416223.5,
        2425848.5,
        2430266.5,
        2430833.5,
        2431004.5,
        2448698.5,
        2450138.5,
        2465737.5,
        2486076.5
    };

    static constexpr std::array<int64_t, 33> expected_modified_julian_days{
        -892769,
        -739963,
        -653107,
        -629359,
        -507269,
        -468421,
        -425149,
        -308836,
        -278491,
        -244221,
        -225971,
        -208416,
        -204739,
        -170726,
        -154420,
        -133900,
        -111458,
        -109099,
        -76860,
        -65152,
        -51980,
        -33022,
        -14352,
        -7175,
        16223,
        25848,
        30266,
        30833,
        31004,
        48698,
        50138,
        65737,
        86076
    };

    static constexpr std::array<double, 33> expected_unix_time{
        -80641958400,
        -67439520000,
        -59935161600,
        -57883334400,
        -47334758400,
        -43978291200,
        -40239590400,
        -30190147200,
        -27568339200,
        -24607411200,
        -23030611200,
        -21513859200,
        -21196166400,
        -18257443200,
        -16848604800,
        -15075676800,
        -13136688000,
        -12932870400,
        -10147420800,
        -9135849600,
        -7997788800,
        -6359817600,
        -4746729600,
        -4126636800,
        -2105049600,
        -1273449600,
        -891734400,
        -842745600,
        -827971200,
        700790400,
        825206400,
        2172960000,
        3930249600,
    };

    class julian_days_test : public helpers::input_output_test<fixed_date_t, double>{};
    class modified_julian_days_test : public helpers::input_output_test<fixed_date_t, int64_t>{};
    class unix_time_test : public helpers::input_output_test<fixed_date_t, double>{};

    TEST_P(julian_days_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_DOUBLE_EQ(moment_from_jd(output), input) << output << " : " << moment_from_jd(output) << " : " << input << std::endl;
    }
    TEST_P(modified_julian_days_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(fixed_from_mjd(output), input) << output << " : " << fixed_from_mjd(output) << " : " << input << std::endl;
    }
    TEST_P(unix_time_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_DOUBLE_EQ(moment_from_unix(output), input) << output << " : " << moment_from_unix(output) << " : " << input << std::endl;
    }

    TEST_P(julian_days_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_DOUBLE_EQ(jd_from_moment(input), output) << input << " : " << jd_from_moment(input) << " : " << output << std::endl;
    }
    TEST_P(modified_julian_days_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(mjd_from_fixed(input), output) << input << " : " << mjd_from_fixed(input) << " : " << output << std::endl;
    }
    TEST_P(unix_time_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_DOUBLE_EQ(unix_from_moment(input), output) << input << " : " << unix_from_moment(input) << " : " << output << std::endl;
    }

    INSTANTIATE_TEST_CASE_P(julian_days, julian_days_test, testing::ValuesIn(helpers::make_test_params_values(dates, expected_julian_days)));
    INSTANTIATE_TEST_CASE_P(modified_julian_days, modified_julian_days_test, testing::ValuesIn(helpers::make_test_params_values(dates, expected_modified_julian_days)));
    INSTANTIATE_TEST_CASE_P(unix_time, unix_time_test, testing::ValuesIn(helpers::make_test_params_values(dates, expected_unix_time)));

}