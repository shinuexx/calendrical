#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"
#include "calendars/iso_calendar.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::calendars;

    static const std::array idates{
        iso_date{-586, 29, 7},
        iso_date{-168, 49, 3},
        iso_date{70, 39, 3},
        iso_date{135, 39, 7},
        iso_date{470, 2, 3},
        iso_date{576, 21, 1},
        iso_date{694, 45, 6},
        iso_date{1013, 16, 7},
        iso_date{1096, 21, 7},
        iso_date{1190, 12, 5},
        iso_date{1240, 10, 6},
        iso_date{1288, 14, 5},
        iso_date{1298, 17, 7},
        iso_date{1391, 23, 7},
        iso_date{1436, 5, 3},
        iso_date{1492, 14, 6},
        iso_date{1553, 38, 6},
        iso_date{1560, 9, 6},
        iso_date{1648, 24, 3},
        iso_date{1680, 26, 7},
        iso_date{1716, 30, 5},
        iso_date{1768, 24, 7},
        iso_date{1819, 31, 1},
        iso_date{1839, 13, 3},
        iso_date{1903, 16, 7},
        iso_date{1929, 34, 7},
        iso_date{1941, 40, 1},
        iso_date{1943, 16, 1},
        iso_date{1943, 40, 4},
        iso_date{1992, 12, 2},
        iso_date{1996, 8, 7},
        iso_date{2038, 45, 3},
        iso_date{2094, 28, 7}
    };

    class iso_calendar_test : public helpers::input_output_test<fixed_date_t, iso_date> {
    protected:
        std::unique_ptr<const iso_calendar> m_calendar = std::make_unique<iso_calendar>();
    };

    TEST_P(iso_calendar_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(iso_calendar_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    INSTANTIATE_TEST_SUITE_P(iso_calendar, iso_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, idates)));
}