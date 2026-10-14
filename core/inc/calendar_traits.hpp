#pragma once

#include <array>
#include <charconv>
#include <cctype>
#include <map>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "datetypes/bahai_date.hpp"
#include "datetypes/chinese_date.hpp"
#include "datetypes/gregorian_date.hpp"
#include "datetypes/hebrew_date.hpp"
#include "datetypes/icelandic_date.hpp"
#include "datetypes/iso_date.hpp"
#include "datetypes/julian_date.hpp"
#include "datetypes/lunar_date.hpp"
#include "datetypes/roman_date.hpp"
#include "datetypes/sexagesimal_name.hpp"
#include "datetypes/simple_date.hpp"
#include "datetypes/simple_lunar_date.hpp"
#include "chinese_marriage_augury.hpp"
#include "hebrew_month.hpp"
#include "lunar_phase.hpp"
#include "month.hpp"
#include "interval.hpp"
#include "roman_event.hpp"
#include "season.hpp"
#include "weekday.hpp"
#include "calendars/gregorian_calendar.hpp"
#include "calendars/julian_calendar.hpp"
#include "calendars/iso_calendar.hpp"
#include "calendars/hebrew_calendar.hpp"
#include "calendars/icelandic_calendar.hpp"
#include "calendars/roman_calendar.hpp"
#include "calendars/bahai_calendar.hpp"
#include "calendars/chinese_calendar.hpp"

namespace calendrical::calendar_traits {
    template<typename EnumT>
    /**
     * @brief Converts symbolic values (primarily enums) to/from textual representations.
     *
     * Explicit specializations provide parsing logic for supported enum and enum-like types.
     */
    struct enum_string_converter {
        // ReSharper disable once CppFunctionIsNotImplemented
        /** @brief Parses an enum value from text. Implemented by specializations. */
        static EnumT from_string(std::string_view);
    };

    namespace detail {
        /** @brief Trims ASCII whitespace from both ends of a string view. */
        inline std::string_view trim(std::string_view value) {
            std::size_t begin = 0;
            while (begin < value.size() && std::isspace(static_cast<unsigned char>(value[begin])) != 0) {
                ++begin;
            }

            std::size_t end = value.size();
            while (end > begin && std::isspace(static_cast<unsigned char>(value[end - 1])) != 0) {
                --end;
            }

            return value.substr(begin, end - begin);
        }

        /** @brief Returns a lowercased copy of the input (ASCII semantics). */
        inline std::string lower_copy(std::string_view value) {
            std::string out{value};
            for (auto& c : out) {
                c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            }
            return out;
        }

        /** @brief Parses a signed 32-bit integer or throws with field context. */
        inline int32_t parse_int32(std::string_view value, std::string_view field_name) {
            int32_t parsed{};
            if (const auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.size(), parsed);
                ec != std::errc{} || ptr != value.data() + value.size()) {
                throw std::runtime_error("Invalid int value for field '" + std::string(field_name) + "': " + std::string(value));
            }
            return parsed;
        }

        /** @brief Parses a boolean (`true/false/1/0`) or throws with field context. */
        inline bool parse_bool(std::string_view value, std::string_view field_name) {
            if (value == "true" || value == "1") return true;
            if (value == "false" || value == "0") return false;
            throw std::runtime_error("Invalid bool value for field '" + std::string(field_name) + "': " + std::string(value));
        }

        /**
         * @brief Parses `type{key:value,...}` record syntax into a field map.
         */
        inline std::map<std::string, std::string_view, std::less<>> parse_record(
            std::string_view input,
            std::string_view type_name) {
            const std::string prefix = std::string(type_name) + "{";
            if (!input.starts_with(prefix) || !input.ends_with("}")) {
                throw std::runtime_error("Invalid " + std::string(type_name) + " format: " + std::string(input));
            }

            const auto body = input.substr(prefix.size(), input.size() - prefix.size() - 1);
            std::map<std::string, std::string_view, std::less<>> fields;
            std::size_t start = 0;

            while (start < body.size()) {
                const auto comma = body.find(',', start);
                const auto token = trim(body.substr(start, comma == std::string_view::npos ? body.size() - start : comma - start));
                if (token.empty()) {
                    throw std::runtime_error("Invalid empty field in " + std::string(type_name));
                }

                const auto colon = token.find(':');
                if (colon == std::string_view::npos) {
                    throw std::runtime_error("Invalid field token in " + std::string(type_name) + ": " + std::string(token));
                }

                const auto key = trim(token.substr(0, colon));
                const auto value = trim(token.substr(colon + 1));
                fields.emplace(std::string(key), value);

                if (comma == std::string_view::npos) {
                    break;
                }
                start = comma + 1;
            }

            return fields;
        }

