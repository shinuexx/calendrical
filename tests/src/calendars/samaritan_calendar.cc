#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"
#include "calendars/samaritan_calendar.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::calendars;

    static const std::array gdates{
        simple_date{1052, 5, 12},
        simple_date{1471, 9, 27},
        simple_date{1709, 7, 4},
        simple_date{1774, 7, 9},
        simple_date{2108, 10, 19},
        simple_date{2214, 3, 5},
        simple_date{2333, 8, 14},
        simple_date{2651, 2, 7},
        simple_date{2734, 2, 24},
        simple_date{2828, 13, 9},
        simple_date{2878, 12, 8},
        simple_date{2926, 13, 23},
        simple_date{2936, 2, 8},
        simple_date{3029, 3, 2},
        simple_date{3074, 11, 7},
        simple_date{3130, 1, 4},
        simple_date{3192, 7, 2},
        simple_date{3198, 11, 28},
        simple_date{3286, 3, 19},
        simple_date{3318, 4, 4},
        simple_date{3354, 5, 6},
        simple_date{3406, 3, 5},
        simple_date{3457, 5, 12},
        simple_date{3477, 13, 12},
        simple_date{3541, 1, 22},
        simple_date{3567, 5, 21},
        simple_date{3580, 7, 9},
        simple_date{3581, 1, 15},
        simple_date{3582, 7, 8},
        simple_date{3630, 12, 13},
        simple_date{3634, 12, 7},
        simple_date{3677, 8, 14},
        simple_date{3732, 4, 6}
    };

    class samaritan_calendar_test : public helpers::input_output_test<fixed_date_t, simple_date> {
    protected:
        std::unique_ptr<const samaritan_calendar> m_calendar = std::make_unique<samaritan_calendar>();
    };

    TEST_P(samaritan_calendar_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(samaritan_calendar_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    INSTANTIATE_TEST_CASE_P(samaritan_calendar, samaritan_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, gdates)));
}