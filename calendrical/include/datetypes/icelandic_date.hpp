#pragma once

#include "interfaces.hpp"
#include "season.hpp"
#include "weekday.hpp"

namespace calendrical::datetypes {
	struct icelandic_date :
		public has_year_if,
		public has_week_if,
		public date_if {

		calendrical::season season;
		calendrical::weekday weekday;

		constexpr icelandic_date(int32_t year, calendrical::season season, int32_t week, calendrical::weekday weekday) :
			has_year_if{ year },
			season{season},
			has_week_if{ week },
			weekday{weekday},
			date_if{} {}

		constexpr bool operator==(const icelandic_date& other) const {
			return (year == other.year)
				&& (season == other.season)
				&& (week == other.week)
				&& (weekday == other.weekday);
		}

		virtual std::string to_string() const override;
		virtual ~icelandic_date() = default;

		constexpr auto tie() const { return std::tie(year, season, week, weekday); }
		constexpr auto tie() { return std::tie(year, season, week, weekday); }
		constexpr auto tuple() const { return std::make_tuple(year, season, week, weekday); }
	};
}