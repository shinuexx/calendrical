#include "lunar_phase.hpp"
#include <sstream>

namespace calendrical {
    std::string to_string(const lunar_phase& phase)
    {
        std::ostringstream oss{};
        switch (phase) {
        case lunar_phase::NEW: oss << "NEW"; break;
        case lunar_phase::FIRST_QUARTER: oss << "FIRST_QUARTER"; break;
        case lunar_phase::FULL: oss << "FULL"; break;
        case lunar_phase::LAST_QUARTER: oss << "LAST_QUARTER"; break;
        default: oss << "lunar_phase{" << static_cast<int32_t>(phase) << '}'; break;
        }
        return oss.str();
    }
}

std::ostream& operator<<(std::ostream& out, const calendrical::lunar_phase& phase)
{
    return out << to_string(phase);
}
