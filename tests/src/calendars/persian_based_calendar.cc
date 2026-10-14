#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"
#include "calendars/persian_calendar.hpp"
#include "calendars/arithmetic_persian_calendar.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::calendars;

    static const std::array observational_dates{
        simple_date{-1208, 5, 1},
        simple_date{-790, 9, 14},
        simple_date{-552, 7, 2},
        simple_date{-487, 7, 9},
        simple_date{-153, 10, 19},
        simple_date{-46, 2, 31},
        simple_date{73, 8, 19},
        simple_date{392, 2, 5},
        simple_date{475, 3, 4},
        simple_date{569, 1, 3},
        simple_date{618, 12, 20},
        simple_date{667, 1, 14},
        simple_date{677, 2, 8},
        simple_date{770, 3, 22},
        simple_date{814, 11, 13},
        simple_date{871, 1, 21},
        simple_date{932, 6, 28},
        simple_date{938, 12, 14},
        simple_date{1027, 3, 21},
        simple_date{1059, 4, 10},
        simple_date{1095, 5, 2},
        simple_date{1147, 3, 30},
        simple_date{1198, 5, 10},
        simple_date{1218, 1, 7},
        simple_date{1282, 1, 29},
        simple_date{1308, 6, 3},
        simple_date{1320, 7, 7},
        simple_date{1322, 1, 29},
        simple_date{1322, 7, 14},
        simple_date{1370, 12, 27},
        simple_date{1374, 12, 6},
        simple_date{1417, 8, 19},
        simple_date{1473, 4, 28}
    };

    static const std::array arithmetic_dates{
        simple_date{-1208, 5, 1},
        simple_date{-790, 9, 14},
        simple_date{-552, 7, 2},
        simple_date{-487, 7, 9},
        simple_date{-153, 10, 18},
        simple_date{-46, 2, 30},
        simple_date{73, 8, 19},
        simple_date{392, 2, 5},
        simple_date{475, 3, 3},
        simple_date{569, 1, 3},
        simple_date{618, 12, 20},
        simple_date{667, 1, 14},
        simple_date{677, 2, 8},
        simple_date{770, 3, 22},
        simple_date{814, 11, 13},
        simple_date{871, 1, 21},
        simple_date{932, 6, 28},
        simple_date{938, 12, 14},
        simple_date{1027, 3, 21},
        simple_date{1059, 4, 10},
        simple_date{1095, 5, 2},
        simple_date{1147, 3, 30},
        simple_date{1198, 5, 10},
        simple_date{1218, 1, 7},
        simple_date{1282, 1, 29},
        simple_date{1308, 6, 3},
        simple_date{1320, 7, 7},
        simple_date{1322, 1, 29},
        simple_date{1322, 7, 14},
        simple_date{1370, 12, 27},
        simple_date{1374, 12, 6},
        simple_date{1417, 8, 19},
        simple_date{1473, 4, 28}
    };

    class persian_based_calendar_test : public helpers::input_output_test<fixed_date_t, simple_date> {
    protected:
        std::unique_ptr<const persian_based_calendar> m_calendar;
    };

    class persian_calendar_test : public persian_based_calendar_test {
    protected:
        void SetUp() override {
            m_calendar = std::make_unique<persian_calendar>();
        }
    };

    class arithmetic_persian_calendar_test : public persian_based_calendar_test {
    protected:
        void SetUp() override {
            m_calendar = std::make_unique<arithmetic_persian_calendar>();
        }
    };


    TEST_P(persian_calendar_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(persian_calendar_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    TEST_P(arithmetic_persian_calendar_test, to) {
        //GTEST_SKIP();
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(arithmetic_persian_calendar_test, from) {
        //GTEST_SKIP();
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    INSTANTIATE_TEST_CASE_P(persian_calendar, persian_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, observational_dates)));
    INSTANTIATE_TEST_CASE_P(arithmetic_persian_calendar, arithmetic_persian_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, arithmetic_dates)));
}