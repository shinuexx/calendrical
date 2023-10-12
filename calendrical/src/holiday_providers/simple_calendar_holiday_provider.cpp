#include "holiday_providers/simple_calendar_holiday_provider.hpp"

namespace calendrical::holiday_providers{
	std::shared_ptr<const calendar_if> simple_calendar_holiday_provider::get_calendar() const {
		return m_simple;
	}
	std::list<holiday_t> simple_calendar_holiday_provider::get_holiday_in_year(int32_t gyear) const {
		return {};
	}

	std::list<fixed_date_t> simple_calendar_holiday_provider::in_gregorian(int32_t cmonth, int32_t day, int32_t gyear) const {
		auto range = m_gregorian->get_year_range(gyear);
		auto y = m_simple->get_year(range.min);
		std::list<fixed_date_t> dates{};
		for (int32_t x : { y - 1, y, y + 1, y + 2 }) {
			dates.push_back(m_simple->to_fixed({ x, cmonth, day }));
		}
		filter(dates, range);
		return dates;
	}
}