#include "datetypes/mayan_dates.hpp"

#include <sstream>

namespace calendrical::datetypes {
    std::string mayan_long_count::to_string() const {
        std::ostringstream out;
        out << "mayan_long_count{alautun:" << alautun
            << ",kinchiltun:" << kinchiltun
            << ",cakabtun:" << cakabtun
            << ",pictun:" << pictun
            << ",baktun:" << baktun
            << ",katun:" << katun
            << ",tun:" << tun
            << ",uinal:" << uinal
            << ",kin:" << kin << '}';
        return out.str();
    }

    std::ostream& operator<<(std::ostream& out, const mayan_long_count& date) {
        return out << date.to_string();
    }
}
