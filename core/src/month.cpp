#include "month.hpp"
#include <sstream>

namespace calendrical {
    std::string to_string(const month& m) {
        std::ostringstream oss{};
        switch (m)
        {
        case month::january:
            oss << "january"; break;
        case month::february:
            oss << "february"; break;
        case month::march:
            oss << "march"; break;
        case month::april:
            oss << "april"; break;
        case month::may:
            oss << "may"; break;
        case month::june:
            oss << "june"; break;
        case month::july:
            oss << "july"; break;
        case month::august:
            oss << "august"; break;
        case month::september:
            oss << "september"; break;
        case month::october:
            oss << "october"; break;
        case month::november:
            oss << "november"; break;
        case month::december:
            oss << "december"; break;
        default:
            oss << "month{" << static_cast<int>(m) + '}'; break;
        }
        return oss.str();
    }
}


std::ostream& operator<<(std::ostream& out, const calendrical::month& m) {
    return out << to_string(m);
}