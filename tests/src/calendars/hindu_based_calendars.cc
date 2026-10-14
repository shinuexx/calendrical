#include <calendars/astro_hindu_lunar_calendar.hpp>
#include <calendars/modern_hindu_lunar_calendar.hpp>
#include <calendars/old_hindu_lunar_calendar.hpp>
#include <datetypes/lunar_date.hpp>
#include <gtest/gtest.h>
#include "calendars/astro_hindu_solar_calendar.hpp"
#include "calendars/modern_hindu_solar_calendar.hpp"
#include "calendars/old_hindu_solar_calendar.hpp"
#include "local_helpers.hpp"
#include "sample_fixed_dates.hpp"

namespace tests {
    using namespace calendrical::calendars;

    static const std::array ohs_dates{
        simple_date{2515, 5, 19},
        simple_date{2933, 9, 26},
        simple_date{3171, 7, 11},
        simple_date{3236, 7, 17},
        simple_date{3570, 10, 19},
        simple_date{3677, 2, 28},
        simple_date{3795, 8, 17},
        simple_date{4114, 1, 26},
        simple_date{4197, 2, 24},
        simple_date{4290, 12, 20},
        simple_date{4340, 12, 7},
        simple_date{4388, 12, 30},
        simple_date{4399, 1, 24},
        simple_date{4492, 3, 7},
        simple_date{4536, 10, 28},
        simple_date{4593, 1, 3},
        simple_date{4654, 6, 12},
        simple_date{4660, 11, 27},
        simple_date{4749, 3, 1},
        simple_date{4781, 3, 21},
        simple_date{4817, 4, 13},
        simple_date{4869, 3, 8},
        simple_date{4920, 4, 20},
        simple_date{4939, 12, 13},
        simple_date{5004, 1, 4},
        simple_date{5030, 5, 11},
        simple_date{5042, 6, 15},
        simple_date{5044, 1, 4},
        simple_date{5044, 6, 23},
        simple_date{5092, 12, 2},
        simple_date{5096, 11, 11},
        simple_date{5139, 7, 26},
        simple_date{5195, 4, 2}
    };

    static constexpr std::array mhs_dates{
        simple_date{-664, 5, 19},
        simple_date{-246, 9, 26},
        simple_date{-8, 7, 9},
        simple_date{57, 7, 16},
        simple_date{391, 10, 21},
        simple_date{498, 2, 31},
        simple_date{616, 8, 16},
        simple_date{935, 1, 28},
        simple_date{1018, 2, 26},
        simple_date{1111, 12, 23},
        simple_date{1161, 12, 10},
        simple_date{1210, 1, 2},
        simple_date{1220, 1, 27},
        simple_date{1313, 3, 8},
        simple_date{1357, 10, 30},
        simple_date{1414, 1, 5},
        simple_date{1475, 6, 10},
        simple_date{1481, 11, 29},
        simple_date{1570, 3, 3},
        simple_date{1602, 3, 22},
        simple_date{1638, 4, 13},
        simple_date{1690, 3, 10},
        simple_date{1741, 4, 20},
        simple_date{1760, 12, 16},
        simple_date{1825, 1, 7},
        simple_date{1851, 5, 10},
        simple_date{1863, 6, 14},
        simple_date{1865, 1, 7},
        simple_date{1865, 6, 21},
        simple_date{1913, 12, 4},
        simple_date{1917, 11, 13},
        simple_date{1960, 7, 24},
        simple_date{2016, 4, 2},
    };

    static constexpr std::array ahs_dates{
        simple_date{-664, 5, 13},
        simple_date{-246, 9, 21},
        simple_date{-8, 7, 5},
        simple_date{57, 7, 11},
        simple_date{391, 10, 17},
        simple_date{498, 2, 27},
        simple_date{616, 8, 13},
        simple_date{935, 1, 26},
        simple_date{1018, 2, 24},
        simple_date{1111, 12, 21},
        simple_date{1161, 12, 8},
        simple_date{1209, 12, 31},
        simple_date{1220, 1, 25},
        simple_date{1313, 3, 7},
        simple_date{1357, 10, 28},
        simple_date{1414, 1, 4},
        simple_date{1475, 6, 9},
        simple_date{1481, 11, 28},
        simple_date{1570, 3, 2},
        simple_date{1602, 3, 22},
        simple_date{1638, 4, 13},
        simple_date{1690, 3, 9},
        simple_date{1741, 4, 20},
        simple_date{1760, 12, 15},
        simple_date{1825, 1, 7},
        simple_date{1851, 5, 10},
        simple_date{1863, 6, 14},
        simple_date{1865, 1, 6},
        simple_date{1865, 6, 21},
        simple_date{1913, 12, 4},
        simple_date{1917, 11, 13},
        simple_date{1960, 7, 25},
        simple_date{2016, 4, 2},
    };

