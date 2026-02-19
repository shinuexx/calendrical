#pragma once

#include <map>


#include "converter.hpp"

namespace calendrical::cli {
    class converter_provider final {
        std::map<std::string_view, std::unique_ptr<converter>> m_converters;
    public:
        converter_provider() = default;

        const converter* operator[](std::string_view name) const;
        converter_provider& add(std::unique_ptr<converter> conv);

        template<std::derived_from<calendar_if> CalendarT,typename parser_t = std::function<typename CalendarT::date_type(const fixed_date_t&)>>
        converter_provider& add(std::string name, std::string format, parser_t parser) {
            add(std::make_unique<calendar_converter<CalendarT>>(
                std::move(name),
                std::move(format),
                parser
                )
            );
            return *this;
        }

        template<std::derived_from<calendar_if> CalendarT,typename parser_t = std::function<typename CalendarT::date_type(const fixed_date_t&)>>
        converter_provider& add(std::string name, std::string format, parser_t parser, CalendarT cal) {
            add(std::make_unique<calendar_converter<CalendarT>>(
                std::move(name),
                std::move(format),
                parser,
                std::move(cal)
                )
            );
            return *this;
        }

        template<typename T> requires std::integral<T> || std::floating_point<T>
        converter_provider& add(std::string name, std::string format, std::function<T(const std::string&)> parser, std::function<fixed_date_t(T)> to_fixed, std::function<T(fixed_date_t)> to_rd) {
            add(
                std::make_unique<primitive_converter<T>>(
                    std::move(name),
                    std::move(format),
                    parser,
                    to_fixed,
                    to_rd
                )
            );
            return *this;
        }

        [[nodiscard]] const auto& converters() const { return m_converters; }
    };
}
