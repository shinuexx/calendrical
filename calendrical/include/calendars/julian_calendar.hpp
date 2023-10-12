#pragma once

#include "datetypes/julian_date.hpp"
#include "range.hpp"
#include "moment.hpp"

namespace calendrical::calendars {

	using namespace calendrical::datetypes;

	class julian_calendar :
		public typed_calendar_if<julian_date, simple_calendar>,
		public year_aware_calendar_if,
		public month_aware_calendar_if<month>,
		public day_aware_calendar_if,
		public new_year_aware_calendar_if,
		public is_leap_year_aware_calendar_if {

		static constexpr fixed_date_t ms_epoch{ -1 };

		month get_month(const fixed_date_t& date, int32_t year) const;

	public:
		virtual int32_t next_year(int32_t year) const override { return year == -1 ? 1 : year + 1; }
		virtual int32_t previous_year(int32_t year) const override { return year == 1 ? -1 : year - 1; }

		virtual fixed_date_t get_new_year(int32_t year) const override;
		virtual int32_t get_year(const fixed_date_t& date) const override;
		virtual int32_t get_day(const fixed_date_t& date) const override;
		virtual bool is_leap_year(int32_t year) const override;

		virtual fixed_date_t get_epoch() const override { return ms_epoch; }
		virtual calendar_type get_calendar_type() const override { return calendar_type::SOLAR; }

		virtual julian_date to_date(const fixed_date_t& date) const override;
		virtual fixed_date_t to_fixed(const julian_date& date) const override;

		virtual month get_month(const fixed_date_t& date) const override;
	};
}