#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"
#include "calendars/roman_calendar.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::calendars;

    static const std::array gdates{
        roman_date{-587, month{8}, roman_event{1}, 3, false},
        roman_date{-169, month{12}, roman_event{3}, 6, false},
        roman_date{70, month{10}, roman_event{1}, 6, false},
        roman_date{135, month{10}, roman_event{2}, 5, false},
        roman_date{470, month{1}, roman_event{3}, 7, false},
        roman_date{576, month{6}, roman_event{1}, 15, false},
        roman_date{694, month{11}, roman_event{3}, 7, false},
        roman_date{1013, month{5}, roman_event{1}, 13, false},
        roman_date{1096, month{6}, roman_event{1}, 15, false},
        roman_date{1190, month{4}, roman_event{1}, 17, false},
        roman_date{1240, month{3}, roman_event{2}, 5, false},
        roman_date{1288, month{4}, roman_event{1}, 7, false},
        roman_date{1298, month{5}, roman_event{1}, 12, false},
        roman_date{1391, month{6}, roman_event{2}, 2, false},
        roman_date{1436, month{2}, roman_event{1}, 8, false},
        roman_date{1492, month{4}, roman_event{1}, 2, false},
        roman_date{1553, month{9}, roman_event{3}, 5, false},
        roman_date{1560, month{3}, roman_event{1}, 6, false},
        roman_date{1648, month{6}, roman_event{1}, 2, false},
        roman_date{1680, month{7}, roman_event{1}, 12, false},
        roman_date{1716, month{7}, roman_event{3}, 3, false},
        roman_date{1768, month{6}, roman_event{3}, 6, false},
        roman_date{1819, month{8}, roman_event{1}, 12, false},
        roman_date{1839, month{3}, roman_event{3}, 1, false},
        roman_date{1903, month{4}, roman_event{3}, 8, false},
        roman_date{1929, month{8}, roman_event{3}, 2, false},
        roman_date{1941, month{10}, roman_event{1}, 16, false},
        roman_date{1943, month{4}, roman_event{3}, 8, false},
        roman_date{1943, month{10}, roman_event{1}, 8, false},
        roman_date{1992, month{3}, roman_event{2}, 4, false},
        roman_date{1996, month{2}, roman_event{3}, 2, false},
        roman_date{2038, month{11}, roman_event{1}, 5, false},
        roman_date{2094, month{7}, roman_event{2}, 3, false},
    };

    static constexpr std::array olympiads{
        olympiad{48, 2},
        olympiad{152, 4},
        olympiad{212, 2},
        olympiad{228, 3},
        olympiad{312, 2},
        olympiad{338, 4},
        olympiad{368, 2},
        olympiad{448, 1},
        olympiad{468, 4},
        olympiad{492, 2},
        olympiad{504, 4},
        olympiad{516, 4},
        olympiad{519, 2},
        olympiad{542, 3},
        olympiad{553, 4},
        olympiad{567, 4},
        olympiad{583, 1},
        olympiad{584, 4},
        olympiad{606, 4},
        olympiad{614, 4},
        olympiad{623, 4},
        olympiad{636, 4},
        olympiad{649, 3},
        olympiad{654, 3},
        olympiad{670, 3},
        olympiad{677, 1},
        olympiad{680, 1},
        olympiad{680, 3},
        olympiad{680, 3},
        olympiad{692, 4},
        olympiad{693, 4},
        olympiad{704, 2},
        olympiad{718, 2}
    };

    class roman_calendar_test : public helpers::input_output_test<fixed_date_t, roman_date> {
    protected:
        const roman_calendar* m_calendar = nullptr;

        void SetUp() override {
            m_calendar = new roman_calendar();
        }

        void TearDown() override {
            delete m_calendar;
            m_calendar = nullptr;
        }
    };

    TEST_P(roman_calendar_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(roman_calendar_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    class olympiad_test : public helpers::input_output_test<fixed_date_t, olympiad> {
    protected:
        const roman_calendar* m_calendar = nullptr;

        void SetUp() override {
            m_calendar = new roman_calendar();
        }

        void TearDown() override {
            delete m_calendar;
            m_calendar = nullptr;
        }
    };

    TEST_P(olympiad_test, to) {
        auto &[input, output] = GetParam();
        const auto date = m_calendar->to_date(input);
        const auto y = date.year;
        EXPECT_EQ(m_calendar->olympiad_from_julian_year(y), output) << input << " : " << to_string(m_calendar->olympiad_from_julian_year(y)) << " : " << to_string(output) << std::endl;
    }
    TEST_P(olympiad_test, from) {
        auto &[input, output] = GetParam();
        const auto date = m_calendar->to_date(input);
        const auto y = date.year;
        EXPECT_EQ(m_calendar->julian_year_from_olympiad(output), y) << to_string(output) << " : " << m_calendar->julian_year_from_olympiad(output) << " : " << input << std::endl;
    }

    INSTANTIATE_TEST_CASE_P(roman_calendar, roman_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, gdates)));
    INSTANTIATE_TEST_CASE_P(olympiad, olympiad_test, testing::ValuesIn(helpers::make_test_params_values(dates, olympiads)));
}