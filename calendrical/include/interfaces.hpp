#pragma once

#include <compare>
#include <memory>
#include <string>
#include <list>
#include <type_traits>
#include "calendar_type.hpp"
#include "fixed_date.hpp"

namespace calendrical {
    struct holiday_t {
        std::string name;
        fixed_date_t date;

        constexpr bool operator==(const holiday_t& other) const { return (date == other.date) && (name == other.name); }
        constexpr auto operator<=>(const holiday_t& other) const { return date <=> other.date; }
    };

    struct date_if {
        virtual std::string to_string() const = 0;
        virtual ~date_if() = default;
    };

    struct calendar_if : public std::enable_shared_from_this<calendar_if> {
        constexpr calendar_if() : std::enable_shared_from_this<calendar_if>{} {}
        virtual fixed_date_t get_epoch() const = 0;
        virtual calendar_type get_calendar_type() const = 0;
        virtual ~calendar_if() = default;
    };

    template<typename T, typename SelfType, std::enable_if_t<std::is_base_of_v<date_if, T>, int> = 0>
    struct typed_calendar_if : public calendar_if {
        constexpr typed_calendar_if() : calendar_if() {}
        virtual T to_date(const fixed_date_t& date) const = 0;
        virtual fixed_date_t to_fixed(const T& date) const = 0;
        virtual std::shared_ptr<const SelfType> get_shared() const { return std::dynamic_pointer_cast<const SelfType>(shared_from_this()); }
        virtual ~typed_calendar_if() = default;
    };

    struct holiday_provider_if {
        virtual std::shared_ptr<const calendar_if> get_calendar() const = 0;
        virtual std::list<holiday_t> get_holiday_in_year(int32_t gyear) const = 0;

        virtual ~holiday_provider_if() = default;
    };

    struct has_year_if
    {
        int32_t year;

        constexpr has_year_if(const int32_t& year) : year(year) {}
        virtual ~has_year_if() = default;
    };

    template <typename MonthType>
    struct has_month_if
    {
        MonthType month;

        constexpr has_month_if(const MonthType& month) : month(month) {}
        virtual ~has_month_if() = default;
    };

    struct has_day_if
    {
        int32_t day;

        constexpr has_day_if(const int32_t& day) : day(day) {}
        virtual ~has_day_if() = default;
    };

    struct has_week_if
    {
        int32_t week;

        constexpr has_week_if(const int32_t& week) : week(week) {}
        virtual ~has_week_if() = default;
    };

    template <typename MonthType>
    struct has_leap_month_if : public has_month_if<MonthType>
    {
        bool leap_month;

        constexpr has_leap_month_if(MonthType month, bool leap_month) : has_month_if<MonthType>{ month }, leap_month(leap_month) {}
        virtual ~has_leap_month_if() = default;
    };

    struct has_leap_day_if : public has_day_if
    {
        bool leap_day;

        constexpr has_leap_day_if(int32_t day, bool leap_day) : has_day_if{ day }, leap_day(leap_day) {}
        virtual ~has_leap_day_if() = default;
    };

    struct has_cycle_if
    {
        int32_t cycle;

        constexpr has_cycle_if(const int32_t& cycle) : cycle(cycle) {}
        virtual ~has_cycle_if() = default;
    };

    struct has_major_if
    {
        int32_t major;

        constexpr has_major_if(const int32_t& major) : major(major) {}
        virtual ~has_major_if() = default;
    };

    template <typename MonthType>
    struct month_aware_calendar_if
    {
        virtual MonthType get_month(const fixed_date_t& date) const = 0;
        virtual ~month_aware_calendar_if() = default;
    };

    template<typename MonthType>
    struct leap_month_aware_calendar_if : public month_aware_calendar_if<MonthType> {
        virtual bool is_leap_month(const fixed_date_t& date) const = 0;

        virtual ~leap_month_aware_calendar_if() = default;
    };

    struct week_aware_calendar_if
    {
        virtual int32_t get_week(const fixed_date_t& date) const = 0;
        virtual ~week_aware_calendar_if() = default;
    };

    struct day_aware_calendar_if
    {
        virtual int32_t get_day(const fixed_date_t& date) const = 0;
        virtual ~day_aware_calendar_if() = default;
    };

    struct leap_day_aware_calendar_if : public day_aware_calendar_if
    {
        virtual bool is_leap_day(const fixed_date_t& date) const = 0;
        virtual ~leap_day_aware_calendar_if() = default;
    };

    struct year_aware_calendar_if
    {
        virtual int32_t get_year(const fixed_date_t& date) const = 0;
        virtual int32_t next_year(int32_t year) const { return year + 1; }
        virtual int32_t previous_year(int32_t year) const { return year - 1; }
        virtual ~year_aware_calendar_if() = default;
    };

    struct new_year_aware_calendar_if
    {
        virtual fixed_date_t get_new_year(int32_t year) const = 0;
        virtual ~new_year_aware_calendar_if() = default;
    };

    struct is_leap_year_aware_calendar_if
    {
        virtual bool is_leap_year(int32_t year) const = 0;
        virtual ~is_leap_year_aware_calendar_if() = default;
    };

    struct major_aware_calendar_if
    {
        virtual int32_t get_major(const fixed_date_t& date) const = 0;
        virtual ~major_aware_calendar_if() = default;
    };

    struct cycle_aware_calendar_if
    {
        virtual int32_t get_cycle(const fixed_date_t& date) const = 0;
        virtual ~cycle_aware_calendar_if() = default;
    };

    template<typename MonthType>
    struct in_gregorian_aware_provider_if {
        virtual std::list<fixed_date_t> in_gregorian(MonthType cmonth, int32_t day, int32_t gyear) const = 0;
        virtual ~in_gregorian_aware_provider_if() = default;
    };
}

static std::ostream& operator<<(std::ostream& out, const calendrical::date_if& date) {
    return out << date.to_string();
}