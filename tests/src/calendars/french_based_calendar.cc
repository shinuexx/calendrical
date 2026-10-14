#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"
#include "calendars/french_calendar.hpp"
#include "calendars/arithmetic_french_calendar.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::calendars;

    static const std::array observational_dates{
        simple_date{-2378, 11, 5},
        simple_date{-1959, 3, 14},
        simple_date{-1721, 1, 2},
        simple_date{-1656, 1, 10},
        simple_date{-1322, 4, 19},
        simple_date{-1216, 9, 1},
        simple_date{-1097, 2, 19},
        simple_date{-779, 8, 5},
        simple_date{-696, 9, 5},
        simple_date{-602, 7, 2},
        simple_date{-552, 6, 20},
        simple_date{-504, 7, 13},
        simple_date{-494, 8, 8},
        simple_date{-401, 9, 23},
        simple_date{-356, 5, 14},
        simple_date{-300, 7, 20},
        simple_date{-239, 13, 2},
        simple_date{-232, 6, 15},
        simple_date{-144, 9, 22},
        simple_date{-112, 10, 12},
        simple_date{-76, 11, 6},
        simple_date{-24, 10, 1},
        simple_date{27, 11, 14},
        simple_date{47, 7, 6},
        simple_date{111, 7, 28},
        simple_date{137, 12, 7},
        simple_date{150, 1, 7},
        simple_date{151, 7, 29},
        simple_date{152, 1, 15},
        simple_date{200, 6, 27},
        simple_date{204, 6, 6},
        simple_date{247, 2, 20},
        simple_date{302, 10, 30}
    };

    static const std::array arithmetic_dates{
        simple_date{-2378, 11, 4},
        simple_date{-1959, 3, 13},
        simple_date{-1721, 1, 2},
        simple_date{-1656, 1, 10},
        simple_date{-1322, 4, 18},
        simple_date{-1216, 9, 1},
        simple_date{-1097, 2, 19},
        simple_date{-779, 8, 4},
        simple_date{-696, 9, 5},
        simple_date{-602, 7, 1},
        simple_date{-552, 6, 20},
        simple_date{-504, 7, 13},
        simple_date{-494, 8, 8},
        simple_date{-401, 9, 23},
        simple_date{-356, 5, 13},
        simple_date{-300, 7, 19},
        simple_date{-239, 13, 1},
        simple_date{-232, 6, 14},
        simple_date{-144, 9, 22},
        simple_date{-112, 10, 12},
        simple_date{-76, 11, 6},
        simple_date{-24, 10, 1},
        simple_date{27, 11, 14},
        simple_date{47, 7, 6},
        simple_date{111, 7, 29},
        simple_date{137, 12, 7},
        simple_date{150, 1, 7},
        simple_date{151, 7, 29},
        simple_date{152, 1, 15},
        simple_date{200, 6, 27},
        simple_date{204, 6, 7},
        simple_date{247, 2, 20},
        simple_date{302, 11, 1}
    };

    class french_based_calendar_test : public helpers::input_output_test<fixed_date_t, simple_date> {
    protected:
        std::unique_ptr<const french_based_calendar> m_calendar;
    };

    class french_calendar_test : public french_based_calendar_test {
    protected:
        void SetUp() override {
            m_calendar = std::make_unique<french_calendar>();
        }
    };

    class arithmetic_french_calendar_test : public french_based_calendar_test {
    protected:
        void SetUp() override {
            m_calendar = std::make_unique<arithmetic_french_calendar>();
        }
    };


    TEST_P(french_calendar_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(french_calendar_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    TEST_P(arithmetic_french_calendar_test, to) {
        //GTEST_SKIP();
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(arithmetic_french_calendar_test, from) {
        //GTEST_SKIP();
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    INSTANTIATE_TEST_SUITE_P(french_calendar, french_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, observational_dates)));
    INSTANTIATE_TEST_SUITE_P(arithmetic_french_calendar, arithmetic_french_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, arithmetic_dates)));
}