#pragma once

#include <string>
#include <iostream>

namespace calendrical {
	enum class hebrew_month {
		nisan = 1,
		iyyar,
		sivan,
		tammuz,
		av,
		elul,
		tishri,
		marheshvan,
		kislev,
		tevet,
		shevat,
		adar,
		adarii
	};

	std::string to_string(const hebrew_month& m);
}

std::ostream& operator<<(std::ostream& out, const calendrical::hebrew_month& m);