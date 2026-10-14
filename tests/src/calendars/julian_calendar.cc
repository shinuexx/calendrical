#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"
#include "calendars/julian_calendar.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::calendars;

    static const std::array gdates{
        julian_date{-587, month{7}, 30},
        julian_date{-169, month{12}, 8},
        julian_date{70, month{9}, 26},
        julian_date{135, month{10}, 3},
        julian_date{470, month{1}, 7},
        julian_date{576, month{5}, 18},
        julian_date{694, month{11}, 7},
        julian_date{1013, month{4}, 19},
        julian_date{1096, month{5}, 18},
        julian_date{1190, month{3}, 16},
        julian_date{1240, month{3}, 3},
        julian_date{1288, month{3}, 26},
        julian_date{1298, month{4}, 20},
        julian_date{1391, month{6}, 4},
        julian_date{1436, month{1}, 25},
        julian_date{1492, month{3}, 31},
        julian_date{1553, month{9}, 9},
        julian_date{1560, month{2}, 24},
        julian_date{1648, month{5}, 31},
        julian_date{1680, month{6}, 20},
        julian_date{1716, month{7}, 13},
        julian_date{1768, month{6}, 8},
        julian_date{1819, month{7}, 21},
        julian_date{1839, month{3}, 15},
        julian_date{1903, month{4}, 6},
        julian_date{1929, month{8}, 12},
        julian_date{1941, month{9}, 16},
        julian_date{1943, month{4}, 6},
        julian_date{1943, month{9}, 24},
        julian_date{1992, month{3}, 4},
        julian_date{1996, month{2}, 12},
        julian_date{2038, month{10}, 28},
        julian_date{2094, month{7}, 5}
    };

    class julian_calendar_test : public helpers::input_output_test<fixed_date_t, julian_date> {
    protected:
        std::unique_ptr<const julian_calendar> m_calendar = std::make_unique<julian_calendar>();
    };

    TEST_P(julian_calendar_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(julian_calendar_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    INSTANTIATE_TEST_CASE_P(julian_calendar, julian_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, gdates)));
}