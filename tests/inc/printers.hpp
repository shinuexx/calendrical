#pragma once

#include <gtest/gtest.h>
#include "datetypes/gregorian_date.hpp"
#include "datetypes/iso_date.hpp"
#include "datetypes/julian_date.hpp"
#include "datetypes/roman_date.hpp"
#include "datetypes/simple_date.hpp"
#include "datetypes/simple_lunar_date.hpp"
#include "datetypes/chinese_date.hpp"
#include "datetypes/bahai_date.hpp"
#include "weekday.hpp"

#include <datetypes/icelandic_date.hpp>
#include "datetypes/hebrew_date.hpp"

#include <datetypes/lunar_date.hpp>
#include <datetypes/sexagesimal_name.hpp>
#include <datetypes/akan_name.hpp>

namespace calendrical{
    namespace datetypes {
        void PrintTo(const simple_date& date, std::ostream* os);
        void PrintTo(const gregorian_date& dates, std::ostream* os);
        void PrintTo(const julian_date& dates, std::ostream* os);
        void PrintTo(const roman_date& dates, std::ostream* os);
        void PrintTo(const olympiad& dates, std::ostream* os);
        void PrintTo(const iso_date& dates, std::ostream* os);
        void PrintTo(const icelandic_date& dates, std::ostream* os);
        void PrintTo(const hebrew_date &dates, std::ostream *os);
        void PrintTo(const chinese_date& dates, std::ostream *os);
        void PrintTo(const sexagesimal_name& dates, std::ostream *os);
        void PrintTo(const akan_name& name, std::ostream *os);
        void PrintTo(const simple_lunar_date &dates, std::ostream *os);
        void PrintTo(const bahai_date &dates, std::ostream *os);
        void PrintTo(const lunar_date &dates, std::ostream *os);
    }
    void PrintTo(const weekday& wd, std::ostream* os);
    void PrintTo(const fixed_date_t& date, std::ostream* os);
}
