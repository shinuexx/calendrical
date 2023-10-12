#pragma once

#include "calendars/julian_calendar.hpp"
#include "angle.hpp"

namespace calendrical::holiday_providers {

	using namespace calendrical::calendars;

	class julian_calendar_holiday_provider :
		public holiday_provider_if,
		public in_gregorian_aware_provider_if<month> {

		std::shared_ptr<julian_calendar> m_julian;
		std::shared_ptr<gregrian_calendar> m_gregorian;

		fixed_date_t season_start;

	public:
		julian_calendar_holiday_provider() : m_julian{ std::make_shared<julian_calendar>() }, m_gregorian{ gregorian_calendar::standard.get_shared() } {
			season_start = m_julian->to_fixed({ -1, month::march, 23 });
		}

		virtual std::shared_ptr<const calendar_if> get_calendar() const override;
		virtual std::list<holiday_t> get_holiday_in_year(int32_t gyear) const override;

		virtual std::list<fixed_date_t> in_gregorian(month cmonth, int32_t day, int32_t gyear) const override;

		std::list<fixed_date_t> orthodox_christmas(int32_t year) const;
		fixed_date_t orthodox_easter(int32_t year) const;
		fixed_date_t orthodox_pentecost(int32_t year) const;

		std::list<moment_t> season_in_gregorian(const angle& season, int32_t year) const;
	};
}