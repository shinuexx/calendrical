#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"
#include "local_helpers.hpp"

#include "holiday_providers/gregorian_calendar_holiday_provider.hpp"
#include "holiday_providers/julian_calendar_holiday_provider.hpp"

namespace tests{
    using namespace calendrical::calendars;

    static const std::array orthodox{
        gregorian_date{-586, month{4}, 3},
        gregorian_date{-168, month{4}, 1},
        gregorian_date{70, month{4}, 13},
        gregorian_date{135, month{4}, 17},
        gregorian_date{470, month{4}, 6},
        gregorian_date{576, month{4}, 7},
        gregorian_date{694, month{4}, 22},
        gregorian_date{1013, month{4}, 11},
        gregorian_date{1096, month{4}, 19},
        gregorian_date{1190, month{4}, 1},
        gregorian_date{1240, month{4}, 22},
        gregorian_date{1288, month{4}, 4},
        gregorian_date{1298, month{4}, 13},
        gregorian_date{1391, month{4}, 3},
        gregorian_date{1436, month{4}, 17},
        gregorian_date{1492, month{5}, 1},
        gregorian_date{1553, month{4}, 12},
        gregorian_date{1560, month{4}, 24},
        gregorian_date{1648, month{4}, 12},
        gregorian_date{1680, month{4}, 21},
        gregorian_date{1716, month{4}, 12},
        gregorian_date{1768, month{4}, 10},
        gregorian_date{1819, month{4}, 18},
        gregorian_date{1839, month{4}, 7},
        gregorian_date{1903, month{4}, 19},
        gregorian_date{1929, month{5}, 5},
        gregorian_date{1941, month{4}, 20},
        gregorian_date{1943, month{4}, 25},
        gregorian_date{1943, month{4}, 25},
        gregorian_date{1992, month{4}, 26},
        gregorian_date{1996, month{4}, 14},
        gregorian_date{2038, month{4}, 25},
        gregorian_date{2094, month{4}, 11}
    };

    static const std::array gregorian{
        gregorian_date{-586, month{4}, 3},
        gregorian_date{-168, month{4}, 8},
        gregorian_date{70, month{4}, 13},
        gregorian_date{135, month{4}, 17},
        gregorian_date{470, month{4}, 6},
        gregorian_date{576, month{4}, 7},
        gregorian_date{694, month{4}, 22},
        gregorian_date{1013, month{4}, 11},
        gregorian_date{1096, month{4}, 19},
        gregorian_date{1190, month{4}, 1},
        gregorian_date{1240, month{4}, 22},
        gregorian_date{1288, month{3}, 28},
        gregorian_date{1298, month{4}, 6},
        gregorian_date{1391, month{4}, 3},
        gregorian_date{1436, month{4}, 17},
        gregorian_date{1492, month{3}, 27},
        gregorian_date{1553, month{4}, 12},
        gregorian_date{1560, month{3}, 27},
        gregorian_date{1648, month{4}, 12},
        gregorian_date{1680, month{4}, 21},
        gregorian_date{1716, month{4}, 12},
        gregorian_date{1768, month{4}, 3},
        gregorian_date{1819, month{4}, 11},
        gregorian_date{1839, month{3}, 31},
        gregorian_date{1903, month{4}, 12},
        gregorian_date{1929, month{3}, 31},
        gregorian_date{1941, month{4}, 13},
        gregorian_date{1943, month{4}, 25},
        gregorian_date{1943, month{4}, 25},
        gregorian_date{1992, month{4}, 19},
        gregorian_date{1996, month{4}, 7},
        gregorian_date{2038, month{4}, 25},
        gregorian_date{2094, month{4}, 4}
    };

