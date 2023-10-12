#include "calendars/coptic_based_calendar.hpp"

namespace calendrical::calendars {
	const coptic_based_calendar coptic_calendar{ 103605 };
	const coptic_based_calendar ethiopic_calendar{ 2796 };

	int32_t coptic_based_calendar::get_month(const fixed_date_t& date, int32_t year) const {
		return static_cast<int32_t>(floor_div(date - get_new_year(year), 30) + 1);
	}
	fixed_date_t coptic_based_calendar::get_new_year(int32_t year) const {
		return to_fixed({ year, 1, 1 });
	}
	int32_t coptic_based_calendar::get_year(const fixed_date_t& date) const {
		return static_cast<int32_t>(floor_div(4 * (date - m_epoch) + 1463, 1461));
	}
	simple_date coptic_based_calendar::to_date(const fixed_date_t& date) const {
		auto year = get_year(date);
		auto month = get_month(date);
		auto day = date + 1 - to_fixed({ year, month, 1 });
		return { year, month, static_cast<int32_t>(day) };
	}
	fixed_date_t coptic_based_calendar::to_fixed(const simple_date& date) const {
		auto [year, month, day] = date.tie();
		return m_epoch - 1
			+ 365 * (year - 1LL)
			+ floor_div(year, 4LL)
			+ 30 * (month - 1LL)
			+ day;
	}
	int32_t coptic_based_calendar::get_month(const fixed_date_t& date) const {
		return get_month(date, get_year(date));
	}
	int32_t coptic_based_calendar::get_day(const fixed_date_t& date) const {
		return to_date(date).day;
	}
	bool coptic_based_calendar::is_leap_year(int32_t year) const {
		return mod(year, 4LL) == 3;
	}
	std::shared_ptr<const coptic_based_calendar> coptic_based_calendar::get_shared() const {
		return std::dynamic_pointer_cast<const coptic_based_calendar>(shared_from_this());
	}
}