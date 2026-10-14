#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"
#include "calendars/tibetan_calendar.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::calendars;

    static const std::array tdates{
        lunar_date{-459, 8, false, 11, false},
        lunar_date{-41, 12, false, 27, false},
        lunar_date{197, 10, false, 3, false},
        lunar_date{262, 10, false, 9, false},
        lunar_date{596, 12, false, 19, false},
        lunar_date{703, 5, false, 4, false},
        lunar_date{821, 10, false, 15, false},
        lunar_date{1140, 4, false, 6, false},
        lunar_date{1223, 4, false, 23, false},
        lunar_date{1317, 3, false, 8, false},
        lunar_date{1367, 2, false, 8, false},
        lunar_date{1415, 2, false, 22, false},
        lunar_date{1425, 4, false, 8, false},
        lunar_date{1518, 5, false, 1, false},
        lunar_date{1563, 1, false, 7, false},
        lunar_date{1619, 3, false, 3, false},
        lunar_date{1680, 8, false, 2, false},
        lunar_date{1687, 1, false, 29, false},
        lunar_date{1775, 4, false, 20, false},
        lunar_date{1807, 6, true, 4, false},
        lunar_date{1843, 6, false, 6, false},
        lunar_date{1895, 5, false, 5, false},
        lunar_date{1946, 6, false, 11, false},
        lunar_date{1966, 2, false, 13, false},
        lunar_date{2030, 2, false, 22, false},
        lunar_date{2056, 7, false, 20, false},
        lunar_date{2068, 8, false, 9, false},
        lunar_date{2070, 3, true, 14, false},
        lunar_date{2070, 8, false, 8, false},
        lunar_date{2119, 1, false, 14, false},
        lunar_date{2123, 1, false, 7, false},
        lunar_date{2165, 9, false, 14, false},
        lunar_date{2221, 6, false, 6, false},
    };

    class tibetan_calendar_test : public helpers::input_output_test<fixed_date_t, lunar_date> {
    protected:
        std::unique_ptr<const tibetan_calendar> m_calendar = std::make_unique<tibetan_calendar>();
    };

    TEST_P(tibetan_calendar_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(tibetan_calendar_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    INSTANTIATE_TEST_CASE_P(tibetan_calendar, tibetan_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, tdates)));
}