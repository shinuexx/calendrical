#include "fixed_date.hpp"

namespace calendrical {
    std::string fixed_date_t::to_string() const {
        return std::to_string(m_v) + " [RD]";
    }

    std::ostream& operator<<(std::ostream& out, const fixed_date_t& rd){
        return out << rd.to_string();
    }
}