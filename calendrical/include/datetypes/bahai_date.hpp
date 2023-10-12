#pragma once

#include "interfaces.hpp"

namespace calendrical::datetypes {
	struct bahai_date :
		public has_major_if,
		public has_cycle_if,
		public has_year_if,
		public has_month_if<int32_t>,
		public has_day_if,
		public date_if {

		constexpr bahai_date(int32_t major, int32_t cycle, int32_t year, int32_t month, int32_t day) : 
			has_major_if{major},
			has_cycle_if{cycle},
			has_year_if{year},
			has_month_if<int32_t>{month},
			has_day_if{day},
			date_if{}{}

		constexpr bool operator==(const bahai_date& other) const {
			return (major == other.major)
				&& (cycle == other.cycle)
				&& (year == other.year)
				&& (month == other.month)
				&& (day == other.day);
		}

		virtual std::string to_string() const override;
		virtual ~bahai_date() = default;

		constexpr auto tie() const { return std::tie(major, cycle, year, month, day); }
		constexpr auto tie() { return std::tie(major, cycle, year, month, day); }
		constexpr auto tuple() const { return std::make_tuple(major, cycle, year, month, day); }
	};
}