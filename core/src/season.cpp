#include "season.hpp"
#include <sstream>

namespace calendrical {
    std::string to_string(const season& s) {
        std::ostringstream oss{};
        switch (s)
        {
        case season::spring: oss << "spring"; break;
        case season::summer: oss << "summer"; break;
        case season::autumn: oss << "autumn"; break;
        case season::winter: oss << "winter"; break;
        default: oss << "season{" << static_cast<int>(s) << '}'; break;
        }
        return oss.str();
    }
}

std::ostream& operator<<(std::ostream& out, const calendrical::season& s) {
    return out << to_string(s);
}
