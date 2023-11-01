#include "small_rational.hpp"
#include <sstream>

namespace calendrical {
    std::string small_rational::to_string() const
    {
        std::ostringstream oss{};
        oss << m_n << '/' << m_d;
        return oss.str();
    }
}

std::ostream& operator<<(std::ostream& out, const calendrical::small_rational& sr) {
    return out << sr.to_string();
}