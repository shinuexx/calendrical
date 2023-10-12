#pragma once

#include "calendars/gregorian_calendar.hpp"

namespace calendrical::holiday_providers {

	using namespace calendrical::calendars;

	class gregorian_calendar_holiday_provider :
		public holiday_provider_if {

		std::shared_ptr<gregorian_calendar> m_gregorian;

	public:
		gregorian_calendar_holiday_provider(std::shared_ptr<gregorian_calendar> gregorian) : m_gregorian{ gregorian } {}
		gregorian_calendar_holiday_provider() : m_gregorian{ gregorian_calendar::standard.get_shared() } {}

		virtual std::shared_ptr<const calendar_if> get_calendar() const override;
		virtual std::list<holiday_t> get_holiday_in_year(int32_t gyear) const override;

		fixed_date_t christmas(int32_t year) const;
		fixed_date_t advent(int32_t year) const;
		fixed_date_t epiphany(int32_t year) const;
		fixed_date_t easter(int32_t year) const;

		fixed_date_t pentecost(int32_t year) const;
		std::list<fixed_date_t> unlucky_fridays_in_range(const range<fixed_date_t>& range) const;

		std::list<fixed_date_t> unlucky_fridays(int32_t year) const;

		fixed_date_t astronomical_easter(int32_t year) const;
	};
}