#pragma once

#include <map>


#include "converter.hpp"

namespace calendrical::cli {
    /** @brief Registry of converters, looked up by name. */
    class converter_provider final {
        /** @brief Converters keyed by name (keys view each converter's own name). */
        std::map<std::string_view, std::unique_ptr<converter>> m_converters;
    public:
        /** @brief Creates an empty registry. */
        converter_provider() = default;

        /** @brief Returns the converter registered as @p name, or `nullptr`. */
        const converter* operator[](std::string_view name) const;
        /** @brief Registers @p conv; a name that is already taken is reported on stderr and ignored. */
        converter_provider& add(std::unique_ptr<converter> conv);

        /** @brief Registers a @ref calendar_converter over a default-constructed @p CalendarT. */
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

        /** @brief Registers a @ref calendar_converter over a copy of @p cal. */
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

        /** @brief Registers a @ref primitive_converter for a single-number notation. */
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

        /** @brief All registered converters, ordered by name. */
        [[nodiscard]] const auto& converters() const { return m_converters; }
    };
}
