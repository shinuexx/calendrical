#pragma once

#include "datetypes/gregorian_date.hpp"
#include "range.hpp"
#include "moment.hpp"
#include "angle.hpp"

namespace calendrical::calendars {
	using namespace calendrical::datetypes;
	class gregorian_calendar :
		public typed_calendar_if<gregorian_date, gregorian_calendar>,
		public year_aware_calendar_if,
		public month_aware_calendar_if<month>,
		public day_aware_calendar_if,
		public new_year_aware_calendar_if,
		public is_leap_year_aware_calendar_if {

		fixed_date_t m_epoch;

		month get_month(const fixed_date_t& date, int32_t year) const;

	public:
		constexpr gregorian_calendar() : m_epoch{ 1 } {}
		constexpr gregorian_calendar(const fixed_date_t& epoch) : m_epoch{ epoch } {}

		virtual ~gregorian_calendar() = default;

		virtual fixed_date_t get_new_year(int32_t year) const override;
		virtual int32_t get_year(const fixed_date_t& date) const override;

		virtual fixed_date_t get_epoch() const override { return m_epoch; }
		virtual calendar_type get_calendar_type() const override { return calendar_type::SOLAR; }

		virtual gregorian_date to_date(const fixed_date_t& date) const override;
		virtual fixed_date_t to_fixed(const gregorian_date& date) const override;

		virtual month get_month(const fixed_date_t& date) const override;

		virtual int32_t get_day(const fixed_date_t& date) const override;

		virtual bool is_leap_year(int32_t year) const override;

		fixed_date_t get_year_end(int32_t year) const;
		range<fixed_date_t> get_year_range(int32_t year) const;

		int32_t get_remaining_days(const gregorian_date& date) const;

		fixed_date_t nth_weekday(int64_t n, weekday k, const gregorian_date& date) const;

		fixed_date_t first_weekday(weekday k, const gregorian_date& date) const;
		fixed_date_t last_weekday(weekday k, const gregorian_date& date) const;

		std::list<moment_t> cycle_in_gregorian(const angle_t& season, int year, double L, const moment_t& start) const;

		static const gregorian_calendar standard;
	};
}