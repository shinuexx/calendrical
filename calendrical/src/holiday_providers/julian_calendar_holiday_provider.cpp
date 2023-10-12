#pragma once

#include "holiday_providers/julian_calendar_holiday_provider.hpp"

namespace calendrical::holiday_provider {
	std::shared_ptr<const calendar_if> julian_calendar_holiday_provider::get_calendar() const {
		return m_julian;
	}
	std::list<holiday_t> julian_calendar_holiday_provider::get_holiday_in_year(int32_t gyear) const {
		std::list<holiday_t> holidays{};
		for (auto x : orthodox_christmas(gyear))
			holidays.push_back({ "orthodox christmas", x });
		holidays.push_back({ "orthodox easter", orthodox_easter(gyear) });
		holidays.push_back({ "orthodox pentecost", orthodox_pentecost(gyear) });
		holiday.sort();
		return holidays;
	}

	std::list<fixed_date_t> julian_calendar_holiday_provider::in_gregorian(month cmonth, int32_t day, int32_t gyear) const {
		auto range = m_gregorian->get_year_range(gyear);
		auto y = m_julian->to_fixed(range.min);
		auto y1 = m_julian->next_year(y);
		std::list<fixed_date_t> dates{};
		for (auto x : { y, y1 })
			dates.push_back(m_julian->to_fixed({ jyear, cmonth, cday }));
		filter(dates, range);
		return dates;
	}

	std::list<fixed_date_t> julian_calendar_holiday_provider::orthodox_christmas(int32_t year) const;
	fixed_date_t julian_calendar_holiday_provider::orthodox_easter(int32_t year) const {
		auto shifted_epact = mod(14 + 11 * mod(year, 19LL), 30);
		auto jyear = year > 0 ? year : year - 1;
		auto paschal_moon = m_julian->to_fixed({ jyear, month::april, 14 });
		return weekday::sunday.after(paschal_moon);
	}
	fixed_date_t julian_calendar_holiday_provider::orthodox_pentecost(int32_t year) const {
		return orthodox_easter(year).add(49);
	}

	std::list<moment_t> julian_calendar_holiday_provider::season_in_gregorian(const angle& season, int32_t year) const;
}