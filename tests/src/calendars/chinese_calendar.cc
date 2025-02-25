#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"
#include "calendars/chinese_calendar.hpp"
#include "local_helpers.hpp"

namespace tests{
    using namespace calendrical::calendars;

    static const std::array cdates{
        chinese_date{35, 11, 6, false, 12},
        chinese_date{42, 9, 10, false, 27},
        chinese_date{46, 7, 8, false, 4},
        chinese_date{47, 12, 8, false, 9},
        chinese_date{52, 46, 11, false, 20},
        chinese_date{54, 33, 4, false, 5},
        chinese_date{56, 31, 10, false, 15},
        chinese_date{61, 50, 3, false, 7},
        chinese_date{63, 13, 4, false, 24},
        chinese_date{64, 47, 2, false, 9},
        chinese_date{65, 37, 2, false, 9},
        chinese_date{66, 25, 2, false, 23},
        chinese_date{66, 35, 3, false, 9},
        chinese_date{68, 8, 5, false, 2},
        chinese_date{68, 53, 1, false, 8},
        chinese_date{69, 49, 3, false, 4},
        chinese_date{70, 50, 8, false, 2},
        chinese_date{70, 57, 1, false, 29},
        chinese_date{72, 25, 4, true, 20},
        chinese_date{72, 57, 6, false, 5},
        chinese_date{73, 33, 6, false, 6},
        chinese_date{74, 25, 5, false, 5},
        chinese_date{75, 16, 6, false, 12},
        chinese_date{75, 36, 2, false, 13},
        chinese_date{76, 40, 3, false, 22},
        chinese_date{77, 6, 7, false, 21},
        chinese_date{77, 18, 8, false, 9},
        chinese_date{77, 20, 3, false, 15},
        chinese_date{77, 20, 9, false, 9},
        chinese_date{78, 9, 2, false, 14},
        chinese_date{78, 13, 1, false, 7},
        chinese_date{78, 55, 10, false, 14},
        chinese_date{79, 51, 6, false, 7}
    };

    static constexpr std::array names = {
        sexagesimal_name{2, 10},
        sexagesimal_name{8, 8},
        sexagesimal_name{4, 8},
        sexagesimal_name{2, 8},
        sexagesimal_name{2, 10},
        sexagesimal_name{10, 2},
        sexagesimal_name{2, 2},
        sexagesimal_name{5, 11},
        sexagesimal_name{10, 8},
        sexagesimal_name{10, 6},
        sexagesimal_name{10, 4},
        sexagesimal_name{5, 3},
        sexagesimal_name{2, 8},
        sexagesimal_name{5, 1},
        sexagesimal_name{1, 11},
        sexagesimal_name{1, 11},
        sexagesimal_name{3, 1},
        sexagesimal_name{2, 8},
        sexagesimal_name{1, 3},
        sexagesimal_name{9, 11},
        sexagesimal_name{1, 7},
        sexagesimal_name{9, 5},
        sexagesimal_name{9, 3},
        sexagesimal_name{6, 4},
        sexagesimal_name{4, 2},
        sexagesimal_name{9, 3},
        sexagesimal_name{7, 5},
        sexagesimal_name{4, 8},
        sexagesimal_name{5, 11},
        sexagesimal_name{9, 5},
        sexagesimal_name{9, 5},
        sexagesimal_name{8, 4},
        sexagesimal_name{7, 3}
    };

    static constexpr std::array<moment_t, 33> next_zhonggi = {
        -214191.633133,
    -61370.729673,
    25498.215092,
    49239.004860,
    171318.588098,
    210156.745715,
    253439.318423,
    369767.423610,
    400113.941895,
    434384.035740,
    452615.458234,
    470177.759817,
    473861.324287,
    507860.235360,
    524172.836499,
    544687.134951,
    567122.835546,
    569492.996631,
    601727.342101,
    613446.520844,
    626626.467423,
    645556.325334,
    664246.376294,
    671426.124336,
    694801.614262,
    704453.869487,
    708867.143728,
    709411.313394,
    709597.630490,
    727277.699910,
    728738.668683,
    744325.563213,
    764656.564371,

    };

    class chinese_based_test : public helpers::input_output_test<fixed_date_t, chinese_date> {
    protected:
        const chinese_calendar* m_calendar = nullptr;
    };

    class sexagesimal_name_test : public helpers::input_output_test<fixed_date_t, sexagesimal_name> {
    };

    class chinese_calendar_test : public chinese_based_test {
    protected:
        void SetUp() override {
            m_calendar = new chinese_calendar();
        }
        void TearDown() override {
            delete m_calendar;
            m_calendar = nullptr;
        }
    };

    class major_solar_term_test : public helpers::input_output_test<fixed_date_t, moment_t> {
    protected:
        const chinese_calendar* m_calendar = nullptr;

        void SetUp() override {
            m_calendar = new chinese_calendar();
        }

        void TearDown() override {
            delete m_calendar;
            m_calendar = nullptr;
        }
    };

    TEST_P(chinese_calendar_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }

    TEST_P(chinese_calendar_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    TEST_P(sexagesimal_name_test, name) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(chinese_calendar::day_name(input), output) << input << " : " << chinese_calendar::day_name(input).to_string() << " : " << output.to_string() << std::endl;
    }

    TEST_P(major_solar_term_test, next_zhonggi) {
        auto& [input, output] = GetParam();
        EXPECT_NEAR(m_calendar->major_solar_term_on_or_after(input), output, 1e-6) << input << " : " << m_calendar->major_solar_term_on_or_after(input) << " : " << output << std::endl;
    }

    INSTANTIATE_TEST_SUITE_P(chinese_calendar, chinese_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, cdates)));
    INSTANTIATE_TEST_SUITE_P(sexagesimal_name, sexagesimal_name_test, testing::ValuesIn(helpers::make_test_params_values(dates, names)));
    INSTANTIATE_TEST_SUITE_P(next_zhonggi, major_solar_term_test, testing::ValuesIn(helpers::make_test_params_values(dates, next_zhonggi)));

}