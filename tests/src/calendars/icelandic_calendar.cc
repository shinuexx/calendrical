#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"
#include "calendars/icelandic_calendar.hpp"
#include "local_helpers.hpp"

namespace tests {
    using namespace calendrical::calendars;

    static const std::array idates{
        icelandic_date{-586, season{90}, 14, weekday{0}},
        icelandic_date{-168, season{270}, 6, weekday{3}},
        icelandic_date{70, season{90}, 22, weekday{3}},
        icelandic_date{135, season{90}, 24, weekday{0}},
        icelandic_date{469, season{270}, 11, weekday{3}},
        icelandic_date{576, season{90}, 4, weekday{1}},
        icelandic_date{694, season{270}, 3, weekday{6}},
        icelandic_date{1013, season{90}, 1, weekday{0}},
        icelandic_date{1096, season{90}, 5, weekday{0}},
        icelandic_date{1189, season{270}, 22, weekday{5}},
        icelandic_date{1239, season{270}, 21, weekday{6}},
        icelandic_date{1287, season{270}, 23, weekday{5}},
        icelandic_date{1298, season{90}, 1, weekday{0}},
        icelandic_date{1391, season{90}, 8, weekday{0}},
        icelandic_date{1435, season{270}, 15, weekday{3}},
        icelandic_date{1491, season{270}, 25, weekday{6}},
        icelandic_date{1553, season{90}, 22, weekday{6}},
        icelandic_date{1559, season{270}, 20, weekday{6}},
        icelandic_date{1648, season{90}, 7, weekday{3}},
        icelandic_date{1680, season{90}, 10, weekday{0}},
        icelandic_date{1716, season{90}, 14, weekday{5}},
        icelandic_date{1768, season{90}, 9, weekday{0}},
        icelandic_date{1819, season{90}, 15, weekday{1}},
        icelandic_date{1838, season{270}, 22, weekday{3}},
        icelandic_date{1902, season{270}, 26, weekday{0}},
        icelandic_date{1929, season{90}, 18, weekday{0}},
        icelandic_date{1941, season{90}, 23, weekday{1}},
        icelandic_date{1942, season{270}, 26, weekday{1}},
        icelandic_date{1943, season{90}, 25, weekday{4}},
        icelandic_date{1991, season{270}, 21, weekday{2}},
        icelandic_date{1995, season{270}, 18, weekday{0}},
        icelandic_date{2038, season{270}, 3, weekday{3}},
        icelandic_date{2094, season{90}, 13, weekday{0}}
    };

    class icelandic_calendar_test : public helpers::input_output_test<fixed_date_t, icelandic_date> {
    protected:
        const icelandic_calendar *m_calendar = nullptr;

        void SetUp() override {
            m_calendar = new icelandic_calendar();
        }

        void TearDown() override {
            delete m_calendar;
            m_calendar = nullptr;
        }
    };

    TEST_P(icelandic_calendar_test, to) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_date(input), output) << input << " : " << m_calendar->to_date(input).to_string() << " : " << output.to_string() << std::endl;
    }

    TEST_P(icelandic_calendar_test, from) {
        auto& [input, output] = GetParam();
        EXPECT_EQ(m_calendar->to_fixed(output), input) << output.to_string() << " : " << m_calendar->to_fixed(output) << " : " << input << std::endl;
    }

    INSTANTIATE_TEST_CASE_P(icelandic_calendar, icelandic_calendar_test, testing::ValuesIn(helpers::make_test_params_values(dates, idates)));
}