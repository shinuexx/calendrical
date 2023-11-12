#include <gtest/gtest.h>
#include "sample_fixed_dates.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace tests {
    using namespace calendrical::calendars;

    static const std::array<gregorian_date, 33> gdates{
        gregorian_date{-586,calendrical::month{7},24},
        gregorian_date{-168,calendrical::month{12},5},
        gregorian_date{70,calendrical::month{9},24},
        gregorian_date{135,calendrical::month{10},2},
        gregorian_date{470,calendrical::month{1},8},
        gregorian_date{576,calendrical::month{5},20},
        gregorian_date{694,calendrical::month{11},10},
        gregorian_date{1013,calendrical::month{4},25},
        gregorian_date{1096,calendrical::month{5},24},
        gregorian_date{1190,calendrical::month{3},23},
        gregorian_date{1240,calendrical::month{3},10},
        gregorian_date{1288,calendrical::month{4},2},
        gregorian_date{1298,calendrical::month{4},27},
        gregorian_date{1391,calendrical::month{6},12},
        gregorian_date{1436,calendrical::month{2},3},
        gregorian_date{1492,calendrical::month{4},9},
        gregorian_date{1553,calendrical::month{9},19},
        gregorian_date{1560,calendrical::month{3},5},
        gregorian_date{1648,calendrical::month{6},10},
        gregorian_date{1680,calendrical::month{6},30},
        gregorian_date{1716,calendrical::month{7},24},
        gregorian_date{1768,calendrical::month{6},19},
        gregorian_date{1819,calendrical::month{8},2},
        gregorian_date{1839,calendrical::month{3},27},
        gregorian_date{1903,calendrical::month{4},19},
        gregorian_date{1929,calendrical::month{8},25},
        gregorian_date{1941,calendrical::month{9},29},
        gregorian_date{1943,calendrical::month{4},19},
        gregorian_date{1943,calendrical::month{10},7},
        gregorian_date{1992,calendrical::month{3},17},
        gregorian_date{1996,calendrical::month{2},25},
        gregorian_date{2038,calendrical::month{11},10},
        gregorian_date{2094,calendrical::month{7},18}
    };

    TEST(gregorian_calendar, valid_shared) {
        auto gc = gregorian_calendar::standard;
        for (std::size_t i = 0; i < dates.size(); ++i) {
            EXPECT_TRUE(gc.to_date(dates[i]) == gdates[i]) << i << " : " << dates[i] << " : " << gc.to_date(dates[i]).to_string() << " : " << gdates[i].to_string() << std::endl;
            EXPECT_TRUE(gc.to_fixed(gdates[i]) == dates[i]) << i << " : " << gdates[i].to_string() << " : " << gc.to_fixed(gdates[i]) << " : " << dates[i] << std::endl;
        }
    }
}