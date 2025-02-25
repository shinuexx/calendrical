#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"
#include "calendars/islamic_calendar.hpp"
#include "calendars/observational_islamic_calendar.hpp"
#include "calendars/saudi_islamic_calendar.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::calendars;

    static const std::array arithmetic_dates{
        simple_date{-1245, 12, 9},
        simple_date{-813, 2, 23},
        simple_date{-568, 4, 1},
        simple_date{-501, 4, 6},
        simple_date{-157, 10, 17},
        simple_date{-47, 6, 3},
        simple_date{75, 7, 13},
        simple_date{403, 10, 5},
        simple_date{489, 5, 22},
        simple_date{586, 2, 7},
        simple_date{637, 8, 7},
        simple_date{687, 2, 20},
        simple_date{697, 7, 7},
        simple_date{793, 7, 1},
        simple_date{839, 7, 6},
        simple_date{897, 6, 1},
        simple_date{960, 9, 30},
        simple_date{967, 5, 27},
        simple_date{1058, 5, 18},
        simple_date{1091, 6, 2},
        simple_date{1128, 8, 4},
        simple_date{1182, 2, 3},
        simple_date{1234, 10, 10},
        simple_date{1255, 1, 11},
        simple_date{1321, 1, 21},
        simple_date{1348, 3, 19},
        simple_date{1360, 9, 8},
        simple_date{1362, 4, 13},
        simple_date{1362, 10, 7},
        simple_date{1412, 9, 13},
        simple_date{1416, 10, 5},
        simple_date{1460, 10, 12},
        simple_date{1518, 3, 5}
    };

    static const std::array observational_dates{
        simple_date{-1245, 12, 11},
        simple_date{-813, 2, 25},
        simple_date{-568, 4, 2},
        simple_date{-501, 4, 7},
        simple_date{-157, 10, 18},
        simple_date{-47, 6, 3},
        simple_date{75, 7, 13},
        simple_date{403, 10, 5},
        simple_date{489, 5, 22},
        simple_date{586, 2, 7},
        simple_date{637, 8, 7},
        simple_date{687, 2, 21},
        simple_date{697, 7, 7},
        simple_date{793, 6, 30},
        simple_date{839, 7, 6},
        simple_date{897, 6, 2},
        simple_date{960, 9, 30},
        simple_date{967, 5, 27},
        simple_date{1058, 5, 18},
        simple_date{1091, 6, 3},
        simple_date{1128, 8, 4},
        simple_date{1182, 2, 4},
        simple_date{1234, 10, 10},
        simple_date{1255, 1, 11},
        simple_date{1321, 1, 20},
        simple_date{1348, 3, 19},
        simple_date{1360, 9, 7},
        simple_date{1362, 4, 14},
        simple_date{1362, 10, 7},
        simple_date{1412, 9, 12},
        simple_date{1416, 10, 5},
        simple_date{1460, 10, 12},
        simple_date{1518, 3, 5}
    };

    static const std::array saudi_dates{
        simple_date{-1245, 12, 11},
        simple_date{-813, 2, 26},
        simple_date{-568, 4, 3},
        simple_date{-501, 4, 8},
        simple_date{-157, 10, 18},
        simple_date{-47, 6, 4},
        simple_date{75, 7, 14},
        simple_date{403, 10, 6},
        simple_date{489, 5, 23},
        simple_date{586, 2, 8},
        simple_date{637, 8, 8},
        simple_date{687, 2, 22},
        simple_date{697, 7, 8},
        simple_date{793, 7, 1},
        simple_date{839, 7, 7},
        simple_date{897, 6, 3},
        simple_date{960, 10, 1},
        simple_date{967, 5, 28},
        simple_date{1058, 5, 19},
        simple_date{1091, 6, 4},
        simple_date{1128, 8, 5},
        simple_date{1182, 2, 4},
        simple_date{1234, 10, 11},
        simple_date{1255, 1, 11},
        simple_date{1321, 1, 21},
        simple_date{1348, 3, 20},
        simple_date{1360, 9, 8},
        simple_date{1362, 4, 14},
        simple_date{1362, 10, 8},
        simple_date{1412, 9, 12},
        simple_date{1416, 10, 6},
        simple_date{1460, 10, 13},
        simple_date{1518, 3, 6}
    };

    class islamic_based_calendar_test : public helpers::input_output_test<fixed_date_t, simple_date> {
    protected:
        const islamic_based_calendar* m_calendar = nullptr;
    };

    class islamic_calendar_test : public islamic_based_calendar_test {
    protected:
        void SetUp() override {
            m_calendar = new islamic_calendar();
        }
        void TearDown() override {
            delete m_calendar;
            m_calendar = nullptr;
        }
    };

    class observational_islamic_calendar_test : public islamic_based_calendar_test {
    protected:
        void SetUp() override {
            m_calendar = new observational_islamic_calendar();
        }
        void TearDown() override {
            delete m_calendar;
            m_calendar = nullptr;
        }
    };

    class saudi_islamic_calendar_test : public islamic_based_calendar_test {
    protected:
        void SetUp() override {
            m_calendar = new saudi_islamic_calendar();
        }
        void TearDown() override {
            delete m_calendar;
            m_calendar = nullptr;
        }
    };

    TEST_P(islamic_calendar_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(islamic_calendar_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    TEST_P(observational_islamic_calendar_test, to) {
        //GTEST_SKIP();
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(observational_islamic_calendar_test, from) {
        //GTEST_SKIP();
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    TEST_P(saudi_islamic_calendar_test, to) {
        //GTEST_SKIP();
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(saudi_islamic_calendar_test, from) {
        //GTEST_SKIP();
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    INSTANTIATE_TEST_CASE_P(islamic_calendar, islamic_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, arithmetic_dates)));
    INSTANTIATE_TEST_CASE_P(observational_islamic_calendar, observational_islamic_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, observational_dates)));
    INSTANTIATE_TEST_CASE_P(saudi_islamic_calendar, saudi_islamic_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, saudi_dates)));
}