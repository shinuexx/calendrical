#pragma once

#include "fixed_date.hpp"
#include <string>
#include <vector>

namespace calendrical{
    struct holiday_t{
        std::string name;
        fixed_date_t date;

        constexpr bool operator==(const holiday_t& other) const { return date == other.date; }
        constexpr auto operator<=>(const holiday_t& other) const { return date <=> other.date; }
    };

    struct calendar_if {
        constexpr calendar_if() = default;
        [[nodiscard]] virtual fixed_date_t epoch() const = 0;
        virtual ~calendar_if() = default;
    };

    template<typename T>
    struct typed_calendar_if : calendar_if {
        using date_type = T;

        constexpr typed_calendar_if() = default;
        [[nodiscard]] virtual T to_date(const fixed_date_t& date) const = 0;
        [[nodiscard]] virtual fixed_date_t to_fixed(const T& date) const = 0;
        ~typed_calendar_if() override = default;
    };

    struct holiday_provider_if {
        [[nodiscard]] virtual const calendar_if* calendar() const = 0;
        [[nodiscard]] virtual std::vector<holiday_t> in_year(int32_t gyear) const = 0;

        virtual ~holiday_provider_if() = default;
    };

    template<typename T,typename = std::enable_if_t<std::is_enum_v<T>>>
    struct from_parser {
        static T from_string(const std::string& ) { return T{}; }
    };
}
