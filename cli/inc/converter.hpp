#pragma once

#include <string>
#include <concepts>
#include <stdexcept>
#include <utility>
#include <concepts.hpp>

#include "types.hpp"

namespace calendrical::cli {
    /**
     * @brief Named, text-based conversion between a date notation and @ref fixed_date_t.
     *
     * The CLI `convert` command chains two converters: `to_rd` on the source, `from_rd` on the target.
     */
    class converter {
    protected:
        /** @brief Name used on the command line, such as `gregorian` or `jd`. */
        std::string m_name;
        /** @brief Human-readable input format shown in help, such as `YYYY-MONTH-DD`. */
        std::string m_format;

        /** @brief Initializes the converter name and input format description. */
        explicit converter(std::string name, std::string format) :m_name{std::move(name)}, m_format {std::move(format)} {}
    public:
        /** @brief Name used on the command line. */
        [[nodiscard]] std::string_view name() const { return m_name; }
        /** @brief Human-readable input format shown in help. */
        [[nodiscard]] std::string_view format() const { return m_format; }

        /** @brief Formats fixed date @p rd in this converter's notation. */
        [[nodiscard]] virtual std::string from_rd(const fixed_date_t& rd) const = 0;
        /**
         * @brief Parses @p date in this converter's notation into a fixed date.
         * @throws std::exception subclasses if @p date cannot be parsed or is not a valid date.
         */
        [[nodiscard]] virtual fixed_date_t to_rd(const std::string& date) const = 0;
        /** @brief Virtual destructor for polymorphic use. */
        virtual ~converter() = default;
    };

    /**
     * @brief Converter for single-number notations (R.D., JD, MJD, Unix time, ...).
     * @tparam T Numeric representation of the notation.
     */
    template<typename T> requires std::integral<T> || std::floating_point<T>
    class primitive_converter: public converter {
        /** @brief Parses the textual input into a number. */
        using date_parser_t = std::function<T(const std::string&)>;
        /** @brief Maps the number to a fixed date. */
        using from_function_t = std::function<fixed_date_t(T)>;
        /** @brief Maps a fixed date to the number. */
        using to_function_t = std::function<T(fixed_date_t)>;
        /** @brief Parses the textual input into a number. */
        date_parser_t m_date_parser;
        /** @brief Maps the parsed number to a fixed date. */
        from_function_t m_to_fixed;
        /** @brief Maps a fixed date to the number that is printed. */
        to_function_t m_to_date;

    public:
        /** @brief Creates a converter from a parser and the two mapping functions. */
        explicit primitive_converter(std::string name, std::string format, date_parser_t date_parser, from_function_t to_fixed, to_function_t to_date)
            : converter(std::move(name), std::move(format)),
                m_date_parser(std::move(date_parser)),
                m_to_fixed{std::move(to_fixed)},
                m_to_date{std::move(to_date)} {}

        /** @brief Prints the number corresponding to @p rd. */
        [[nodiscard]] std::string from_rd(const fixed_date_t& rd) const override {
            const auto x = m_to_date(rd);
            return std::to_string(x);
        };
        /** @brief Parses a number and maps it to a fixed date. */
        [[nodiscard]] fixed_date_t to_rd(const std::string& date) const override {
            const auto parsed_date = m_date_parser(date);
            return m_to_fixed(parsed_date);
        }
    };

    /**
     * @brief Converter for a calendar's own date notation.
     *
     * Input is parsed by a date parser and validated with the calendar's `is_valid_date`
     * before conversion; output uses the date type's `to_string`.
     * @tparam CalendarT Calendar type providing `date_type`, `to_date`, `to_fixed` and `is_valid_date`.
     */
    template<std::derived_from<calendar_if> CalendarT>
    class calendar_converter : public converter{
        /** @brief Parses the textual input into the calendar's date type. */
        using date_parser_t = std::function<typename CalendarT::date_type(const std::string&)>;
        /** @brief Calendar instance used for validation and conversion. */
        CalendarT m_calendar{};
        /** @brief Parses the textual input into the calendar's date type. */
        date_parser_t m_date_parser;
    public:
        /** @brief Creates a converter over a default-constructed calendar. */
        explicit calendar_converter(std::string name, std::string format, date_parser_t date_parser)
            : converter(std::move(name), std::move(format)), m_date_parser{std::move(date_parser)} {}

        /** @brief Creates a converter over a copy of @p cal, for example a calendar with a custom epoch. */
        explicit calendar_converter(std::string name, std::string format, date_parser_t date_parser, CalendarT cal)
            : converter(std::move(name), std::move(format)), m_calendar{std::move(cal)}, m_date_parser{std::move(date_parser)} {}

        /** @brief Formats @p rd as a date of this calendar. */
        [[nodiscard]] std::string from_rd(const fixed_date_t& rd) const override {
            return m_calendar.to_date(rd).to_string();
        }
        /**
         * @brief Parses and validates @p date, then converts it to a fixed date.
         * @throws std::invalid_argument if the parsed date does not exist in this calendar.
         */
        [[nodiscard]] fixed_date_t to_rd(const std::string& date) const override {
            const auto parsed_date = m_date_parser(date);
            if (!m_calendar.is_valid_date(parsed_date)) {
                throw std::invalid_argument("Invalid date: " + date);
            }
            return m_calendar.to_fixed(parsed_date);
        }

        ~calendar_converter() override = default;
    };
}
