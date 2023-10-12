#pragma once

#include "gregorian_calendar.hpp"
#include "datetypes/iso_date.hpp"

namespace calendrical::calendars {

	using namespace calendrical::calendars;

	class iso_calendar :
		public typed_calendar_if<iso_date, iso_calendar>,
		public year_aware_calendar_if,
		public week_aware_calendar_if,
		public day_aware_calendar_if,
		public new_year_aware_calendar_if,
		public is_leap_year_aware_calendar_if {

		std::shared_ptr<gregorian_calendar> m_gregorian;

		int32_t get_week(const fixed_date_t& date, int32_t year) const;

	public:
		iso_calendar();

		virtua ~iso_calendar() = default;

		virtual fixed_date_t get_epoch() const override;
		virtual calendar_type get_calendar_type() const { return calendar_type::SOLAR; }

		virtual fixed_date_t get_new_year(int32_t year) const override;

		virtual iso_date to_date(const fixed_date_t& date) const override;
		virtual fixed_date_t to_fixed(const iso_date& date) const override;

		virtual int32_t get_year(const fixed_date_t& date) const override;
		virtual bool is_leap_year(int32_t yaer) const override;

		virtual int32_t get_day(const fixed_date_t& date) const override;

		virtual int32_t get_week(const fixed_date_t& date) const override;
	};
}