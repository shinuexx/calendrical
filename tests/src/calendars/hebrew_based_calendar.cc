#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"
#include "calendars/hebrew_calendar.hpp"
#include "calendars/observational_hebrew_calendar.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::calendars;

    static const std::array arithmetic_dates{
        hebrew_date{3174, hebrew_month{5}, 10},
        hebrew_date{3593, hebrew_month{9}, 25},
        hebrew_date{3831, hebrew_month{7}, 3},
        hebrew_date{3896, hebrew_month{7}, 9},
        hebrew_date{4230, hebrew_month{10}, 18},
        hebrew_date{4336, hebrew_month{3}, 4},
        hebrew_date{4455, hebrew_month{8}, 13},
        hebrew_date{4773, hebrew_month{2}, 6},
        hebrew_date{4856, hebrew_month{2}, 23},
        hebrew_date{4950, hebrew_month{1}, 7},
        hebrew_date{5000, hebrew_month{13}, 8},
        hebrew_date{5048, hebrew_month{1}, 21},
        hebrew_date{5058, hebrew_month{2}, 7},
        hebrew_date{5151, hebrew_month{4}, 1},
        hebrew_date{5196, hebrew_month{11}, 7},
        hebrew_date{5252, hebrew_month{1}, 3},
        hebrew_date{5314, hebrew_month{7}, 1},
        hebrew_date{5320, hebrew_month{12}, 27},
        hebrew_date{5408, hebrew_month{3}, 20},
        hebrew_date{5440, hebrew_month{4}, 3},
        hebrew_date{5476, hebrew_month{5}, 5},
        hebrew_date{5528, hebrew_month{4}, 4},
        hebrew_date{5579, hebrew_month{5}, 11},
        hebrew_date{5599, hebrew_month{1}, 12},
        hebrew_date{5663, hebrew_month{1}, 22},
        hebrew_date{5689, hebrew_month{5}, 19},
        hebrew_date{5702, hebrew_month{7}, 8},
        hebrew_date{5703, hebrew_month{1}, 14},
        hebrew_date{5704, hebrew_month{7}, 8},
        hebrew_date{5752, hebrew_month{13}, 12},
        hebrew_date{5756, hebrew_month{12}, 5},
        hebrew_date{5799, hebrew_month{8}, 12},
        hebrew_date{5854, hebrew_month{5}, 5},

    };

    static const std::array observational_dates{
        hebrew_date{3174, hebrew_month{5}, 11},
        hebrew_date{3593, hebrew_month{9}, 24},
        hebrew_date{3831, hebrew_month{7}, 2},
        hebrew_date{3896, hebrew_month{7}, 7},
        hebrew_date{4230, hebrew_month{10}, 18},
        hebrew_date{4336, hebrew_month{3}, 3},
        hebrew_date{4455, hebrew_month{9}, 13},
        hebrew_date{4773, hebrew_month{2}, 5},
        hebrew_date{4856, hebrew_month{2}, 22},
        hebrew_date{4950, hebrew_month{1}, 7},
        hebrew_date{5000, hebrew_month{13}, 7},
        hebrew_date{5048, hebrew_month{1}, 21},
        hebrew_date{5058, hebrew_month{2}, 7},
        hebrew_date{5151, hebrew_month{3}, 30},
        hebrew_date{5196, hebrew_month{12}, 6},
        hebrew_date{5252, hebrew_month{2}, 2},
        hebrew_date{5313, hebrew_month{6}, 30},
        hebrew_date{5320, hebrew_month{12}, 27},
        hebrew_date{5408, hebrew_month{3}, 18},
        hebrew_date{5440, hebrew_month{4}, 3},
        hebrew_date{5476, hebrew_month{5}, 4},
        hebrew_date{5528, hebrew_month{4}, 4},
        hebrew_date{5579, hebrew_month{5}, 10},
        hebrew_date{5599, hebrew_month{1}, 11},
        hebrew_date{5663, hebrew_month{1}, 20},
        hebrew_date{5689, hebrew_month{6}, 19},
        hebrew_date{5702, hebrew_month{7}, 7},
        hebrew_date{5703, hebrew_month{2}, 14},
        hebrew_date{5704, hebrew_month{8}, 7},
        hebrew_date{5752, hebrew_month{1}, 12},
        hebrew_date{5756, hebrew_month{12}, 5},
        hebrew_date{5799, hebrew_month{9}, 12},
        hebrew_date{5854, hebrew_month{5}, 5},

    };

    class hebrew_based_calendar_test : public helpers::input_output_test<fixed_date_t, hebrew_date> {
    protected:
        std::unique_ptr<const hebrew_based_calendar> m_calendar;
    };

    class hebrew_calendar_test : public hebrew_based_calendar_test {
    protected:
        void SetUp() override {
            m_calendar = std::make_unique<hebrew_calendar>();
        }
    };

    class observational_hebrew_calendar_test : public hebrew_based_calendar_test {
    protected:
        void SetUp() override {
            m_calendar = std::make_unique<observational_hebrew_calendar>();
        }
    };


    TEST_P(hebrew_calendar_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(hebrew_calendar_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    TEST_P(observational_hebrew_calendar_test, to) {
        //GTEST_SKIP();
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(observational_hebrew_calendar_test, from) {
        //GTEST_SKIP();
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    INSTANTIATE_TEST_CASE_P(hebrew_calendar, hebrew_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, arithmetic_dates)));
    INSTANTIATE_TEST_CASE_P(observational_hebrew_calendar, observational_hebrew_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, observational_dates)));
}