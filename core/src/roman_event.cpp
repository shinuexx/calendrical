#include "roman_event.hpp"
#include <sstream>

namespace calendrical {
    std::string to_string(const roman_event& event)
    {
        std::ostringstream oss{};
        switch (event)
        {
        case roman_event::kalends: oss << "kalends"; break;
        case roman_event::nones: oss << "nones"; break;
        case roman_event::ides: oss << "ides"; break;
        default: oss << "roman_event{" << static_cast<int32_t>(event) << '}'; break;
        }
        return oss.str();
    }
}

std::ostream& operator<<(std::ostream& out, const calendrical::roman_event& event) {
    return out << to_string(event);
}
