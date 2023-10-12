#include "calendar_type.hpp"
#include <sstream>

namespace calendrical {
	std::string to_string(const calendrical::calendar_type& type)
	{
		std::ostringstream oss{};
		switch (type)
		{
		case calendrical::calendar_type::UNKNOWN: oss << "UNKNOWN"; break;
		case calendrical::calendar_type::SOLAR: oss << "SOLAR"; break;
		case calendrical::calendar_type::LUNAR: oss << "LUNAR"; break;
		case calendrical::calendar_type::LUNISOLAR: oss << "UNISOLAR"; break;
		default: oss << "calendar_type{" << static_cast<int32_t>(type) << '}'; break;
		}
		return oss.str();
	}
}

std::ostream& operator<<(std::ostream& out, const calendrical::calendar_type& type)
{
	return out << calendrical::to_string(type);
}
