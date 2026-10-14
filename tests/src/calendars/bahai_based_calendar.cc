#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"
#include "calendars/bahai_calendar.hpp"
#include "calendars/astro_bahai_calendar.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::calendars;

    static const std::array arithmetic_dates{
        bahai_date{-6, 6, 3, 7, 12},
        bahai_date{-5, 9, 3, 14, 13},
        bahai_date{-4, 2, 13, 10, 17},
        bahai_date{-4, 6, 2, 11, 6},
        bahai_date{-3, 4, 13, 16, 9},
        bahai_date{-3, 10, 6, 4, 4},
        bahai_date{-3, 16, 10, 13, 7},
        bahai_date{-2, 14, 6, 2, 17},
        bahai_date{-2, 18, 13, 4, 8},
        bahai_date{-1, 4, 12, 1, 3},
        bahai_date{-1, 7, 4, 19, 9},
        bahai_date{-1, 9, 15, 1, 13},
        bahai_date{-1, 10, 6, 2, 19},
        bahai_date{-1, 15, 4, 5, 8},
        bahai_date{-1, 17, 10, 17, 16},
        bahai_date{0, 1, 10, 2, 1},
        bahai_date{0, 4, 14, 10, 12},
        bahai_date{0, 5, 1, 19, 4},
        bahai_date{0, 9, 14, 5, 6},
        bahai_date{0, 11, 8, 6, 7},
        bahai_date{0, 13, 6, 7, 12},
        bahai_date{0, 16, 1, 5, 15},
        bahai_date{0, 18, 14, 8, 2},
        bahai_date{0, 19, 15, 1, 7},
        bahai_date{1, 4, 3, 2, 11},
        bahai_date{1, 5, 10, 9, 6},
        bahai_date{1, 6, 3, 11, 3},
        bahai_date{1, 6, 5, 2, 11},
        bahai_date{1, 6, 5, 11, 11},
        bahai_date{1, 8, 15, 19, 16},
        bahai_date{1, 8, 19, 18, 19},
        bahai_date{1, 11, 5, 13, 7},
        bahai_date{1, 14, 4, 7, 6}
    };

    static const std::array astro_dates{
        bahai_date{-6, 6, 3, 7, 11},
        bahai_date{-5, 9, 3, 14, 13},
        bahai_date{-4, 2, 13, 10, 17},
        bahai_date{-4, 6, 2, 11, 6},
        bahai_date{-3, 4, 13, 16, 10},
        bahai_date{-3, 10, 6, 4, 5},
        bahai_date{-3, 16, 10, 13, 7},
        bahai_date{-2, 14, 6, 2, 17},
        bahai_date{-2, 18, 13, 4, 9},
        bahai_date{-1, 4, 12, 1, 3},
        bahai_date{-1, 7, 4, 19, 10},
        bahai_date{-1, 9, 15, 1, 14},
        bahai_date{-1, 10, 6, 3, 1},
        bahai_date{-1, 15, 4, 5, 8},
        bahai_date{-1, 17, 10, 17, 16},
        bahai_date{0, 1, 10, 2, 2},
        bahai_date{0, 4, 14, 10, 12},
        bahai_date{0, 5, 1, 19, 4},
        bahai_date{0, 9, 14, 5, 7},
        bahai_date{0, 11, 8, 6, 8},
        bahai_date{0, 13, 6, 7, 13},
        bahai_date{0, 16, 1, 5, 16},
        bahai_date{0, 18, 14, 8, 2},
        bahai_date{0, 19, 15, 1, 7},
        bahai_date{1, 4, 3, 2, 10},
        bahai_date{1, 5, 10, 9, 6},
        bahai_date{1, 6, 3, 11, 3},
        bahai_date{1, 6, 5, 2, 11},
        bahai_date{1, 6, 5, 11, 11},
        bahai_date{1, 8, 15, 19, 17},
        bahai_date{1, 8, 19, 18, 19},
        bahai_date{1, 11, 5, 13, 8},
        bahai_date{1, 14, 4, 7, 7},

    };

    class bahai_based_calendar_test : public helpers::input_output_test<fixed_date_t, bahai_date> {
    protected:
        std::unique_ptr<const bahai_based_calendar> m_calendar;
    };

    class bahai_calendar_test : public bahai_based_calendar_test {
    protected:
        void SetUp() override {
            m_calendar = std::make_unique<bahai_calendar>();
        }
    };

    class astro_bahai_calendar_test : public bahai_based_calendar_test {
    protected:
        void SetUp() override {
            m_calendar = std::make_unique<astro_bahai_calendar>();
        }
    };


    TEST_P(bahai_calendar_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(bahai_calendar_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    TEST_P(astro_bahai_calendar_test, to) {
        //GTEST_SKIP();
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(astro_bahai_calendar_test, from) {
        //GTEST_SKIP();
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    INSTANTIATE_TEST_SUITE_P(bahai_calendar, bahai_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, arithmetic_dates)));
    INSTANTIATE_TEST_SUITE_P(astro_bahai_calendar, astro_bahai_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, astro_dates)));
}