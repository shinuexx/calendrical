#pragma once

#include "fixed_date.hpp"
#include <compare>
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

        /** @brief Equality comparison. */
        constexpr bool operator==(const holiday_t& other) const = default;
        /** @brief Chronological ordering; holidays on the same date are ordered by name. */
        constexpr std::strong_ordering operator<=>(const holiday_t& other) const {
            if (const auto cmp = date <=> other.date; cmp != 0)
                return cmp;
            return name <=> other.name;
        }

        /** @brief Comparator comparing holidays strictly by date. */
        struct date_only_less_t {
            using is_transparent = void;

            constexpr bool operator()(const holiday_t& lhs, const holiday_t& rhs) const noexcept {
                return lhs.date < rhs.date;
            }
            constexpr bool operator()(const holiday_t& lhs, const fixed_date_t& rhs) const noexcept {
                return lhs.date < rhs;
            }
            constexpr bool operator()(const fixed_date_t& lhs, const holiday_t& rhs) const noexcept {
                return lhs < rhs.date;
            }
        };

        static constexpr date_only_less_t date_only_less{};
    };

    using date_only_less_t = holiday_t::date_only_less_t;
    inline constexpr date_only_less_t date_only_less = holiday_t::date_only_less;

    /**
     * @brief Runtime-polymorphic base interface for calendar systems.
     */
    struct calendar_if {
        constexpr calendar_if() = default;
        /** @brief Returns the calendar epoch as a fixed date. */
        [[nodiscard]] virtual fixed_date_t epoch() const = 0;
        constexpr virtual ~calendar_if() = default;
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
        /** @brief Validates if the given date is valid for this calendar. */
        [[nodiscard]] virtual bool is_valid_date(const T& date) const {
            return to_date(to_fixed(date)) == date;
        }
        constexpr ~typed_calendar_if() override = default;
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

}
