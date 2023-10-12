#include "holiday_providers/coptic_based_calendar_holiday_provider.hpp"

namespace calendrical::holiday_providers {
	std::shared_ptr<const calendar_if> coptic_based_calendar_holiday_provider::get_calendar() const {
		return m_coptic;
	}
	std::list<holiday_t> coptic_based_calendar_holiday_provider::get_holiday_in_year(int32_t gyear) const {
		std::list<holiday_t> holidays{};
		for (auto x : christmas(gyear))
			holidays.push_back({ "Christmas", x });
		holidays.sort();
		return holidays;
	}

	std::list<fixed_date_t> coptic_based_calendar_holiday_provider::in_gregorian(int32_t cmonth, int32_t day, int32_t gyear) const {
		auto range = m_gregorian->get_year_range(gyear);
		auto y = m_coptic->get_year(range.min);
		std::list<fixed_date_t> dates{};
		for (int32_t x : std::array{ y, y + 1, y + 2 }) {
			dates.push_back(m_coptic->to_fixed({ x, cmonth, day }));
		}
		filter(dates, range);
		return dates;
	}

	std::list<fixed_date_t> coptic_based_calendar_holiday_provider::christmas(int32_t year) const {
		return in_gregorian(4, 29, year);
	}
}