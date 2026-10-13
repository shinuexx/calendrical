#include "lunar_phase.hpp"
#include <sstream>

namespace calendrical {
    std::string to_string(const lunar_phase& phase)
    {
        std::ostringstream oss{};
        switch (phase) {
        case lunar_phase::new_moon: oss << "new_moon"; break;
        case lunar_phase::first_quarter: oss << "first_quarter"; break;
        case lunar_phase::full_moon: oss << "full_moon"; break;
        case lunar_phase::last_quarter: oss << "last_quarter"; break;
        default: oss << "lunar_phase{" << static_cast<int32_t>(phase) << '}'; break;
        }
        return oss.str();
    }

    std::ostream& operator<<(std::ostream& out, const lunar_phase& phase)
    {
        return out << to_string(phase);
    }
}
