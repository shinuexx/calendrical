#pragma once

#include "interfaces.hpp"
#include "month.hpp"
#include "roman_event.hpp"

namespace calendrical::datetypes {
    struct roman_date :
        public has_year_if,
        public has_month_if<month>,
        public date_if{

        roman_event event;
        int32_t count;
        bool leap;

        constexpr roman_date(int32_t year, calendrical::month month, roman_event event, int32_t count, bool leap) :
            has_year_if{ year },
            has_month_if{ month },
            event{ event },
            count{ count },
            leap{ leap },
            date_if {} {}

        constexpr bool operator==(const roman_date& other) const {
            return (year == other.year)
                && (month == other.month)
                && (event == other.event)
                && (count == other.count)
                && (leap == other.leap);
        }

        virtual std::string to_string() const override;
        virtual ~roman_date() = default;

        constexpr auto tie() const { return std::tie(year, month, event, count, leap); }
        constexpr auto tie() { return std::tie(year, month, event, count, leap); }
        constexpr auto tuple() const { return std::make_tuple(year, month, event, count, leap); }
    };

    struct olympiad {
        int32_t cycle, year;
    };

    std::string to_string(const olympiad& odate);
}

std::ostream& operator<<(std::ostream& out, const calendrical::datetypes::olympiad& odate);