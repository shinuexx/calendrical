#pragma once

#include <string>
#include <concepts>
#include <utility>
#include <concepts.hpp>

#include "types.hpp"

namespace calendrical::cli {
    class converter {
    protected:
        std::string m_name;
        std::string m_format;

        explicit converter(std::string name, std::string format) :m_name{std::move(name)}, m_format {std::move(format)} {}
    public:
        [[nodiscard]] std::string_view name() const { return m_name; }
        [[nodiscard]] std::string_view format() const { return m_format; }

        [[nodiscard]] virtual std::string from_rd(const fixed_date_t& rd) const = 0;
        [[nodiscard]] virtual fixed_date_t to_rd(const std::string& date) const = 0;
        virtual ~converter() = default;
    };

    template<typename T> requires std::integral<T> || std::floating_point<T>
    class primitive_converter: public converter {
        using date_parser_t = std::function<T(const std::string&)>;
        using from_function_t = std::function<fixed_date_t(T)>;
        using to_function_t = std::function<T(fixed_date_t)>;
        date_parser_t m_date_parser;
        from_function_t m_to_fixed;
        to_function_t m_to_date;

    public:
        explicit primitive_converter(std::string name, std::string format, date_parser_t date_parser, from_function_t to_fixed, to_function_t to_date)
            : converter(std::move(name), std::move(format)),
                m_date_parser(std::move(date_parser)),
                m_to_fixed{std::move(to_fixed)},
                m_to_date{std::move(to_date)} {}

        [[nodiscard]] std::string from_rd(const fixed_date_t& rd) const override {
            const auto x = m_to_date(rd);
            return std::to_string(x);
        };
        [[nodiscard]] fixed_date_t to_rd(const std::string& date) const override {
            const auto parsed_date = m_date_parser(date);
            return m_to_fixed(parsed_date);
        }
    };

    template<std::derived_from<calendar_if> CalendarT>
    class calendar_converter : public converter{
        using date_parser_t = std::function<typename CalendarT::date_type(const std::string&)>;
        CalendarT m_calendar{};
        date_parser_t m_date_parser;
    public:
        explicit calendar_converter(std::string name, std::string format, date_parser_t date_parser)
            : converter(std::move(name), std::move(format)), m_date_parser{std::move(date_parser)} {}

        explicit calendar_converter(std::string name, std::string format, date_parser_t date_parser, CalendarT cal)
            : converter(std::move(name), std::move(format)), m_calendar{std::move(cal)}, m_date_parser{std::move(date_parser)} {}

        [[nodiscard]] std::string from_rd(const fixed_date_t& rd) const override {
            return m_calendar.to_date(rd).to_string();
        }
        [[nodiscard]] fixed_date_t to_rd(const std::string& date) const override {
            const auto parsed_date = m_date_parser(date);
            return m_calendar.to_fixed(parsed_date);
        }

        ~calendar_converter() override = default;
    };
}
