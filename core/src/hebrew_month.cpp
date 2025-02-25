#include "hebrew_month.hpp"
#include <sstream>

namespace calendrical {
    std::string to_string(const hebrew_month& m) {
        std::ostringstream oss{};
        switch (m)
        {
        case hebrew_month::nisan: oss << "nisan"; break;
        case hebrew_month::iyyar: oss << "iyyar"; break;
        case hebrew_month::sivan: oss << "sivan"; break;
        case hebrew_month::tammuz: oss << "tammuz"; break;
        case hebrew_month::av: oss << "av"; break;
        case hebrew_month::elul: oss << "elul"; break;
        case hebrew_month::tishri: oss << "tishri"; break;
        case hebrew_month::marheshvan: oss << "marheshvan"; break;
        case hebrew_month::kislev: oss << "kislev"; break;
        case hebrew_month::tevet: oss << "tevet"; break;
        case hebrew_month::shevat: oss << "shevat"; break;
        case hebrew_month::adar: oss << "adar"; break;
        case hebrew_month::adarii: oss << "adarii"; break;
        default: oss << "hebrew_month{" << static_cast<int>(m) << '}'; break;
        }
        return oss.str();
    }
}

std::ostream& operator<<(std::ostream& out, const calendrical::hebrew_month& m) {
    return out << to_string(m);
}