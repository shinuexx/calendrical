#include "roman_event.hpp"
#include <sstream>

namespace calendrical {
	std::string to_string(const roman_event& event)
	{
		std::ostringstream oss{};
		switch (event)
		{
		case roman_event::KALENDS: oss << "KALENDS"; break;
		case roman_event::NONES: oss << "NONES"; break;
		case roman_event::IDES: oss << "IDES"; break;
		default: oss << "roman_event{" << static_cast<int32_t>(event) << '}'; break;
		}
		return oss.str();
	}
}

std::ostream& operator<<(std::ostream& out, const calendrical::roman_event& event) {
	return out << calendrical::to_string(event);
}