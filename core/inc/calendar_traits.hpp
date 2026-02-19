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
#include "hebrew_month.hpp"
#include "month.hpp"
#include "roman_event.hpp"
#include "season.hpp"
#include "weekday.hpp"

namespace calendrical::calendar_traits {
    template<typename EnumT>
    struct enum_string_converter {
        // ReSharper disable once CppFunctionIsNotImplemented
        static EnumT from_string(std::string_view);
    };

    namespace detail {
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

        inline std::string lower_copy(std::string_view value) {
            std::string out{value};
            for (auto& c : out) {
                c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            }
            return out;
        }

        inline int32_t parse_int32(std::string_view value, std::string_view field_name) {
            int32_t parsed{};
            if (const auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.size(), parsed);
                ec != std::errc{} || ptr != value.data() + value.size()) {
                throw std::runtime_error("Invalid int value for field '" + std::string(field_name) + "': " + std::string(value));
            }
            return parsed;
        }

        inline bool parse_bool(std::string_view value, std::string_view field_name) {
            if (value == "true" || value == "1") return true;
            if (value == "false" || value == "0") return false;
            throw std::runtime_error("Invalid bool value for field '" + std::string(field_name) + "': " + std::string(value));
        }

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

        inline std::string_view get_required_field(
            const std::map<std::string, std::string_view, std::less<>>& fields,
            std::string_view field_name,
            std::string_view type_name) {
            if (const auto it = fields.find(field_name); it != fields.end()) {
                return it->second;
            }
            throw std::runtime_error("Missing field '" + std::string(field_name) + "' in " + std::string(type_name));
        }

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
    struct enum_string_converter<month> {
        static month from_string(std::string_view value) {
            if (!value.empty() && std::isdigit(static_cast<unsigned char>(value.front())) != 0) {
                if (const auto m = detail::parse_int32(value, "month");
                    m >= static_cast<int32_t>(month::january) && m <= static_cast<int32_t>(month::december)) {
                    return static_cast<month>(m);
                }
                throw std::runtime_error("Invalid month value: " + std::string(value));
            }

            const auto parsed = from_parser<month>::from_string(detail::lower_copy(value));
            if (parsed == static_cast<month>(-1)) {
                throw std::runtime_error("Invalid month value: " + std::string(value));
            }
            return parsed;
        }
    };

    template<>
    struct enum_string_converter<hebrew_month> {
        static hebrew_month from_string(std::string_view value) {
            if (!value.empty() && std::isdigit(static_cast<unsigned char>(value.front())) != 0) {
                if (const auto m = detail::parse_int32(value, "hebrew_month");
                    m >= static_cast<int32_t>(hebrew_month::nisan) && m <= static_cast<int32_t>(hebrew_month::adarii)) {
                    return static_cast<hebrew_month>(m);
                }
                throw std::runtime_error("Invalid hebrew month value: " + std::string(value));
            }

            const auto parsed = from_parser<hebrew_month>::from_string(detail::lower_copy(value));
            if (parsed == static_cast<hebrew_month>(-1)) {
                throw std::runtime_error("Invalid hebrew month value: " + std::string(value));
            }
            return parsed;
        }
    };

    template<>
    struct enum_string_converter<season> {
        static season from_string(std::string_view value) {
            const auto lower = detail::lower_copy(value);
            if (lower == "spring") return season::SPRING;
            if (lower == "summer") return season::SUMMER;
            if (lower == "autumn") return season::AUTUMN;
            if (lower == "winter") return season::WINTER;

            switch (const auto numeric = detail::parse_int32(value, "season")) {
                case static_cast<int32_t>(season::SPRING): return season::SPRING;
                case static_cast<int32_t>(season::SUMMER): return season::SUMMER;
                case static_cast<int32_t>(season::AUTUMN): return season::AUTUMN;
                case static_cast<int32_t>(season::WINTER): return season::WINTER;
                default: throw std::runtime_error("Invalid season value: " + std::string(value));
            }
        }
    };

    template<>
    struct enum_string_converter<roman_event> {
        static roman_event from_string(std::string_view value) {
            const auto lower = detail::lower_copy(value);
            if (lower == "kalends") return roman_event::KALENDS;
            if (lower == "nones") return roman_event::NONES;
            if (lower == "ides") return roman_event::IDES;

            switch (const auto numeric = detail::parse_int32(value, "roman_event")) {
                case static_cast<int32_t>(roman_event::KALENDS): return roman_event::KALENDS;
                case static_cast<int32_t>(roman_event::NONES): return roman_event::NONES;
                case static_cast<int32_t>(roman_event::IDES): return roman_event::IDES;
                default: throw std::runtime_error("Invalid roman_event value: " + std::string(value));
            }
        }
    };

