#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"
#include "calendars/simple_calendar.hpp"
#include "local_helpers.hpp"

namespace tests{
    using namespace calendrical::calendars;

    static const std::array edates{
        simple_date{161, 7, 15},
        simple_date{580, 3, 6},
        simple_date{818, 2, 22},
        simple_date{883, 3, 15},
        simple_date{1217, 9, 15},
        simple_date{1324, 2, 18},
        simple_date{1442, 9, 10},
        simple_date{1761, 5, 8},
        simple_date{1844, 6, 28},
        simple_date{1938, 5, 18},
        simple_date{1988, 5, 18},
        simple_date{2036, 6, 23},
        simple_date{2046, 7, 20},
        simple_date{2139, 9, 28},
        simple_date{2184, 5, 29},
        simple_date{2240, 8, 19},
        simple_date{2302, 2, 11},
        simple_date{2308, 7, 30},
        simple_date{2396, 11, 29},
        simple_date{2428, 12, 27},
        simple_date{2465, 1, 24},
        simple_date{2517, 1, 2},
        simple_date{2568, 2, 27},
        simple_date{2587, 10, 29},
        simple_date{2651, 12, 7},
        simple_date{2678, 4, 17},
        simple_date{2690, 5, 25},
        simple_date{2691, 12, 17},
        simple_date{2692, 6, 3},
        simple_date{2740, 11, 27},
        simple_date{2744, 11, 7},
        simple_date{2787, 8, 1},
        simple_date{2843, 4, 20}
    };

    static const std::array adates{
        simple_date{-1138, 4, 10},
        simple_date{-720, 12, 6},
        simple_date{-482, 11, 22},
        simple_date{-417, 12, 15},
        simple_date{-82, 6, 10},
        simple_date{24, 11, 18},
        simple_date{143, 6, 5},
        simple_date{462, 2, 3},
        simple_date{545, 3, 23},
        simple_date{639, 2, 13},
        simple_date{689, 2, 13},
        simple_date{737, 3, 18},
        simple_date{747, 4, 15},
        simple_date{840, 6, 23},
        simple_date{885, 2, 24},
        simple_date{941, 5, 14},
        simple_date{1002, 11, 11},
        simple_date{1009, 4, 25},
        simple_date{1097, 8, 24},
        simple_date{1129, 9, 22},
        simple_date{1165, 10, 24},
        simple_date{1217, 10, 2},
        simple_date{1268, 11, 27},
        simple_date{1288, 7, 24},
        simple_date{1352, 9, 2},
        simple_date{1379, 1, 12},
        simple_date{1391, 2, 20},
        simple_date{1392, 9, 12},
        simple_date{1393, 2, 28},
        simple_date{1441, 8, 22},
        simple_date{1445, 8, 2},
        simple_date{1488, 4, 26},
        simple_date{1544, 1, 15}
    };

    class simple_calendar_test : public helpers::input_output_test<fixed_date_t, simple_date> {
    protected:
        const simple_calendar* m_calendar = nullptr;
    };

    class egyptian_calendar_test : public simple_calendar_test {
    protected:
        void SetUp() override {
            m_calendar = std::addressof(egyptian_calendar);
        }
        void TearDown() override {
            m_calendar = nullptr;
        }
    };

    class armenian_calendar_test : public simple_calendar_test{
    protected:
        void SetUp() override {
            m_calendar = std::addressof(armenian_calendar);
        }
        void TearDown() override {
            m_calendar = nullptr;
        }
    };
    TEST_P(egyptian_calendar_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }

    TEST_P(egyptian_calendar_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    TEST_P(armenian_calendar_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }

    TEST_P(armenian_calendar_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    INSTANTIATE_TEST_SUITE_P(egyptian_calendar, egyptian_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, edates)));
    INSTANTIATE_TEST_SUITE_P(egyptian_calendar, armenian_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, adates)));
}