    static constexpr std::array ohl_dates{
        simple_lunar_date{2515, 6, false, 11},
        simple_lunar_date{2933, 9, false, 26},
        simple_lunar_date{3171, 8, false, 3},
        simple_lunar_date{3236, 8, false, 9},
        simple_lunar_date{3570, 11, true, 19},
        simple_lunar_date{3677, 3, false, 5},
        simple_lunar_date{3795, 9, false, 15},
        simple_lunar_date{4114, 2, false, 7},
        simple_lunar_date{4197, 2, false, 24},
        simple_lunar_date{4291, 1, false, 9},
        simple_lunar_date{4340, 12, false, 9},
        simple_lunar_date{4389, 1, false, 23},
        simple_lunar_date{4399, 2, false, 8},
        simple_lunar_date{4492, 4, false, 2},
        simple_lunar_date{4536, 11, false, 7},
        simple_lunar_date{4593, 1, false, 3},
        simple_lunar_date{4654, 7, false, 2},
        simple_lunar_date{4660, 11, false, 29},
        simple_lunar_date{4749, 3, false, 20},
        simple_lunar_date{4781, 4, false, 4},
        simple_lunar_date{4817, 5, false, 6},
        simple_lunar_date{4869, 4, false, 5},
        simple_lunar_date{4920, 5, false, 12},
        simple_lunar_date{4940, 1, true, 13},
        simple_lunar_date{5004, 1, false, 23},
        simple_lunar_date{5030, 5, false, 21},
        simple_lunar_date{5042, 7, false, 9},
        simple_lunar_date{5044, 1, false, 15},
        simple_lunar_date{5044, 7, false, 9},
        simple_lunar_date{5092, 12, false, 14},
        simple_lunar_date{5096, 12, false, 7},
        simple_lunar_date{5139, 8, false, 14},
        simple_lunar_date{5195, 4, false, 6},
    };

    static constexpr std::array mhl_dates{
        lunar_date{-529, 6, false, 11, false},
        lunar_date{-111, 9, false, 27, false},
        lunar_date{127, 8, false, 3, false},
        lunar_date{192, 8, false, 9, false},
        lunar_date{526, 11, false, 19, false},
        lunar_date{633, 3, false, 5, false},
        lunar_date{751, 9, false, 15, false},
        lunar_date{1070, 2, false, 6, false},
        lunar_date{1153, 3, true, 23, false},
        lunar_date{1247, 1, false, 8, false},
        lunar_date{1297, 1, false, 8, false},
        lunar_date{1345, 1, false, 22, false},
        lunar_date{1355, 2, false, 8, false},
        lunar_date{1448, 4, false, 1, false},
        lunar_date{1492, 11, false, 7, false},
        lunar_date{1549, 2, true, 3, false},
        lunar_date{1610, 7, false, 2, false},
        lunar_date{1616, 11, false, 28, true},
        lunar_date{1705, 3, false, 20, false},
        lunar_date{1737, 4, false, 4, false},
        lunar_date{1773, 5, false, 6, false},
        lunar_date{1825, 4, false, 5, false},
        lunar_date{1876, 5, false, 11, false},
        lunar_date{1896, 1, false, 13, false},
        lunar_date{1960, 1, false, 22, false},
        lunar_date{1986, 5, false, 20, false},
        lunar_date{1998, 7, false, 9, false},
        lunar_date{2000, 1, false, 14, false},
        lunar_date{2000, 7, false, 8, false},
        lunar_date{2048, 12, false, 14, false},
        lunar_date{2052, 12, false, 7, false},
        lunar_date{2095, 8, false, 14, false},
        lunar_date{2151, 4, false, 6, false},
    };

