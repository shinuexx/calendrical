#include "holiday_providers/gregorian_calendar_holiday_provider.hpp"

namespace calendrical::holiday_providers {
	std::shared_ptr<const calendar_if> gregorian_calendar_holiday_provider::get_calendar() const {
		return m_gregorian;
	}
	std::list<holiday_t> gregorian_calendar_holiday_provider::get_holiday_in_year(int32_t gyear) const {
		std::list<holiday_t> holidays{};
		holidays.push_back({ "new year", m_gregorian->get_new_year(gyear) });
		holidays.push_back({ "christmas", christmas(gyear) });
		holidays.push_back({ "advent", advent(gyear) });
		holidays.push_back({ "epiphany", epiphany(gyear) });
		holidays.push_back({ "easter", easter(gyear) });
		holidays.push_back({ "pentecost", pentecost(gyear) });
		holidays.push_back({ "astronomical easter", astronomical_easter(gyear) });
		for (auto x : unlucky_fridays(gyear))
			holidays.push_back({ "unlucky friday", x });
		holidays.sort();
		return holidays;
	}

	fixed_date_t gregorian_calendar_holiday_provider::christmas(int32_t year) const {
		return m_gregorian->to_fixed({ year, month::december, 25 });
	}
	fixed_date_t gregorian_calendar_holiday_provider::advent(int32_t year) const {
		return weekday::sunday.nearest(m_gregorian->to_fixed({ year, month::november, 30 }));
	}
	fixed_date_t gregorian_calendar_holiday_provider::epiphany(int32_t year) const {
		return m_gregorian->first_weekday(weekday::sunday, { year, month::january, 2 });
	}
	fixed_date_t gregorian_calendar_holiday_provider::easter(int32_t year) const {
		auto century = floor_div(year, 100LL) + 1;
		auto shifted_epact = mod(14 + 11 * mod(year, 19LL) - floor_div(3 * century, 4) + floor_div(5 + 8 * century, 25), 30);
		auto adjusted_epact = shifted_epact == 0 || (shifted_epact == 1 && 10 < mod(year, 19LL)) ? shifted_epact + 1 : shifted_epact;
		auto paschal_moon = m_gregorian->to_fixed({ year, month::april, 19 }) - adjusted_epact;
		return weekday::sunday.after(paschal_moon);
	}

	fixed_date_t gregorian_calendar_holiday_provider::pentecost(int32_t year) const {
		return easter(year).add(49);
	}
	std::list<fixed_date_t> gregorian_calendar_holiday_provider::unlucky_fridays_in_range(const range<fixed_date_t>& range) const {
		std::list<fixed_date_t> fridays{};
		for (auto fri = weekday::friday.on_or_after(range.min); fri < range.max; fri = weekday::friday.on_or_after(fri + 1))
			if (m_gregorian->to_date(fri).day == 13)
				fridays.push_back(fri);
		fridays.sort();
		return fridays;
	}

	std::list<fixed_date_t> gregorian_calendar_holiday_provider::unlucky_fridays(int32_t year) const {
		return unlucky_fridays_in_range(m_gregorian->get_year_range(year));
	}

	fixed_date_t gregorian_calendar_holiday_provider::astronomical_easter(int32_t year) const {
		return {};
	}
}