    namespace detail {
        inline month parse_month(std::string_view value) {
            return enum_string_converter<month>::from_string(value);
        }

        inline hebrew_month parse_hebrew_month(std::string_view value) {
            return enum_string_converter<hebrew_month>::from_string(value);
        }

        inline season parse_season(std::string_view value) {
            return enum_string_converter<season>::from_string(value);
        }

        inline roman_event parse_roman_event(std::string_view value) {
            return enum_string_converter<roman_event>::from_string(value);
        }
    }

    template<typename DateT>
    struct datetype_string_converter;

    template<>
    struct datetype_string_converter<datetypes::simple_date> {
        static std::string to_string(const datetypes::simple_date& date) { return date.to_string(); }
        static datetypes::simple_date from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "simple_date");
            return {
                detail::parse_int32(detail::get_required_field(fields, "year", "simple_date"), "year"),
                detail::parse_int32(detail::get_required_field(fields, "month", "simple_date"), "month"),
                detail::parse_int32(detail::get_required_field(fields, "day", "simple_date"), "day")
            };
        }
        static datetypes::simple_date from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 3) throw std::runtime_error("simple_date compact format: year-month-day");
            return {
                detail::parse_int32(parts[0], "year"),
                detail::parse_int32(parts[1], "month"),
                detail::parse_int32(parts[2], "day")
            };
        }
    };

    template<>
    struct datetype_string_converter<datetypes::gregorian_date> {
        static std::string to_string(const datetypes::gregorian_date& date) { return date.to_string(); }
        static datetypes::gregorian_date from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "gregorian_date");
            return {
                detail::parse_int32(detail::get_required_field(fields, "year", "gregorian_date"), "year"),
                detail::parse_month(detail::get_required_field(fields, "month", "gregorian_date")),
                detail::parse_int32(detail::get_required_field(fields, "day", "gregorian_date"), "day")
            };
        }
        static datetypes::gregorian_date from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 3) throw std::runtime_error("gregorian_date compact format: year-month-day");
            return {
                detail::parse_int32(parts[0], "year"),
                detail::parse_month(parts[1]),
                detail::parse_int32(parts[2], "day")
            };
        }
    };

    template<>
    struct datetype_string_converter<datetypes::julian_date> {
        static std::string to_string(const datetypes::julian_date& date) { return date.to_string(); }
        static datetypes::julian_date from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "julian_date");
            return {
                detail::parse_int32(detail::get_required_field(fields, "year", "julian_date"), "year"),
                detail::parse_month(detail::get_required_field(fields, "month", "julian_date")),
                detail::parse_int32(detail::get_required_field(fields, "day", "julian_date"), "day")
            };
        }
        static datetypes::julian_date from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 3) throw std::runtime_error("julian_date compact format: year-month-day");
            return {
                detail::parse_int32(parts[0], "year"),
                detail::parse_month(parts[1]),
                detail::parse_int32(parts[2], "day")
            };
        }
    };

    template<>
    struct datetype_string_converter<datetypes::iso_date> {
        static std::string to_string(const datetypes::iso_date& date) { return date.to_string(); }
        static datetypes::iso_date from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "iso_date");
            return {
                detail::parse_int32(detail::get_required_field(fields, "year", "iso_date"), "year"),
                detail::parse_int32(detail::get_required_field(fields, "week", "iso_date"), "week"),
                detail::parse_int32(detail::get_required_field(fields, "day", "iso_date"), "day")
            };
        }
        static datetypes::iso_date from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 3) throw std::runtime_error("iso_date compact format: year-week-day");
            return {
                detail::parse_int32(parts[0], "year"),
                detail::parse_int32(parts[1], "week"),
                detail::parse_int32(parts[2], "day")
            };
        }
    };

    template<>
    struct datetype_string_converter<datetypes::hebrew_date> {
        static std::string to_string(const datetypes::hebrew_date& date) { return date.to_string(); }
        static datetypes::hebrew_date from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "hebrew_date");
            return {
                detail::parse_int32(detail::get_required_field(fields, "year", "hebrew_date"), "year"),
                detail::parse_hebrew_month(detail::get_required_field(fields, "month", "hebrew_date")),
                detail::parse_int32(detail::get_required_field(fields, "day", "hebrew_date"), "day")
            };
        }
        static datetypes::hebrew_date from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 3) throw std::runtime_error("hebrew_date compact format: year-month-day");
            return {
                detail::parse_int32(parts[0], "year"),
                detail::parse_hebrew_month(parts[1]),
                detail::parse_int32(parts[2], "day")
            };
        }
    };

    template<>
    struct datetype_string_converter<datetypes::simple_lunar_date> {
        static std::string to_string(const datetypes::simple_lunar_date& date) { return date.to_string(); }
        static datetypes::simple_lunar_date from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "simple_lunar_date");
            return {
                detail::parse_int32(detail::get_required_field(fields, "year", "simple_lunar_date"), "year"),
                detail::parse_int32(detail::get_required_field(fields, "month", "simple_lunar_date"), "month"),
                detail::parse_bool(detail::get_required_field(fields, "leap_month", "simple_lunar_date"), "leap_month"),
                detail::parse_int32(detail::get_required_field(fields, "day", "simple_lunar_date"), "day")
            };
        }
        static datetypes::simple_lunar_date from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 4) throw std::runtime_error("simple_lunar_date compact format: year-month-leap_month-day");
            return {
                detail::parse_int32(parts[0], "year"),
                detail::parse_int32(parts[1], "month"),
                detail::parse_bool(parts[2], "leap_month"),
                detail::parse_int32(parts[3], "day")
            };
        }
    };

    template<>
    struct datetype_string_converter<datetypes::lunar_date> {
        static std::string to_string(const datetypes::lunar_date& date) { return date.to_string(); }
        static datetypes::lunar_date from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "lunar_date");
            return {
                detail::parse_int32(detail::get_required_field(fields, "year", "lunar_date"), "year"),
                detail::parse_int32(detail::get_required_field(fields, "month", "lunar_date"), "month"),
                detail::parse_bool(detail::get_required_field(fields, "leap_month", "lunar_date"), "leap_month"),
                detail::parse_int32(detail::get_required_field(fields, "day", "lunar_date"), "day"),
                detail::parse_bool(detail::get_required_field(fields, "leap_day", "lunar_date"), "leap_day")
            };
        }
        static datetypes::lunar_date from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 5) throw std::runtime_error("lunar_date compact format: year-month-leap_month-day-leap_day");
            return {
                detail::parse_int32(parts[0], "year"),
                detail::parse_int32(parts[1], "month"),
                detail::parse_bool(parts[2], "leap_month"),
                detail::parse_int32(parts[3], "day"),
                detail::parse_bool(parts[4], "leap_day")
            };
        }
    };

    template<>
    struct datetype_string_converter<datetypes::bahai_date> {
        static std::string to_string(const datetypes::bahai_date& date) { return date.to_string(); }
        static datetypes::bahai_date from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "bahai_date");
            return {
                detail::parse_int32(detail::get_required_field(fields, "major", "bahai_date"), "major"),
                detail::parse_int32(detail::get_required_field(fields, "cycle", "bahai_date"), "cycle"),
                detail::parse_int32(detail::get_required_field(fields, "year", "bahai_date"), "year"),
                detail::parse_int32(detail::get_required_field(fields, "month", "bahai_date"), "month"),
                detail::parse_int32(detail::get_required_field(fields, "day", "bahai_date"), "day")
            };
        }
        static datetypes::bahai_date from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 5) throw std::runtime_error("bahai_date compact format: major-cycle-year-month-day");
            return {
                detail::parse_int32(parts[0], "major"),
                detail::parse_int32(parts[1], "cycle"),
                detail::parse_int32(parts[2], "year"),
                detail::parse_int32(parts[3], "month"),
                detail::parse_int32(parts[4], "day")
            };
        }
    };

    template<>
    struct datetype_string_converter<datetypes::chinese_date> {
        static std::string to_string(const datetypes::chinese_date& date) { return date.to_string(); }
        static datetypes::chinese_date from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "chinese_date");
            return {
                detail::parse_int32(detail::get_required_field(fields, "cycle", "chinese_date"), "cycle"),
                detail::parse_int32(detail::get_required_field(fields, "year", "chinese_date"), "year"),
                detail::parse_int32(detail::get_required_field(fields, "month", "chinese_date"), "month"),
                detail::parse_bool(detail::get_required_field(fields, "leap_month", "chinese_date"), "leap_month"),
                detail::parse_int32(detail::get_required_field(fields, "day", "chinese_date"), "day")
            };
        }
        static datetypes::chinese_date from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 5) throw std::runtime_error("chinese_date compact format: cycle-year-month-leap_month-day");
            return {
                detail::parse_int32(parts[0], "cycle"),
                detail::parse_int32(parts[1], "year"),
                detail::parse_int32(parts[2], "month"),
                detail::parse_bool(parts[3], "leap_month"),
                detail::parse_int32(parts[4], "day")
            };
        }
    };

    template<>
    struct datetype_string_converter<datetypes::icelandic_date> {
        static std::string to_string(const datetypes::icelandic_date& date) { return date.to_string(); }
        static datetypes::icelandic_date from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "icelandic_date");
            return {
                detail::parse_int32(detail::get_required_field(fields, "year", "icelandic_date"), "year"),
                detail::parse_season(detail::get_required_field(fields, "season", "icelandic_date")),
                detail::parse_int32(detail::get_required_field(fields, "week", "icelandic_date"), "week"),
                detail::parse_weekday(detail::get_required_field(fields, "weekday", "icelandic_date"))
            };
        }
        static datetypes::icelandic_date from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 4) throw std::runtime_error("icelandic_date compact format: year-season-week-weekday");
            return {
                detail::parse_int32(parts[0], "year"),
                detail::parse_season(parts[1]),
                detail::parse_int32(parts[2], "week"),
                detail::parse_weekday(parts[3])
            };
        }
    };

    template<>
    struct datetype_string_converter<datetypes::roman_date> {
        static std::string to_string(const datetypes::roman_date& date) { return date.to_string(); }
        static datetypes::roman_date from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "roman_date");
            return {
                detail::parse_int32(detail::get_required_field(fields, "year", "roman_date"), "year"),
                detail::parse_month(detail::get_required_field(fields, "month", "roman_date")),
                detail::parse_roman_event(detail::get_required_field(fields, "event", "roman_date")),
                detail::parse_int32(detail::get_required_field(fields, "count", "roman_date"), "count"),
                detail::parse_bool(detail::get_required_field(fields, "leap", "roman_date"), "leap")
            };
        }
        static datetypes::roman_date from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 5) throw std::runtime_error("roman_date compact format: year-month-event-count-leap");
            return {
                detail::parse_int32(parts[0], "year"),
                detail::parse_month(parts[1]),
                detail::parse_roman_event(parts[2]),
                detail::parse_int32(parts[3], "count"),
                detail::parse_bool(parts[4], "leap")
            };
        }
    };

    template<>
    struct datetype_string_converter<datetypes::sexagesimal_name> {
        static std::string to_string(const datetypes::sexagesimal_name& value) { return value.to_string(); }
        static datetypes::sexagesimal_name from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "sexagesimal_name");
            return {
                detail::parse_int32(detail::get_required_field(fields, "stem", "sexagesimal_name"), "stem"),
                detail::parse_int32(detail::get_required_field(fields, "branch", "sexagesimal_name"), "branch")
            };
        }
        static datetypes::sexagesimal_name from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 2) throw std::runtime_error("sexagesimal_name compact format: stem-branch");
            return {
                detail::parse_int32(parts[0], "stem"),
                detail::parse_int32(parts[1], "branch")
            };
        }
    };

    template<>
    struct datetype_string_converter<datetypes::olympiad> {
        static std::string to_string(const datetypes::olympiad& value) { return datetypes::to_string(value); }
        static datetypes::olympiad from_string(std::string_view input) {
            const auto fields = detail::parse_record(input, "olympiad");
            return {
                detail::parse_int32(detail::get_required_field(fields, "cycle", "olympiad"), "cycle"),
                detail::parse_int32(detail::get_required_field(fields, "year", "olympiad"), "year")
            };
        }
        static datetypes::olympiad from_compact_string(std::string_view input) {
            const auto parts = detail::split_parts(input, '-');
            if (parts.size() != 2) throw std::runtime_error("olympiad compact format: cycle-year");
            return {
                detail::parse_int32(parts[0], "cycle"),
                detail::parse_int32(parts[1], "year")
            };
        }
    };

    template<typename DateT>
    std::string datetype_to_string(const DateT& date) {
        return datetype_string_converter<DateT>::to_string(date);
    }

    template<typename DateT>
    DateT datetype_from_string(std::string_view input) {
        return datetype_string_converter<DateT>::from_string(input);
    }

    template<typename DateT>
    DateT datetype_from_compact_string(std::string_view input) {
        return datetype_string_converter<DateT>::from_compact_string(input);
    }
}
