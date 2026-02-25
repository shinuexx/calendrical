#pragma once

#include "fixed_date.hpp"
#include <string>
#include <vector>

namespace calendrical{
    /**
     * @brief Named holiday occurrence in fixed-date coordinates.
     */
    struct holiday_t{
        /** @brief Human-readable holiday name. */
        std::string name;
        /** @brief Holiday date in internal fixed-date representation. */
        fixed_date_t date;

        /** @brief Equality comparison by date. */
        constexpr bool operator==(const holiday_t& other) const { return date == other.date; }
        /** @brief Ordering comparison by date. */
        constexpr auto operator<=>(const holiday_t& other) const { return date <=> other.date; }
    };

    /**
     * @brief Runtime-polymorphic base interface for calendar systems.
     */
    struct calendar_if {
        constexpr calendar_if() = default;
        /** @brief Returns the calendar epoch as a fixed date. */
        [[nodiscard]] virtual fixed_date_t epoch() const = 0;
        virtual ~calendar_if() = default;
    };

    template<typename T>
    /**
     * @brief Typed calendar interface exposing conversions to a concrete date type.
     */
    struct typed_calendar_if : calendar_if {
        /** @brief Concrete date type handled by the calendar implementation. */
        using date_type = T;

        constexpr typed_calendar_if() = default;
        /** @brief Converts a fixed date to the calendar-specific date type. */
        [[nodiscard]] virtual T to_date(const fixed_date_t& date) const = 0;
        /** @brief Converts a calendar-specific date to fixed-date representation. */
        [[nodiscard]] virtual fixed_date_t to_fixed(const T& date) const = 0;
        ~typed_calendar_if() override = default;
    };

    /**
     * @brief Interface for holiday providers backed by a specific calendar.
     */
    struct holiday_provider_if {
        /** @brief Returns the calendar used by this provider. */
        [[nodiscard]] virtual const calendar_if* calendar() const = 0;
        /** @brief Returns holidays that fall in Gregorian year @p gyear. */
        [[nodiscard]] virtual std::vector<holiday_t> in_year(int32_t gyear) const = 0;

        virtual ~holiday_provider_if() = default;
    };

    template<typename T,typename = std::enable_if_t<std::is_enum_v<T>>>
    /**
     * @brief Default string parser hook for enum types.
     *
     * Specific enums provide explicit specializations.
     */
    struct from_parser {
        /** @brief Fallback parser that returns a default-initialized enum value. */
        static T from_string(const std::string& ) { return T{}; }
    };
}
