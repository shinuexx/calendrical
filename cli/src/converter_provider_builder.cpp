#include "converter_provider_builder.hpp"

#include "calendar_traits.hpp"
#include "calendars/arithmetic_french_calendar.hpp"
#include "calendars/arithmetic_persian_calendar.hpp"
#include "calendars/astro_bahai_calendar.hpp"
#include "calendars/astro_hindu_lunar_calendar.hpp"
#include "calendars/astro_hindu_solar_calendar.hpp"
#include "calendars/babylonian_calendar.hpp"
#include "calendars/bahai_calendar.hpp"
#include "calendars/chinese_calendar.hpp"
#include "calendars/coptic_based_calendar.hpp"
#include "calendars/french_calendar.hpp"
#include "calendars/gregorian_calendar.hpp"
#include "calendars/hebrew_calendar.hpp"
#include "calendars/icelandic_calendar.hpp"
#include "calendars/islamic_calendar.hpp"
#include "calendars/iso_calendar.hpp"
#include "calendars/japanese_calendar.hpp"
#include "calendars/julian_calendar.hpp"
#include "calendars/korean_calendar.hpp"
#include "calendars/modern_hindu_lunar_calendar.hpp"
#include "calendars/modern_hindu_solar_calendar.hpp"
#include "calendars/observational_hebrew_calendar.hpp"
#include "calendars/observational_islamic_calendar.hpp"
#include "calendars/old_hindu_lunar_calendar.hpp"
#include "calendars/old_hindu_solar_calendar.hpp"
#include "calendars/persian_calendar.hpp"
#include "calendars/roman_calendar.hpp"
#include "calendars/samaritan_calendar.hpp"
#include "calendars/saudi_islamic_calendar.hpp"
#include "calendars/simple_calendar.hpp"
#include "calendars/tibetan_calendar.hpp"
#include "calendars/vietnamese_calendar.hpp"
#include "helpers.hpp"
#include "primitives.hpp"

namespace calendrical::cli {
    namespace {
        template<typename CalendarT>
        using calendar_date_t = typename CalendarT::date_type;

        template<typename CalendarT>
        auto compact_date_parser() {
            return [](const std::string& input) {
                return calendar_traits::datetype_from_compact_string<calendar_date_t<CalendarT>>(input);
            };
        }

        template<std::derived_from<calendar_if> CalendarT>
        void add_calendar_converter(
            converter_provider& provider,
            std::string name,
            std::string format) {
            provider.add<CalendarT>(
                std::move(name),
                std::move(format),
                compact_date_parser<CalendarT>()
            );
        }

        template<std::derived_from<calendar_if> CalendarT>
        void add_calendar_converter(
            converter_provider& provider,
            std::string name,
            std::string format,
            CalendarT calendar) {
            provider.add<CalendarT>(
                std::move(name),
                std::move(format),
                compact_date_parser<CalendarT>(),
                std::move(calendar)
            );
        }
    }

