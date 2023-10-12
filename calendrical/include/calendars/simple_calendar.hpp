#pragma once

#include "../datetypes/simple_date.hpp"

namespace calendrical::calendars {
	using namespace calendrical::datetypes;
	class simple_calendar :
		public typed_calendar_if<simple_date, simple_calendar>,
		public year_aware_calendar_if,
		public month_aware_calendar_if<int32_t>,
		public day_aware_calendar_if,
		public new_year_aware_calendar_if {

		fixed_date_t m_epoch;

	public:
		constexpr simple_calendar(const fixed_date_t& epoch) : m_epoch{ epoch } {}

		virtual ~simple_calendar() = default;

		virtual fixed_date_t get_new_year(int32_t year) const override;
		virtual int32_t get_year(const fixed_date_t& date) const override;

		virtual fixed_date_t get_epoch() const override { return m_epoch; }
		virtual calendar_type get_calendar_type() const override { return calendar_type::SOLAR; }

		virtual simple_date to_date(const fixed_date_t& date) const override;
		virtual fixed_date_t to_fixed(const simple_date& date) const override;

		virtual int32_t get_month(const fixed_date_t& date) const override;
		virtual int32_t get_day(const fixed_date_t& date) const override;

		std::shared_ptr<const simple_calendar> get_shared() const;
	};

	extern const simple_calendar egyptian_calendar;
	extern const simple_calendar armenian_calendar;
}