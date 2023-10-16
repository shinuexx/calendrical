#pragma once

#include "datetypes/roman_date.hpp"
#include "julian_calendar.hpp"

namespace calendrical::calendars {
	
	using namespace calendrical::datetypes;

	class roman_calendar :
		public typed_calendar_if<roman_date, roman_calendar>,
		public year_aware_calendar_if,
		public month_aware_calendar_if<month>,
		public is_leap_year_aware_calendar_if {

		julian_calendar m_julian;

	public:
		roman_calendar();

		virtual int32_t next_year(int32_t year) const override { return year == -1 ? 1 : year + 1; }
		virtual int32_t previous_year(int32_t year) const override { return year == 1 ? -1 : year - 1; }

		virtual int32_t get_year(const fixed_date_t& date) const override;
		virtual bool is_leap_year(int32_t year) const override;

		virtual fixed_date_t get_epoch() const override { return m_julian.get_epoch(); }
		virtual calendar_type get_calendar_type() const override { return calendar_type::SOLAR; }

		roman_event get_event(const fixed_date_t& date) const;
		int32_t get_count(const fixed_date_t& date) const;
		bool get_leap(const fixed_date_t& date) const;

		virtual roman_date to_date(const fixed_date_t& date) const override;
		virtual fixed_date_t to_fixed(const roman_date& date) const override;

		virtual month get_month(const fixed_date_t& date) const override;

	private:
		int32_t ides_of_month(month m) const;
		int32_t nones_of_month(month m) const;
	};
}