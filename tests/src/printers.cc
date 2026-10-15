#include "printers.hpp"

namespace calendrical{
    namespace datetypes {
        void PrintTo(const simple_date& date, std::ostream* os) { *os << date.to_string(); }
        void PrintTo(const gregorian_date& dates, std::ostream* os) { *os << dates.to_string(); }
        void PrintTo(const julian_date& dates, std::ostream* os) { *os << dates.to_string(); }
        void PrintTo(const roman_date& dates, std::ostream* os) { *os << dates.to_string(); }
        void PrintTo(const olympiad &dates, std::ostream *os) { *os << to_string(dates); }
        void PrintTo(const iso_date &dates, std::ostream *os) { *os << dates.to_string(); }
        void PrintTo(const icelandic_date &dates, std::ostream *os) {*os << dates.to_string(); }
        void PrintTo(const hebrew_date &dates, std::ostream *os) {*os << dates.to_string(); }
        void PrintTo(const chinese_date& dates, std::ostream *os) { *os << dates.to_string(); }
        void PrintTo(const sexagesimal_name& dates, std::ostream *os) { *os << dates.to_string(); }
        void PrintTo(const akan_name& name, std::ostream *os) { *os << name.to_string(); }
        void PrintTo(const simple_lunar_date& dates, std::ostream *os) { *os << dates.to_string(); }
        void PrintTo(const bahai_date &dates, std::ostream *os) { *os << dates.to_string(); }
        void PrintTo(const lunar_date &dates, std::ostream *os) { *os << dates.to_string(); }
    } // namespace datetypes
    void PrintTo(const weekday &wd, std::ostream *os) { *os << wd; }
    void PrintTo(const fixed_date_t &date, std::ostream *os) { *os << date; }
} // namespace calendrical
