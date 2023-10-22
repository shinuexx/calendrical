#include "range.hpp"
#include <sstream>

namespace calendrical {
    std::string to_string(const range_type& type) {
        std::ostringstream oss{};
        switch (type)
        {
        case range_type::opened: oss << "opened"; break;
        case range_type::closed: oss << "closed"; break;
        case range_type::left_closed: oss << "left_closed"; break;
        case range_type::right_closed: oss << "right_closed"; break;
        default: oss << "range_type{" << static_cast<int>(type) << '}'; break;
        }
        return oss.str();
    }
}