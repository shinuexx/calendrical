#include "fixed_date.hpp"

namespace calendrical {
	std::string to_string(const fixed_date_t& rd) {
		return std::to_string(rd.m_v) + "[RD]";
	}
}

std::ostream& operator<<(std::ostream& out, const calendrical::fixed_date_t& rd) {
	return out << calendrical::to_string(rd);
}