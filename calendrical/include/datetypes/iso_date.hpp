#pragma once

#include "interfaces.hpp"

namespace calendrical::datetypes {
	struct iso_date :
		public has_year_if,
		public has_week_if,
		public has_day_if,
		public date_if {

		constexpr iso_date(int32_t year, int32_t week, int32_t day) :
			has_year_if{ year },
			has_week_if{ week },
			has_day_if{ day },
			date_if{} {}

		constexpr bool operator==(const iso_date& other) const {
			return (year == other.year)
				&& (week == other.week)
				&& (day == other.day);
		}

		virtual std::string to_string() const override;
		virtual ~iso_date() = default;

		constexpr auto tie() const { return std::tie(year, week, day); }
		constexpr auto tie() { return std::tie(year, week, day); }
		constexpr auto tuple() const { return std::make_tuple(year, week, day); }
	};
}