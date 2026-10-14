#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"
#include "calendars/babylonian_calendar.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::calendars;

    static const std::array bdates{
        simple_lunar_date{-275, 4, false, 10},
        simple_lunar_date{143, 8, false, 24},
        simple_lunar_date{381, 7, false, 1},
        simple_lunar_date{446, 7, false, 7},
        simple_lunar_date{780, 10, false, 17},
        simple_lunar_date{887, 2, false, 3},
        simple_lunar_date{1005, 8, false, 13},
        simple_lunar_date{1324, 1, false, 5},
        simple_lunar_date{1407, 2, false, 22},
        simple_lunar_date{1500, 12, false, 7},
        simple_lunar_date{1550, 12, false, 7},
        simple_lunar_date{1598, 12, false, 21},
        simple_lunar_date{1609, 1, false, 7},
        simple_lunar_date{1702, 2, false, 29},
        simple_lunar_date{1746, 11, false, 6},
        simple_lunar_date{1803, 1, false, 2},
        simple_lunar_date{1864, 5, false, 30},
        simple_lunar_date{1870, 11, false, 27},
        simple_lunar_date{1959, 2, false, 18},
        simple_lunar_date{1991, 4, false, 2},
        simple_lunar_date{2027, 4, false, 4},
        simple_lunar_date{2079, 3, false, 3},
        simple_lunar_date{2130, 4, false, 10},
        simple_lunar_date{2149, 12, false, 11},
        simple_lunar_date{2213, 12, true, 20},
        simple_lunar_date{2240, 5, false, 19},
        simple_lunar_date{2252, 6, false, 7},
        simple_lunar_date{2254, 1, false, 13},
        simple_lunar_date{2254, 7, false, 7},
        simple_lunar_date{2302, 12, false, 12},
        simple_lunar_date{2306, 12, false, 5},
        simple_lunar_date{2349, 8, false, 12},
        simple_lunar_date{2405, 4, false, 5}
    };

    class babylonian_calendar_test : public helpers::input_output_test<fixed_date_t, simple_lunar_date> {
    protected:
        std::unique_ptr<const babylonian_calendar> m_calendar = std::make_unique<babylonian_calendar>();
    };

    TEST_P(babylonian_calendar_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(babylonian_calendar_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    INSTANTIATE_TEST_SUITE_P(babylonian_calendar, babylonian_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, bdates)));
}