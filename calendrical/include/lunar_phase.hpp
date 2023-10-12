#pragma once

#include <string>
#include <iostream>

namespace calendrical {
	enum class lunar_phase {
		NEW = 0,
		FIRST_QUARTER = 90,
		FULL = 180,
		LAST_QUARTER = 270
	};

	std::string to_string(const lunar_phase& phase);
}

std::ostream& operator<<(std::ostream& out, const calendrical::lunar_phase& phase);