    converter_provider build_converter_provider() {
        converter_provider provider{};

        provider.add<int64_t>(
            "rd",
            "NNNNNN",
            [](const std::string& x) { return std::stoll(x); },
            bind_constructor<fixed_date_t>{},
            [](fixed_date_t x) { return static_cast<int64_t>(x); }
        );
        provider.add<double>(
            "jd",
            "NNNNNN(.fraction)",
            [](const std::string& x) { return std::stod(x); },
            [](double x) { return primitives::fixed_from_jd(x); },
            [](fixed_date_t x) { return primitives::jd_from_moment(x); }
        );
        provider.add<int64_t>(
            "mjd",
            "NNNNNN",
            [](const std::string& x) { return std::stoll(x); },
            [](int64_t x) { return primitives::fixed_from_mjd(x); },
            [](fixed_date_t x) { return primitives::mjd_from_fixed(x); }
        );
        provider.add<double>(
            "unix",
            "seconds_since_epoch",
            [](const std::string& x) { return std::stod(x); },
            [](double x) { return primitives::moment_from_unix(x).to_fixed_date(); },
            [](fixed_date_t x) { return primitives::unix_from_moment(x); }
        );
        provider.add<double>(
            "unix_timestamp",
            "seconds_since_epoch",
            [](const std::string& x) { return std::stod(x); },
            [](double x) { return primitives::moment_from_unix(x).to_fixed_date(); },
            [](fixed_date_t x) { return primitives::unix_from_moment(x); }
        );

        add_calendar_converter<calendars::arithmetic_french_calendar>(provider, "arithmetic_french", "YYYY-MM-DD");
        add_calendar_converter<calendars::arithmetic_persian_calendar>(provider, "arithmetic_persian", "YYYY-MM-DD");
        add_calendar_converter<calendars::simple_calendar>(provider, "armenian", "YYYY-MM-DD", calendars::armenian_calendar);
        add_calendar_converter<calendars::astro_bahai_calendar>(provider, "astro_bahai", "MAJOR-CYCLE-YEAR-MONTH-DAY");
        add_calendar_converter<calendars::astro_hindu_lunar_calendar>(provider, "astro_hindu_lunar", "YEAR-MONTH-LEAP_MONTH-DAY-LEAP_DAY");
        add_calendar_converter<calendars::astro_hindu_solar_calendar>(provider, "astro_hindu_solar", "YYYY-MM-DD");
        add_calendar_converter<calendars::babylonian_calendar>(provider, "babylonian", "YEAR-MONTH-LEAP_MONTH-DAY");
        add_calendar_converter<calendars::bahai_calendar>(provider, "bahai", "MAJOR-CYCLE-YEAR-MONTH-DAY");
        add_calendar_converter<calendars::chinese_calendar>(provider, "chinese", "CYCLE-YEAR-MONTH-LEAP_MONTH-DAY");
        add_calendar_converter<calendars::coptic_based_calendar>(provider, "coptic", "YYYY-MM-DD", calendars::coptic_calendar);
        add_calendar_converter<calendars::simple_calendar>(provider, "egyptian", "YYYY-MM-DD", calendars::egyptian_calendar);
        add_calendar_converter<calendars::coptic_based_calendar>(provider, "ethiopic", "YYYY-MM-DD", calendars::ethiopic_calendar);
        add_calendar_converter<calendars::french_calendar>(provider, "french", "YYYY-MM-DD");
        add_calendar_converter<calendars::gregorian_calendar>(provider, "gregorian", "YYYY-MONTH-DD");
        add_calendar_converter<calendars::hebrew_calendar>(provider, "hebrew", "YYYY-MONTH-DD");
        add_calendar_converter<calendars::icelandic_calendar>(provider, "icelandic", "YEAR-SEASON-WEEK-WEEKDAY");
        add_calendar_converter<calendars::islamic_calendar>(provider, "islamic", "YYYY-MM-DD");
        add_calendar_converter<calendars::iso_calendar>(provider, "iso", "YEAR-WEEK-DAY");
        add_calendar_converter<calendars::japanese_calendar>(provider, "japanese", "CYCLE-YEAR-MONTH-LEAP_MONTH-DAY");
        add_calendar_converter<calendars::julian_calendar>(provider, "julian", "YYYY-MONTH-DD");
        add_calendar_converter<calendars::korean_calendar>(provider, "korean", "CYCLE-YEAR-MONTH-LEAP_MONTH-DAY");
        add_calendar_converter<calendars::modern_hindu_lunar_calendar>(provider, "modern_hindu_lunar", "YEAR-MONTH-LEAP_MONTH-DAY-LEAP_DAY");
        add_calendar_converter<calendars::modern_hindu_solar_calendar>(provider, "modern_hindu_solar", "YYYY-MM-DD");
        add_calendar_converter<calendars::observational_hebrew_calendar>(provider, "observational_hebrew", "YYYY-MONTH-DD");
        add_calendar_converter<calendars::observational_islamic_calendar>(provider, "observational_islamic", "YYYY-MM-DD");
        add_calendar_converter<calendars::old_hindu_lunar_calendar>(provider, "old_hindu_lunar", "YEAR-MONTH-LEAP_MONTH-DAY");
        add_calendar_converter<calendars::old_hindu_solar_calendar>(provider, "old_hindu_solar", "YYYY-MM-DD");
        add_calendar_converter<calendars::persian_calendar>(provider, "persian", "YYYY-MM-DD");
        add_calendar_converter<calendars::roman_calendar>(provider, "roman", "YEAR-MONTH-EVENT-COUNT-LEAP");
        add_calendar_converter<calendars::samaritan_calendar>(provider, "samaritan", "YYYY-MM-DD");
        add_calendar_converter<calendars::saudi_islamic_calendar>(provider, "saudi_islamic", "YYYY-MM-DD");
        add_calendar_converter<calendars::tibetan_calendar>(provider, "tibetan", "YEAR-MONTH-LEAP_MONTH-DAY-LEAP_DAY");
        add_calendar_converter<calendars::vietnamese_calendar>(provider, "vietnamese", "CYCLE-YEAR-MONTH-LEAP_MONTH-DAY");

        return provider;
    }
}
