#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"
#include "calendars/coptic_based_calendar.hpp"
#include "local_helpers.hpp"

namespace tests{
    using namespace calendrical::calendars;

    static const std::array cdates{
        simple_date{-870, 12, 6},
        simple_date{-451, 4, 12},
        simple_date{-213, 1, 29},
        simple_date{-148, 2, 5},
        simple_date{186, 5, 12},
        simple_date{292, 9, 23},
        simple_date{411, 3, 11},
        simple_date{729, 8, 24},
        simple_date{812, 9, 23},
        simple_date{906, 7, 20},
        simple_date{956, 7, 7},
        simple_date{1004, 7, 30},
        simple_date{1014, 8, 25},
        simple_date{1107, 10, 10},
        simple_date{1152, 5, 29},
        simple_date{1208, 8, 5},
        simple_date{1270, 1, 12},
        simple_date{1276, 6, 29},
        simple_date{1364, 10, 6},
        simple_date{1396, 10, 26},
        simple_date{1432, 11, 19},
        simple_date{1484, 10, 14},
        simple_date{1535, 11, 27},
        simple_date{1555, 7, 19},
        simple_date{1619, 8, 11},
        simple_date{1645, 12, 19},
        simple_date{1658, 1, 19},
        simple_date{1659, 8, 11},
        simple_date{1660, 1, 26},
        simple_date{1708, 7, 8},
        simple_date{1712, 6, 17},
        simple_date{1755, 3, 1},
        simple_date{1810, 11, 11}
    };

    static const std::array edates{
        simple_date{-594, 12, 6},
        simple_date{-175, 4, 12},
        simple_date{63, 1, 29},
        simple_date{128, 2, 5},
        simple_date{462, 5, 12},
        simple_date{568, 9, 23},
        simple_date{687, 3, 11},
        simple_date{1005, 8, 24},
        simple_date{1088, 9, 23},
        simple_date{1182, 7, 20},
        simple_date{1232, 7, 7},
        simple_date{1280, 7, 30},
        simple_date{1290, 8, 25},
        simple_date{1383, 10, 10},
        simple_date{1428, 5, 29},
        simple_date{1484, 8, 5},
        simple_date{1546, 1, 12},
        simple_date{1552, 6, 29},
        simple_date{1640, 10, 6},
        simple_date{1672, 10, 26},
        simple_date{1708, 11, 19},
        simple_date{1760, 10, 14},
        simple_date{1811, 11, 27},
        simple_date{1831, 7, 19},
        simple_date{1895, 8, 11},
        simple_date{1921, 12, 19},
        simple_date{1934, 1, 19},
        simple_date{1935, 8, 11},
        simple_date{1936, 1, 26},
        simple_date{1984, 7, 8},
        simple_date{1988, 6, 17},
        simple_date{2031, 3, 1},
        simple_date{2086, 11, 11}
    };

    class coptic_based_calendar_test : public helpers::input_output_test<fixed_date_t, simple_date> {
    protected:
        const coptic_based_calendar* m_calendar = nullptr;
    };

    class coptic_calendar_test : public coptic_based_calendar_test {
    protected:
        void SetUp() override {
            m_calendar = std::addressof(coptic_calendar);
        }
        void TearDown() override {
            m_calendar = nullptr;
        }
    };

    class ethiopic_calendar_test : public coptic_based_calendar_test{
    protected:
        void SetUp() override {
            m_calendar = std::addressof(ethiopic_calendar);
        }
        void TearDown() override {
            m_calendar = nullptr;
        }
    };
    TEST_P(coptic_calendar_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(coptic_calendar_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    TEST_P(ethiopic_calendar_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(ethiopic_calendar_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    INSTANTIATE_TEST_SUITE_P(egyptian_calendar, coptic_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, cdates)));
    INSTANTIATE_TEST_SUITE_P(egyptian_calendar, ethiopic_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, edates)));
}
