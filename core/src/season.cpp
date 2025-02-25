#include "season.hpp"
#include <sstream>

namespace calendrical {
    std::string to_string(const season& s) {
        std::ostringstream oss{};
        switch (s)
        {
        case season::SPRING: oss << "SPRING"; break;
        case season::SUMMER: oss << "SUMMER"; break;
        case season::AUTUMN: oss << "AUTUMN"; break;
        case season::WINTER: oss << "WINTER"; break;
        default: oss << "season{" << static_cast<int>(s) << '}'; break;
        }
        return oss.str();
    }
}

std::ostream& operator<<(std::ostream& out, const calendrical::season& s) {
    return out << to_string(s);
}