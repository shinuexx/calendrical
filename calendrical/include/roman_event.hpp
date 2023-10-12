#pragma once

#include <string>
#include <iostream>

namespace calendrical {
	enum class roman_event {
		KALENDS = 1,
		NONES = 2,
		IDES = 3
	};

	std::string to_string(const roman_event& event);
}

std::ostream& operator<<(std::ostream& out, const calendrical::roman_event& event);