        /** @brief Splits a delimited string and trims each part. */
        inline std::vector<std::string_view> split_parts(std::string_view input, char delimiter) {
            std::vector<std::string_view> parts;
            std::size_t start = 0;
            while (true) {
                const auto pos = input.find(delimiter, start);
                if (pos == std::string_view::npos) {
                    parts.emplace_back(trim(input.substr(start)));
                    break;
                }
                parts.emplace_back(trim(input.substr(start, pos - start)));
                start = pos + 1;
            }
            return parts;
        }

        /** @brief Returns a required field value or throws if the key is missing. */
        inline std::string_view get_required_field(
            const std::map<std::string, std::string_view, std::less<>>& fields,
            std::string_view field_name,
            std::string_view type_name) {
            if (const auto it = fields.find(field_name); it != fields.end()) {
                return it->second;
            }
            throw std::runtime_error("Missing field '" + std::string(field_name) + "' in " + std::string(type_name));
        }

        /** @brief Parses weekday from name (`monday`) or numeric index (`0..6`). */
        inline weekday parse_weekday(std::string_view value) {
            const auto lower = lower_copy(value);
            constexpr std::array names{
                "sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday"
            };
            for (int i = 0; i < static_cast<int>(names.size()); ++i) {
                if (lower == names[i]) return weekday{i};
            }

            const auto numeric = parse_int32(value, "weekday");
            weekday wd{numeric};
            if (!wd.valid()) {
                throw std::runtime_error("Invalid weekday value: " + std::string(value));
            }
            return wd;
        }

    }

    template<>
    /** @brief Parser specialization for @ref calendrical::month. */
    struct enum_string_converter<month> {
        /** @brief Accepts numeric month or lowercase English month name. */
        static month from_string(std::string_view value) {
            if (!value.empty() && std::isdigit(static_cast<unsigned char>(value.front())) != 0) {
                if (const auto m = detail::parse_int32(value, "month");
                    m >= static_cast<int32_t>(month::january) && m <= static_cast<int32_t>(month::december)) {
                    return static_cast<month>(m);
                }
                throw std::runtime_error("Invalid month value: " + std::string(value));
            }

            const auto lower = detail::lower_copy(value);
            if (lower == "january") return month::january;
            if (lower == "february") return month::february;
            if (lower == "march") return month::march;
            if (lower == "april") return month::april;
            if (lower == "may") return month::may;
            if (lower == "june") return month::june;
            if (lower == "july") return month::july;
            if (lower == "august") return month::august;
            if (lower == "september") return month::september;
            if (lower == "october") return month::october;
            if (lower == "november") return month::november;
            if (lower == "december") return month::december;
            throw std::runtime_error("Invalid month value: " + std::string(value));
        }
    };

    template<>
    /** @brief Parser specialization for @ref calendrical::hebrew_month. */
    struct enum_string_converter<hebrew_month> {
        /** @brief Accepts numeric month or supported transliterated month name. */
        static hebrew_month from_string(std::string_view value) {
            if (!value.empty() && std::isdigit(static_cast<unsigned char>(value.front())) != 0) {
                if (const auto m = detail::parse_int32(value, "hebrew_month");
                    m >= static_cast<int32_t>(hebrew_month::nisan) && m <= static_cast<int32_t>(hebrew_month::adarii)) {
                    return static_cast<hebrew_month>(m);
                }
                throw std::runtime_error("Invalid hebrew month value: " + std::string(value));
            }

            const auto lower = detail::lower_copy(value);
            if (lower == "nisan") return hebrew_month::nisan;
            if (lower == "iyyar") return hebrew_month::iyyar;
            if (lower == "sivan") return hebrew_month::sivan;
            if (lower == "tammuz") return hebrew_month::tammuz;
            if (lower == "av") return hebrew_month::av;
            if (lower == "elul") return hebrew_month::elul;
            if (lower == "tishri") return hebrew_month::tishri;
            if (lower == "marheshvan") return hebrew_month::marheshvan;
            if (lower == "kislev") return hebrew_month::kislev;
            if (lower == "tevet") return hebrew_month::tevet;
            if (lower == "shevat") return hebrew_month::shevat;
            if (lower == "adar") return hebrew_month::adar;
            if (lower == "adarii" || lower == "adar2") return hebrew_month::adarii;
            throw std::runtime_error("Invalid hebrew month value: " + std::string(value));
        }
    };

    template<>
    /** @brief Parser specialization for @ref calendrical::season. */
    struct enum_string_converter<season> {
        /** @brief Accepts season name or canonical solar-longitude angle value. */
        static season from_string(std::string_view value) {
            const auto lower = detail::lower_copy(value);
            if (lower == "spring") return season::spring;
            if (lower == "summer") return season::summer;
            if (lower == "autumn") return season::autumn;
            if (lower == "winter") return season::winter;

            switch (detail::parse_int32(value, "season")) {
                case static_cast<int32_t>(season::spring): return season::spring;
                case static_cast<int32_t>(season::summer): return season::summer;
                case static_cast<int32_t>(season::autumn): return season::autumn;
                case static_cast<int32_t>(season::winter): return season::winter;
                default: throw std::runtime_error("Invalid season value: " + std::string(value));
            }
        }
    };

    template<>
    /** @brief Parser specialization for @ref calendrical::roman_event. */
    struct enum_string_converter<roman_event> {
        /** @brief Accepts Roman event name or numeric enum value. */
        static roman_event from_string(std::string_view value) {
            const auto lower = detail::lower_copy(value);
            if (lower == "kalends") return roman_event::kalends;
            if (lower == "nones") return roman_event::nones;
            if (lower == "ides") return roman_event::ides;

            switch (detail::parse_int32(value, "roman_event")) {
                case static_cast<int32_t>(roman_event::kalends): return roman_event::kalends;
                case static_cast<int32_t>(roman_event::nones): return roman_event::nones;
                case static_cast<int32_t>(roman_event::ides): return roman_event::ides;
                default: throw std::runtime_error("Invalid roman_event value: " + std::string(value));
            }
        }
    };

    template<>
    /** @brief Parser specialization for @ref calendrical::lunar_phase. */
    struct enum_string_converter<lunar_phase> {
        /** @brief Accepts canonical lowercase phase names or numeric phase angle values. */
        static lunar_phase from_string(std::string_view value) {
            const auto lower = detail::lower_copy(value);
            if (lower == "new_moon" || lower == "new") return lunar_phase::new_moon;
            if (lower == "first_quarter") return lunar_phase::first_quarter;
            if (lower == "full_moon" || lower == "full") return lunar_phase::full_moon;
            if (lower == "last_quarter") return lunar_phase::last_quarter;

            switch (detail::parse_int32(value, "lunar_phase")) {
                case static_cast<int32_t>(lunar_phase::new_moon): return lunar_phase::new_moon;
                case static_cast<int32_t>(lunar_phase::first_quarter): return lunar_phase::first_quarter;
                case static_cast<int32_t>(lunar_phase::full_moon): return lunar_phase::full_moon;
                case static_cast<int32_t>(lunar_phase::last_quarter): return lunar_phase::last_quarter;
                default: throw std::runtime_error("Invalid lunar_phase value: " + std::string(value));
            }
        }
    };

    template<>
    /** @brief Parser specialization for @ref calendrical::chinese_marriage_augury. */
    struct enum_string_converter<chinese_marriage_augury> {
        /** @brief Accepts lowercase augury names or numeric enum values. */
        static chinese_marriage_augury from_string(std::string_view value) {
            const auto lower = detail::lower_copy(value);
            if (lower == "double_bright") return chinese_marriage_augury::double_bright;
            if (lower == "bright") return chinese_marriage_augury::bright;
            if (lower == "blind") return chinese_marriage_augury::blind;
            if (lower == "widow") return chinese_marriage_augury::widow;

            switch (detail::parse_int32(value, "chinese_marriage_augury")) {
                case static_cast<int32_t>(chinese_marriage_augury::double_bright): return chinese_marriage_augury::double_bright;
                case static_cast<int32_t>(chinese_marriage_augury::bright): return chinese_marriage_augury::bright;
                case static_cast<int32_t>(chinese_marriage_augury::blind): return chinese_marriage_augury::blind;
                case static_cast<int32_t>(chinese_marriage_augury::widow): return chinese_marriage_augury::widow;
                default: throw std::runtime_error("Invalid chinese_marriage_augury value: " + std::string(value));
            }
        }
    };

    template<>
    /** @brief Parser specialization for @ref calendrical::interval_type. */
    struct enum_string_converter<interval_type> {
        /** @brief Accepts lowercase interval boundary policy names or numeric enum values. */
        static interval_type from_string(std::string_view value) {
            const auto lower = detail::lower_copy(value);
            if (lower == "opened") return interval_type::opened;
            if (lower == "closed") return interval_type::closed;
            if (lower == "left_closed") return interval_type::left_closed;
            if (lower == "right_closed") return interval_type::right_closed;

            switch (detail::parse_int32(value, "interval_type")) {
                case static_cast<int32_t>(interval_type::opened): return interval_type::opened;
                case static_cast<int32_t>(interval_type::closed): return interval_type::closed;
                case static_cast<int32_t>(interval_type::left_closed): return interval_type::left_closed;
                case static_cast<int32_t>(interval_type::right_closed): return interval_type::right_closed;
                default: throw std::runtime_error("Invalid interval_type value: " + std::string(value));
            }
        }
    };

    template<>
    /** @brief Parser specialization for @ref calendrical::weekday. */
    struct enum_string_converter<weekday> {
        /** @brief Accepts weekday name (`monday`) or numeric index (`0..6`). */
        static weekday from_string(std::string_view value) {
            const auto lower = detail::lower_copy(value);
            constexpr std::array names{
                "sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday"
            };
            for (int i = 0; i < static_cast<int>(names.size()); ++i) {
                if (lower == names[i]) return weekday{i};
            }

            const auto numeric = detail::parse_int32(value, "weekday");
            weekday wd{numeric};
            if (!wd.valid()) {
                throw std::runtime_error("Invalid weekday value: " + std::string(value));
            }
            return wd;
        }
    };

    namespace detail {
        /** @brief Convenience wrapper for month parsing. */
        inline month parse_month(std::string_view value) {
            return enum_string_converter<month>::from_string(value);
        }

        /** @brief Convenience wrapper for Hebrew month parsing. */
        inline hebrew_month parse_hebrew_month(std::string_view value) {
            return enum_string_converter<hebrew_month>::from_string(value);
        }

        /** @brief Convenience wrapper for season parsing. */
        inline season parse_season(std::string_view value) {
            return enum_string_converter<season>::from_string(value);
        }

        /** @brief Convenience wrapper for Roman event parsing. */
        inline roman_event parse_roman_event(std::string_view value) {
            return enum_string_converter<roman_event>::from_string(value);
        }

        /** @brief Convenience wrapper for lunar phase parsing. */
        inline lunar_phase parse_lunar_phase(std::string_view value) {
            return enum_string_converter<lunar_phase>::from_string(value);
        }

        /** @brief Convenience wrapper for Chinese marriage augury parsing. */
        inline chinese_marriage_augury parse_chinese_marriage_augury(std::string_view value) {
            return enum_string_converter<chinese_marriage_augury>::from_string(value);
        }

        /** @brief Convenience wrapper for interval boundary policy parsing. */
        inline interval_type parse_interval_type(std::string_view value) {
            return enum_string_converter<interval_type>::from_string(value);
        }
    }

    template<typename DateT>
    /**
     * @brief Converts date-like value types to/from textual representations.
     *
     * Explicit specializations support structured (`type{...}`) and compact
     * (`a-b-c`) formats.
     */
    struct datetype_string_converter;

    template<>
    /** @brief String conversion for @ref datetypes::simple_date. */
    struct datetype_string_converter<datetypes::simple_date> {
        /** @brief Serializes to the canonical structured string form. */
        static std::string to_string(const datetypes::simple_date& date) { return date.to_string(); }
        /** @brief Parses `simple_date{year:...,month:...,day:...}`. */
        static datetypes::simple_date from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "simple_date");
            const datetypes::simple_date date{
                detail::parse_int32(detail::get_required_field(fields, "year", "simple_date"), "year"),
                detail::parse_int32(detail::get_required_field(fields, "month", "simple_date"), "month"),
                detail::parse_int32(detail::get_required_field(fields, "day", "simple_date"), "day")
            };
            if (date.month < 1 || date.day < 1) {
                throw std::invalid_argument("Invalid simple_date: " + std::string(input));
            }
            return date;
        }
        /** @brief Parses compact `year-month-day` representation. */
        static datetypes::simple_date from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 3) throw std::runtime_error("simple_date compact format: year-month-day");
            const datetypes::simple_date date{
                detail::parse_int32(parts[0], "year"),
                detail::parse_int32(parts[1], "month"),
                detail::parse_int32(parts[2], "day")
            };
            if (date.month < 1 || date.day < 1) {
                throw std::invalid_argument("Invalid simple_date: " + std::string(input));
            }
            return date;
        }
    };

    template<>
    /** @brief String conversion for @ref datetypes::gregorian_date. */
    struct datetype_string_converter<datetypes::gregorian_date> {
        /** @brief Serializes to the canonical structured string form. */
        static std::string to_string(const datetypes::gregorian_date& date) { return date.to_string(); }
        /** @brief Parses `gregorian_date{year:...,month:...,day:...}`. */
        static datetypes::gregorian_date from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "gregorian_date");
            const datetypes::gregorian_date date{
                detail::parse_int32(detail::get_required_field(fields, "year", "gregorian_date"), "year"),
                detail::parse_month(detail::get_required_field(fields, "month", "gregorian_date")),
                detail::parse_int32(detail::get_required_field(fields, "day", "gregorian_date"), "day")
            };
            if (!calendars::gregorian_calendar::standard.is_valid_date(date)) {
                throw std::invalid_argument("Invalid gregorian_date: " + std::string(input));
            }
            return date;
        }
        /** @brief Parses compact `year-month-day` representation. */
        static datetypes::gregorian_date from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 3) throw std::runtime_error("gregorian_date compact format: year-month-day");
            const datetypes::gregorian_date date{
                detail::parse_int32(parts[0], "year"),
                detail::parse_month(parts[1]),
                detail::parse_int32(parts[2], "day")
            };
            if (!calendars::gregorian_calendar::standard.is_valid_date(date)) {
                throw std::invalid_argument("Invalid gregorian_date: " + std::string(input));
            }
            return date;
        }
    };

    template<>
    /** @brief String conversion for @ref datetypes::julian_date. */
    struct datetype_string_converter<datetypes::julian_date> {
        /** @brief Serializes to the canonical structured string form. */
        static std::string to_string(const datetypes::julian_date& date) { return date.to_string(); }
        /** @brief Parses `julian_date{year:...,month:...,day:...}`. */
        static datetypes::julian_date from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "julian_date");
            const datetypes::julian_date date{
                detail::parse_int32(detail::get_required_field(fields, "year", "julian_date"), "year"),
                detail::parse_month(detail::get_required_field(fields, "month", "julian_date")),
                detail::parse_int32(detail::get_required_field(fields, "day", "julian_date"), "day")
            };
            if (!calendars::julian_calendar{}.is_valid_date(date)) {
                throw std::invalid_argument("Invalid julian_date: " + std::string(input));
            }
            return date;
        }
        /** @brief Parses compact `year-month-day` representation. */
        static datetypes::julian_date from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 3) throw std::runtime_error("julian_date compact format: year-month-day");
            const datetypes::julian_date date{
                detail::parse_int32(parts[0], "year"),
                detail::parse_month(parts[1]),
                detail::parse_int32(parts[2], "day")
            };
            if (!calendars::julian_calendar{}.is_valid_date(date)) {
                throw std::invalid_argument("Invalid julian_date: " + std::string(input));
            }
            return date;
        }
    };

    template<>
    /** @brief String conversion for @ref datetypes::iso_date. */
    struct datetype_string_converter<datetypes::iso_date> {
        /** @brief Serializes to the canonical structured string form. */
        static std::string to_string(const datetypes::iso_date& date) { return date.to_string(); }
        /** @brief Parses `iso_date{year:...,week:...,day:...}`. */
        static datetypes::iso_date from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "iso_date");
            const datetypes::iso_date date{
                detail::parse_int32(detail::get_required_field(fields, "year", "iso_date"), "year"),
                detail::parse_int32(detail::get_required_field(fields, "week", "iso_date"), "week"),
                detail::parse_int32(detail::get_required_field(fields, "day", "iso_date"), "day")
            };
            if (date.week < 1 || date.day < 1 || date.day > 7 || !calendars::iso_calendar{}.is_valid_date(date)) {
                throw std::invalid_argument("Invalid iso_date: " + std::string(input));
            }
            return date;
        }
        /** @brief Parses compact `year-week-day` representation. */
        static datetypes::iso_date from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 3) throw std::runtime_error("iso_date compact format: year-week-day");
            const datetypes::iso_date date{
                detail::parse_int32(parts[0], "year"),
                detail::parse_int32(parts[1], "week"),
                detail::parse_int32(parts[2], "day")
            };
            if (date.week < 1 || date.day < 1 || date.day > 7 || !calendars::iso_calendar{}.is_valid_date(date)) {
                throw std::invalid_argument("Invalid iso_date: " + std::string(input));
            }
            return date;
        }
    };

    template<>
    /** @brief String conversion for @ref datetypes::hebrew_date. */
    struct datetype_string_converter<datetypes::hebrew_date> {
        /** @brief Serializes to the canonical structured string form. */
        static std::string to_string(const datetypes::hebrew_date& date) { return date.to_string(); }
        /** @brief Parses `hebrew_date{year:...,month:...,day:...}`. */
        static datetypes::hebrew_date from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "hebrew_date");
            const datetypes::hebrew_date date{
                detail::parse_int32(detail::get_required_field(fields, "year", "hebrew_date"), "year"),
                detail::parse_hebrew_month(detail::get_required_field(fields, "month", "hebrew_date")),
                detail::parse_int32(detail::get_required_field(fields, "day", "hebrew_date"), "day")
            };
            if (!calendars::hebrew_calendar{}.is_valid_date(date)) {
                throw std::invalid_argument("Invalid hebrew_date: " + std::string(input));
            }
            return date;
        }
        /** @brief Parses compact `year-month-day` representation. */
        static datetypes::hebrew_date from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 3) throw std::runtime_error("hebrew_date compact format: year-month-day");
            const datetypes::hebrew_date date{
                detail::parse_int32(parts[0], "year"),
                detail::parse_hebrew_month(parts[1]),
                detail::parse_int32(parts[2], "day")
            };
            if (!calendars::hebrew_calendar{}.is_valid_date(date)) {
                throw std::invalid_argument("Invalid hebrew_date: " + std::string(input));
            }
            return date;
        }
    };

    template<>
    /** @brief String conversion for @ref datetypes::simple_lunar_date. */
    struct datetype_string_converter<datetypes::simple_lunar_date> {
        /** @brief Serializes to the canonical structured string form. */
        static std::string to_string(const datetypes::simple_lunar_date& date) { return date.to_string(); }
        /** @brief Parses `simple_lunar_date{...}` including `leap_month`. */
        static datetypes::simple_lunar_date from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "simple_lunar_date");
            const datetypes::simple_lunar_date date{
                detail::parse_int32(detail::get_required_field(fields, "year", "simple_lunar_date"), "year"),
                detail::parse_int32(detail::get_required_field(fields, "month", "simple_lunar_date"), "month"),
                detail::parse_bool(detail::get_required_field(fields, "leap_month", "simple_lunar_date"), "leap_month"),
                detail::parse_int32(detail::get_required_field(fields, "day", "simple_lunar_date"), "day")
            };
            if (date.month < 1 || date.month > 13 || date.day < 1 || date.day > 30) {
                throw std::invalid_argument("Invalid simple_lunar_date: " + std::string(input));
            }
            return date;
        }
        /** @brief Parses compact `year-month-leap_month-day` representation. */
        static datetypes::simple_lunar_date from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 4) throw std::runtime_error("simple_lunar_date compact format: year-month-leap_month-day");
            const datetypes::simple_lunar_date date{
                detail::parse_int32(parts[0], "year"),
                detail::parse_int32(parts[1], "month"),
                detail::parse_bool(parts[2], "leap_month"),
                detail::parse_int32(parts[3], "day")
            };
            if (date.month < 1 || date.month > 13 || date.day < 1 || date.day > 30) {
                throw std::invalid_argument("Invalid simple_lunar_date: " + std::string(input));
            }
            return date;
        }
    };

    template<>
    /** @brief String conversion for @ref datetypes::lunar_date. */
    struct datetype_string_converter<datetypes::lunar_date> {
        /** @brief Serializes to the canonical structured string form. */
        static std::string to_string(const datetypes::lunar_date& date) { return date.to_string(); }
        /** @brief Parses `lunar_date{...}` including leap-month and leap-day markers. */
        static datetypes::lunar_date from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "lunar_date");
            const datetypes::lunar_date date{
                detail::parse_int32(detail::get_required_field(fields, "year", "lunar_date"), "year"),
                detail::parse_int32(detail::get_required_field(fields, "month", "lunar_date"), "month"),
                detail::parse_bool(detail::get_required_field(fields, "leap_month", "lunar_date"), "leap_month"),
                detail::parse_int32(detail::get_required_field(fields, "day", "lunar_date"), "day"),
                detail::parse_bool(detail::get_required_field(fields, "leap_day", "lunar_date"), "leap_day")
            };
            if (date.month < 1 || date.month > 13 || date.day < 1 || date.day > 30) {
                throw std::invalid_argument("Invalid lunar_date: " + std::string(input));
            }
            return date;
        }
        /** @brief Parses compact `year-month-leap_month-day-leap_day` representation. */
        static datetypes::lunar_date from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 5) throw std::runtime_error("lunar_date compact format: year-month-leap_month-day-leap_day");
            const datetypes::lunar_date date{
                detail::parse_int32(parts[0], "year"),
                detail::parse_int32(parts[1], "month"),
                detail::parse_bool(parts[2], "leap_month"),
                detail::parse_int32(parts[3], "day"),
                detail::parse_bool(parts[4], "leap_day")
            };
            if (date.month < 1 || date.month > 13 || date.day < 1 || date.day > 30) {
                throw std::invalid_argument("Invalid lunar_date: " + std::string(input));
            }
            return date;
        }
    };

    template<>
    /** @brief String conversion for @ref datetypes::bahai_date. */
    struct datetype_string_converter<datetypes::bahai_date> {
        /** @brief Serializes to the canonical structured string form. */
        static std::string to_string(const datetypes::bahai_date& date) { return date.to_string(); }
        /** @brief Parses `bahai_date{major:...,cycle:...,year:...,month:...,day:...}`. */
        static datetypes::bahai_date from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "bahai_date");
            const datetypes::bahai_date date{
                detail::parse_int32(detail::get_required_field(fields, "major", "bahai_date"), "major"),
                detail::parse_int32(detail::get_required_field(fields, "cycle", "bahai_date"), "cycle"),
                detail::parse_int32(detail::get_required_field(fields, "year", "bahai_date"), "year"),
                detail::parse_int32(detail::get_required_field(fields, "month", "bahai_date"), "month"),
                detail::parse_int32(detail::get_required_field(fields, "day", "bahai_date"), "day")
            };
            if (!calendars::bahai_calendar{}.is_valid_date(date)) {
                throw std::invalid_argument("Invalid bahai_date: " + std::string(input));
            }
            return date;
        }
        /** @brief Parses compact `major-cycle-year-month-day` representation. */
        static datetypes::bahai_date from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 5) throw std::runtime_error("bahai_date compact format: major-cycle-year-month-day");
            const datetypes::bahai_date date{
                detail::parse_int32(parts[0], "major"),
                detail::parse_int32(parts[1], "cycle"),
                detail::parse_int32(parts[2], "year"),
                detail::parse_int32(parts[3], "month"),
                detail::parse_int32(parts[4], "day")
            };
            if (!calendars::bahai_calendar{}.is_valid_date(date)) {
                throw std::invalid_argument("Invalid bahai_date: " + std::string(input));
            }
            return date;
        }
    };

    template<>
    /** @brief String conversion for @ref datetypes::chinese_date. */
    struct datetype_string_converter<datetypes::chinese_date> {
        /** @brief Serializes to the canonical structured string form. */
        static std::string to_string(const datetypes::chinese_date& date) { return date.to_string(); }
        /** @brief Parses `chinese_date{...}` including cycle and leap-month marker. */
        static datetypes::chinese_date from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "chinese_date");
            const datetypes::chinese_date date{
                detail::parse_int32(detail::get_required_field(fields, "cycle", "chinese_date"), "cycle"),
                detail::parse_int32(detail::get_required_field(fields, "year", "chinese_date"), "year"),
                detail::parse_int32(detail::get_required_field(fields, "month", "chinese_date"), "month"),
                detail::parse_bool(detail::get_required_field(fields, "leap_month", "chinese_date"), "leap_month"),
                detail::parse_int32(detail::get_required_field(fields, "day", "chinese_date"), "day")
            };
            if (date.cycle < 1 || date.year < 1 || date.year > 60 || date.month < 1 || date.month > 12 || date.day < 1 || date.day > 30 || !calendars::chinese_calendar{}.is_valid_date(date)) {
                throw std::invalid_argument("Invalid chinese_date: " + std::string(input));
            }
            return date;
        }
        /** @brief Parses compact `cycle-year-month-leap_month-day` representation. */
        static datetypes::chinese_date from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 5) throw std::runtime_error("chinese_date compact format: cycle-year-month-leap_month-day");
            const datetypes::chinese_date date{
                detail::parse_int32(parts[0], "cycle"),
                detail::parse_int32(parts[1], "year"),
                detail::parse_int32(parts[2], "month"),
                detail::parse_bool(parts[3], "leap_month"),
                detail::parse_int32(parts[4], "day")
            };
            if (date.cycle < 1 || date.year < 1 || date.year > 60 || date.month < 1 || date.month > 12 || date.day < 1 || date.day > 30 || !calendars::chinese_calendar{}.is_valid_date(date)) {
                throw std::invalid_argument("Invalid chinese_date: " + std::string(input));
            }
            return date;
        }
    };

    template<>
    /** @brief String conversion for @ref datetypes::icelandic_date. */
    struct datetype_string_converter<datetypes::icelandic_date> {
        /** @brief Serializes to the canonical structured string form. */
        static std::string to_string(const datetypes::icelandic_date& date) { return date.to_string(); }
        /** @brief Parses `icelandic_date{year:...,season:...,week:...,weekday:...}`. */
        static datetypes::icelandic_date from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "icelandic_date");
            const datetypes::icelandic_date date{
                detail::parse_int32(detail::get_required_field(fields, "year", "icelandic_date"), "year"),
                detail::parse_season(detail::get_required_field(fields, "season", "icelandic_date")),
                detail::parse_int32(detail::get_required_field(fields, "week", "icelandic_date"), "week"),
                detail::parse_weekday(detail::get_required_field(fields, "weekday", "icelandic_date"))
            };
            if (date.week < 1 || !calendars::icelandic_calendar{}.is_valid_date(date)) {
                throw std::invalid_argument("Invalid icelandic_date: " + std::string(input));
            }
            return date;
        }
        /** @brief Parses compact `year-season-week-weekday` representation. */
        static datetypes::icelandic_date from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 4) throw std::runtime_error("icelandic_date compact format: year-season-week-weekday");
            const datetypes::icelandic_date date{
                detail::parse_int32(parts[0], "year"),
                detail::parse_season(parts[1]),
                detail::parse_int32(parts[2], "week"),
                detail::parse_weekday(parts[3])
            };
            if (date.week < 1 || !calendars::icelandic_calendar{}.is_valid_date(date)) {
                throw std::invalid_argument("Invalid icelandic_date: " + std::string(input));
            }
            return date;
        }
    };

    template<>
    /** @brief String conversion for @ref datetypes::roman_date. */
    struct datetype_string_converter<datetypes::roman_date> {
        /** @brief Serializes to the canonical structured string form. */
        static std::string to_string(const datetypes::roman_date& date) { return date.to_string(); }
        /** @brief Parses `roman_date{year:...,month:...,event:...,count:...,leap:...}`. */
        static datetypes::roman_date from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "roman_date");
            const datetypes::roman_date date{
                detail::parse_int32(detail::get_required_field(fields, "year", "roman_date"), "year"),
                detail::parse_month(detail::get_required_field(fields, "month", "roman_date")),
                detail::parse_roman_event(detail::get_required_field(fields, "event", "roman_date")),
                detail::parse_int32(detail::get_required_field(fields, "count", "roman_date"), "count"),
                detail::parse_bool(detail::get_required_field(fields, "leap", "roman_date"), "leap")
            };
            if (date.count < 1 || !calendars::roman_calendar{}.is_valid_date(date)) {
                throw std::invalid_argument("Invalid roman_date: " + std::string(input));
            }
            return date;
        }
        /** @brief Parses compact `year-month-event-count-leap` representation. */
        static datetypes::roman_date from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 5) throw std::runtime_error("roman_date compact format: year-month-event-count-leap");
            const datetypes::roman_date date{
                detail::parse_int32(parts[0], "year"),
                detail::parse_month(parts[1]),
                detail::parse_roman_event(parts[2]),
                detail::parse_int32(parts[3], "count"),
                detail::parse_bool(parts[4], "leap")
            };
            if (date.count < 1 || !calendars::roman_calendar{}.is_valid_date(date)) {
                throw std::invalid_argument("Invalid roman_date: " + std::string(input));
            }
            return date;
        }
    };

    template<>
    /** @brief String conversion for @ref datetypes::sexagesimal_name. */
    struct datetype_string_converter<datetypes::sexagesimal_name> {
        /** @brief Serializes to the canonical structured string form. */
        static std::string to_string(const datetypes::sexagesimal_name& value) { return value.to_string(); }
        /** @brief Parses `sexagesimal_name{stem:...,branch:...}`. */
        static datetypes::sexagesimal_name from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "sexagesimal_name");
            const datetypes::sexagesimal_name name{
                detail::parse_int32(detail::get_required_field(fields, "stem", "sexagesimal_name"), "stem"),
                detail::parse_int32(detail::get_required_field(fields, "branch", "sexagesimal_name"), "branch")
            };
            if (name.stem < 1 || name.stem > 10 || name.branch < 1 || name.branch > 12 || (name.stem % 2 != name.branch % 2)) {
                throw std::invalid_argument("Invalid sexagesimal_name: " + std::string(input));
            }
            return name;
        }
        /** @brief Parses compact `stem-branch` representation. */
        static datetypes::sexagesimal_name from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 2) throw std::runtime_error("sexagesimal_name compact format: stem-branch");
            const datetypes::sexagesimal_name name{
                detail::parse_int32(parts[0], "stem"),
                detail::parse_int32(parts[1], "branch")
            };
            if (name.stem < 1 || name.stem > 10 || name.branch < 1 || name.branch > 12 || (name.stem % 2 != name.branch % 2)) {
                throw std::invalid_argument("Invalid sexagesimal_name: " + std::string(input));
            }
            return name;
        }
    };

    template<>
    /** @brief String conversion for @ref datetypes::olympiad. */
    struct datetype_string_converter<datetypes::olympiad> {
        /** @brief Serializes to the canonical structured string form. */
        static std::string to_string(const datetypes::olympiad& value) { return datetypes::to_string(value); }
        /** @brief Parses `olympiad{cycle:...,year:...}`. */
        static datetypes::olympiad from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "olympiad");
            const datetypes::olympiad o{
                detail::parse_int32(detail::get_required_field(fields, "cycle", "olympiad"), "cycle"),
                detail::parse_int32(detail::get_required_field(fields, "year", "olympiad"), "year")
            };
            if (o.cycle < 1 || o.year < 1 || o.year > 4) {
                throw std::invalid_argument("Invalid olympiad: " + std::string(input));
            }
            return o;
        }
        /** @brief Parses compact `cycle-year` representation. */
        static datetypes::olympiad from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 2) throw std::runtime_error("olympiad compact format: cycle-year");
            const datetypes::olympiad o{
                detail::parse_int32(parts[0], "cycle"),
                detail::parse_int32(parts[1], "year")
            };
            if (o.cycle < 1 || o.year < 1 || o.year > 4) {
                throw std::invalid_argument("Invalid olympiad: " + std::string(input));
            }
            return o;
        }
    };

    template<typename DateT>
    /** @brief Serializes a supported date-like type using its converter specialization. */
    std::string datetype_to_string(const DateT& date) {
        return datetype_string_converter<DateT>::to_string(date);
    }

    template<typename DateT>
    /** @brief Parses a supported date-like type from structured `type{...}` syntax. */
    DateT datetype_from_string(std::string_view input) {
        return datetype_string_converter<DateT>::from_string(input);
    }

    template<typename DateT>
    /** @brief Parses a supported date-like type from compact dash-separated syntax. */
    DateT datetype_from_compact_string(std::string_view input) {
        return datetype_string_converter<DateT>::from_compact_string(input);
    }
}
