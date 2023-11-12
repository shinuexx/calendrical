#include "datetypes/simple_date.hpp"
#include "datetypes/gregorian_date.hpp"
#include "datetypes/julian_date.hpp"
#include "datetypes/roman_date.hpp"
#include "datetypes/iso_date.hpp"
#include "datetypes/icelandic_date.hpp"
#include "datetypes/hebrew_date.hpp"
#include "datetypes/simple_lunar_date.hpp"
#include "datetypes/bahai_date.hpp"
#include "datetypes/chinese_date.hpp"
#include "datetypes/lunar_date.hpp"
#include "datetypes/sexagesimal_name.hpp"
#include <sstream>

namespace calendrical::datetypes {
    std::string simple_date::to_string() const {
        std::ostringstream oss{};
        oss << "simple_date{year:" << year << ",month:" << month << ",day:" << day << '}';
        return oss.str();
    }
    std::string gregorian_date::to_string() const {
        std::ostringstream oss{};
        oss << "gregorian_date{year:" << year << ",month:" << calendrical::to_string(month) << ",day:" << day << '}';
        return oss.str();
    }
    std::string julian_date::to_string() const {
        std::ostringstream oss{};
        oss << "julian_date{year:" << year << ",month:" << calendrical::to_string(month) << ",day:" << day << '}';
        return oss.str();
    }
    std::string roman_date::to_string() const {
        std::ostringstream oss{};
        oss << "roman_date{year:" << year << ",month:" << calendrical::to_string(month) << ",event:" << calendrical::to_string(event) << ",count:" << count << ",leap:" << std::boolalpha << leap << '}';
        return oss.str();
    }
    std::string iso_date::to_string() const {
        std::ostringstream oss{};
        oss << "iso_date{year:" << year << ",week:" << week << ",day:" << day << '}';
        return oss.str();
    }
    std::string icelandic_date::to_string() const {
        std::ostringstream oss{};
        oss << "icelandic_date{year:" << year << ",season:" << calendrical::to_string(season) << ",week:" << week << ",weekday:" << weekday << '}';
        return oss.str();
    }
    std::string hebrew_date::to_string() const {
        std::ostringstream oss{};
        oss << "hebrew_date{year:" << year << ",month:" << calendrical::to_string(month) << ",day:" << day << '}';
        return oss.str();
    }
    std::string simple_lunar_date::to_string() const {
        std::ostringstream oss{};
        oss << "simple_lunar_date{year:" << year << ",month:" << month << ",leap:" << std::boolalpha << leap_month << ",day:" << day << '}';
        return oss.str();
    }
    std::string bahai_date::to_string() const {
        std::ostringstream oss{};
        oss << "bahai_date{major:" << major << ",cycle:" << cycle << ",year:" << year << ",month:" << month << ",day:" << day << '}';
        return oss.str();
    }
    std::string lunar_date::to_string() const {
        std::ostringstream oss{};
        oss << "lunar_date{year:" << year << ",month:" << month << ",leap_month:" << std::boolalpha << leap_month << ",day:" << day << ",leap_day:" << std::boolalpha << leap_day << '}';
        return oss.str();
    }
    std::string chinese_date::to_string() const {
        std::ostringstream oss{};
        oss << "chinese_date{cycle:" << cycle << ", year:" << year << ", month:" << month << ",leap_month:" << std::boolalpha << leap_month << ",day:" << day << '}';
        return oss.str();
    }
    std::string to_string(const olympiad& odate) {
        std::ostringstream oss{};
        oss << "olympiad{cycle:" << odate.cycle << ",year:" << odate.year << "}";
        return oss.str();
    }

    sexagesimal_name sexagesimal_name::from(int64_t n) {
        return {
            (int32_t)adjusted_remainder(n, 10),
            (int32_t)adjusted_remainder(n, 12),
        };
    }

    int32_t sexagesimal_name::difference(const sexagesimal_name& date) const {
        int64_t stem_difference = (int64_t)date.stem - stem;
        auto branch_difference = date.branch - branch;
        return (int32_t)adjusted_remainder(stem_difference + 25 * (branch_difference - stem_difference), 60);
    }

    std::string sexagesimal_name::to_string() const
    {
        std::ostringstream oss{};
        oss << "sexagesimal_name{stem:" << stem << ",branch:" << branch << "}";
        return oss.str();
    }
}

std::ostream& operator<<(std::ostream& out, const calendrical::datetypes::olympiad& odate) {
    return out << calendrical::datetypes::to_string(odate);
}
std::ostream& operator<<(std::ostream& out, const calendrical::datetypes::sexagesimal_name& odate) {
    return out << odate.to_string();
}