    static constexpr std::array ahl_dates{
        lunar_date{-529, 6, true, 11, false},
        lunar_date{-111, 9, false, 27, false},
        lunar_date{127, 8, false, 3, true},
        lunar_date{192, 8, false, 9, false},
        lunar_date{526, 10, false, 20, false},
        lunar_date{633, 3, false, 5, false},
        lunar_date{751, 8, false, 15, false},
        lunar_date{1070, 2, false, 6, false},
        lunar_date{1153, 2, false, 23, false},
        lunar_date{1247, 1, false, 8, false},
        lunar_date{1296, 12, false, 8, false},
        lunar_date{1345, 1, false, 23, false},
        lunar_date{1355, 2, false, 8, false},
        lunar_date{1448, 4, false, 1, false},
        lunar_date{1492, 11, false, 7, false},
        lunar_date{1549, 2, true, 4, false},
        lunar_date{1610, 7, false, 2, false},
        lunar_date{1616, 11, false, 29, false},
        lunar_date{1705, 3, false, 20, false},
        lunar_date{1737, 4, false, 5, false},
        lunar_date{1773, 5, false, 6, false},
        lunar_date{1825, 4, false, 5, false},
        lunar_date{1876, 5, false, 11, false},
        lunar_date{1896, 1, false, 13, false},
        lunar_date{1960, 1, false, 22, false},
        lunar_date{1986, 5, false, 20, false},
        lunar_date{1998, 7, false, 9, false},
        lunar_date{2000, 1, false, 14, false},
        lunar_date{2000, 7, false, 8, false},
        lunar_date{2048, 12, false, 14, false},
        lunar_date{2052, 12, false, 7, false},
        lunar_date{2095, 8, false, 14, false},
        lunar_date{2151, 4, false, 6, false},
    };

    class old_hindu_solar_calendar_test : public helpers::input_output_test<fixed_date_t, simple_date> {
    protected:
        std::unique_ptr<const old_hindu_solar_calendar> m_calendar = std::make_unique<old_hindu_solar_calendar>();
    };

    class modern_hindu_solar_calendar_test : public helpers::input_output_test<fixed_date_t, simple_date> {
    protected:
        std::unique_ptr<const modern_hindu_solar_calendar> m_calendar = std::make_unique<modern_hindu_solar_calendar>();
    };

    class astro_hindu_solar_calendar_test : public helpers::input_output_test<fixed_date_t, simple_date> {
    protected:
        std::unique_ptr<const astro_hindu_solar_calendar> m_calendar = std::make_unique<astro_hindu_solar_calendar>();
    };

    class old_hindu_lunar_calendar_test : public helpers::input_output_test<fixed_date_t, simple_lunar_date> {
    protected:
        std::unique_ptr<const old_hindu_lunar_calendar> m_calendar = std::make_unique<old_hindu_lunar_calendar>();
    };

    class modern_hindu_lunar_calendar_test : public helpers::input_output_test<fixed_date_t, lunar_date> {
    protected:
        std::unique_ptr<const modern_hindu_lunar_calendar> m_calendar = std::make_unique<modern_hindu_lunar_calendar>();
    };

    class astro_hindu_lunar_calendar_test : public helpers::input_output_test<fixed_date_t, lunar_date> {
    protected:
        std::unique_ptr<const astro_hindu_lunar_calendar> m_calendar = std::make_unique<astro_hindu_lunar_calendar>();
    };

    TEST_P(old_hindu_solar_calendar_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(old_hindu_solar_calendar_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    TEST_P(modern_hindu_solar_calendar_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(modern_hindu_solar_calendar_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    TEST_P(astro_hindu_solar_calendar_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(astro_hindu_solar_calendar_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    TEST_P(old_hindu_lunar_calendar_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(old_hindu_lunar_calendar_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    TEST_P(modern_hindu_lunar_calendar_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(modern_hindu_lunar_calendar_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : "  << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    TEST_P(astro_hindu_lunar_calendar_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }
    TEST_P(astro_hindu_lunar_calendar_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : "  << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    INSTANTIATE_TEST_CASE_P(old_hindu_solar_calendar, old_hindu_solar_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, ohs_dates)));
    INSTANTIATE_TEST_CASE_P(modern_hindu_solar_calendar, modern_hindu_solar_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, mhs_dates)));
    INSTANTIATE_TEST_CASE_P(astro_hindu_solar_calendar, astro_hindu_solar_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, ahs_dates)));

    INSTANTIATE_TEST_CASE_P(old_hindu_lunar_calendar, old_hindu_lunar_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, ohl_dates)));
    INSTANTIATE_TEST_CASE_P(modern_hindu_lunar_calendar, modern_hindu_lunar_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, mhl_dates)));
    INSTANTIATE_TEST_CASE_P(astro_hindu_lunar_calendar, astro_hindu_lunar_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, ahl_dates)));
}