    static const std::array astronomical{
        gregorian_date{-586, month{4}, 3},
        gregorian_date{-168, month{4}, 8},
        gregorian_date{70, month{4}, 13},
        gregorian_date{135, month{4}, 17},
        gregorian_date{470, month{4}, 6},
        gregorian_date{576, month{4}, 7},
        gregorian_date{694, month{4}, 22},
        gregorian_date{1013, month{4}, 11},
        gregorian_date{1096, month{4}, 19},
        gregorian_date{1190, month{4}, 1},
        gregorian_date{1240, month{4}, 22},
        gregorian_date{1288, month{3}, 28},
        gregorian_date{1298, month{4}, 6},
        gregorian_date{1391, month{4}, 3},
        gregorian_date{1436, month{4}, 17},
        gregorian_date{1492, month{3}, 27},
        gregorian_date{1553, month{4}, 12},
        gregorian_date{1560, month{3}, 27},
        gregorian_date{1648, month{4}, 12},
        gregorian_date{1680, month{4}, 21},
        gregorian_date{1716, month{4}, 12},
        gregorian_date{1768, month{4}, 3},
        gregorian_date{1819, month{4}, 11},
        gregorian_date{1839, month{3}, 31},
        gregorian_date{1903, month{4}, 19}, // 12
        gregorian_date{1929, month{3}, 31},
        gregorian_date{1941, month{4}, 13},
        gregorian_date{1943, month{3}, 28}, // 4,25
        gregorian_date{1943, month{3}, 28}, // 4,25
        gregorian_date{1992, month{4}, 19},
        gregorian_date{1996, month{4}, 7},
        gregorian_date{2038, month{3}, 28}, // 4,25
        gregorian_date{2094, month{4}, 4}
    };

    class easter_abstract_test : public helpers::input_output_test<fixed_date_t, gregorian_date> {
    protected:
        gregorian_calendar m_gregorian{};
    };

    class orthodox_easter_test : public easter_abstract_test {
    protected:
        holiday_providers::julian_calendar_holiday_provider m_provider{};
    };

    class classic_easter_test : public easter_abstract_test {
    protected:
        holiday_providers::gregorian_calendar_holiday_provider m_provider{};
    };

    class astronomical_easter_test : public easter_abstract_test {
    protected:
        holiday_providers::gregorian_calendar_holiday_provider m_provider{};
    };
    TEST_P(orthodox_easter_test, calculate) {
        auto &[input, output] = GetParam();
        const auto year = m_gregorian.get_year(input);
        const auto easter = m_provider.orthodox_easter(year);
        const auto actual = m_gregorian.to_date(easter);
        const auto diff = m_gregorian.to_fixed(output) - easter;
        EXPECT_EQ(actual, output) << year << " : " << actual.to_string() << " : " << output.to_string() << " : " << diff << std::endl;
    }

    TEST_P(classic_easter_test, calculate) {
        auto &[input, output] = GetParam();
        const auto year = m_gregorian.get_year(input);
        const auto easter = m_provider.easter(year);
        const auto actual = m_gregorian.to_date(easter);
        const auto diff = m_gregorian.to_fixed(output) - easter;
        EXPECT_EQ(actual, output) << year << " : " << actual.to_string() << " : " << output.to_string() << " : " << diff << std::endl;
    }

    TEST_P(astronomical_easter_test, calculate) {
        auto &[input, output] = GetParam();
        const auto year = m_gregorian.get_year(input);
        const auto easter = m_provider.astronomical_easter(year);
        const auto actual = m_gregorian.to_date(easter);
        const auto diff = m_gregorian.to_fixed(output) - easter;
        EXPECT_EQ(actual, output) << year << " : " << actual.to_string() << " : " << output.to_string() << " : " << diff << std::endl;
    }

    INSTANTIATE_TEST_SUITE_P(orthodox_easter, orthodox_easter_test, testing::ValuesIn(helpers::make_test_params_values(dates, orthodox)));
    INSTANTIATE_TEST_SUITE_P(easter, classic_easter_test, testing::ValuesIn(helpers::make_test_params_values(dates, gregorian)));
    INSTANTIATE_TEST_SUITE_P(astronomical_easter, astronomical_easter_test, testing::ValuesIn(helpers::make_test_params_values(dates, astronomical)));
}