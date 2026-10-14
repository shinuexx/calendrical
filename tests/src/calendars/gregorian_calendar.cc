#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"
#include "calendars/gregorian_calendar.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::calendars;

    static const std::array gdates{
        gregorian_date{-586,month{7},24},
        gregorian_date{-168,month{12},5},
        gregorian_date{70,month{9},24},
        gregorian_date{135,month{10},2},
        gregorian_date{470,month{1},8},
        gregorian_date{576,month{5},20},
        gregorian_date{694,month{11},10},
        gregorian_date{1013,month{4},25},
        gregorian_date{1096,month{5},24},
        gregorian_date{1190,month{3},23},
        gregorian_date{1240,month{3},10},
        gregorian_date{1288,month{4},2},
        gregorian_date{1298,month{4},27},
        gregorian_date{1391,month{6},12},
        gregorian_date{1436,month{2},3},
        gregorian_date{1492,month{4},9},
        gregorian_date{1553,month{9},19},
        gregorian_date{1560,month{3},5},
        gregorian_date{1648,month{6},10},
        gregorian_date{1680,month{6},30},
        gregorian_date{1716,month{7},24},
        gregorian_date{1768,month{6},19},
        gregorian_date{1819,month{8},2},
        gregorian_date{1839,month{3},27},
        gregorian_date{1903,month{4},19},
        gregorian_date{1929,month{8},25},
        gregorian_date{1941,month{9},29},
        gregorian_date{1943,month{4},19},
        gregorian_date{1943,month{10},7},
        gregorian_date{1992,month{3},17},
        gregorian_date{1996,month{2},25},
        gregorian_date{2038,month{11},10},
        gregorian_date{2094,month{7},18}
    };

    class gregorian_calendar_test : public helpers::input_output_test<fixed_date_t, gregorian_date> {
    protected:
        const gregorian_calendar *m_gregorian = nullptr;

        void SetUp() override {
            m_gregorian = std::addressof(gregorian_calendar::standard);
        }

        void TearDown() override {
            m_gregorian = nullptr;
        }
    };

    TEST_P(gregorian_calendar_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_gregorian->to_date(input), output) << input << " : " << m_gregorian->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(gregorian_calendar_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_gregorian->to_fixed(output), input) << output.to_string() << " : " << m_gregorian->to_fixed(output) << " : " << input << std::endl;
    }

    INSTANTIATE_TEST_SUITE_P(gregorian_calendar, gregorian_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, gdates)));
}