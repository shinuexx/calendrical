#include "holiday_provider_provider_builder.hpp"

#include "calendars/astro_bahai_calendar.hpp"
#include "calendars/bahai_calendar.hpp"
#include "calendars/chinese_calendar.hpp"
#include "calendars/hebrew_calendar.hpp"
#include "calendars/islamic_calendar.hpp"
#include "calendars/korean_calendar.hpp"
#include "calendars/modern_hindu_lunar_calendar.hpp"
#include "calendars/modern_hindu_solar_calendar.hpp"
#include "calendars/pawukon_calendar.hpp"
#include "calendars/persian_calendar.hpp"
#include "calendars/samaritan_calendar.hpp"
#include "calendars/tibetan_calendar.hpp"
#include "calendars/vietnamese_calendar.hpp"
#include "holiday_providers/bahai_based_calendar_holiday_provider.hpp"
#include "holiday_providers/chinese_calendar_holiday_provider.hpp"
#include "holiday_providers/gregorian_calendar_holiday_provider.hpp"
#include "holiday_providers/hebrew_based_calendar_holiday_provider.hpp"
#include "holiday_providers/islamic_based_calendar_holiday_provider.hpp"
#include "holiday_providers/julian_calendar_holiday_provider.hpp"
#include "holiday_providers/modern_hindu_lunar_calendar_holiday_provider.hpp"
#include "holiday_providers/modern_hindu_solar_calendar_holiday_provider.hpp"
#include "holiday_providers/pawukon_calendar_holiday_provider.hpp"
#include "holiday_providers/persian_based_calendar_holiday_provider.hpp"
#include "holiday_providers/samaritan_calendar_holiday_provider.hpp"
#include "holiday_providers/simple_calendar_holiday_provider.hpp"
#include "holiday_providers/tibetan_calendar_holiday_provider.hpp"
#include "holiday_providers/us_holiday_provider.hpp"

namespace calendrical::cli {
    namespace {
        /** @brief Process-wide calendar instance shared by the registered holiday providers. */
        template<typename CalendarT>
        const CalendarT& calendar_instance() {
            static const CalendarT instance{};
            return instance;
        }
    }

    holiday_provider_provider build_holiday_provider_provider() {
        holiday_provider_provider provider{};

        provider.add<holiday_providers::gregorian_calendar_holiday_provider>("gregorian");
        provider.add<holiday_providers::us_holiday_provider>("us");
        provider.add<holiday_providers::julian_calendar_holiday_provider>("julian");
        provider.add<holiday_providers::islamic_based_calendar_holiday_provider>(
            "islamic_based",
            std::addressof(calendar_instance<calendars::islamic_calendar>())
        );
        provider.add<holiday_providers::hebrew_based_calendar_holiday_provider>(
            "hebrew_based",
            std::addressof(calendar_instance<calendars::hebrew_calendar>())
        );
        provider.add<holiday_providers::bahai_based_calendar_holiday_provider>(
            "bahai_based",
            std::addressof(calendar_instance<calendars::astro_bahai_calendar>()),
            std::addressof(calendar_instance<calendars::bahai_calendar>())
        );
        provider.add<holiday_providers::persian_based_calendar_holiday_provider>(
            "persian_based",
            std::addressof(calendar_instance<calendars::persian_calendar>())
        );
        provider.add<holiday_providers::samaritan_calendar_holiday_provider>(
            "samaritan",
            std::addressof(calendar_instance<calendars::samaritan_calendar>())
        );
        provider.add<holiday_providers::modern_hindu_lunar_calendar_holiday_provider>(
            "modern_hindu_lunar",
            std::addressof(calendar_instance<calendars::modern_hindu_lunar_calendar>())
        );
        provider.add<holiday_providers::modern_hindu_solar_calendar_holiday_provider>(
            "modern_hindu_solar",
            std::addressof(calendar_instance<calendars::modern_hindu_solar_calendar>())
        );
        provider.add<holiday_providers::chinese_calendar_holiday_provider>(
            "chinese",
            std::addressof(calendar_instance<calendars::chinese_calendar>())
        );
        provider.add<holiday_providers::chinese_calendar_holiday_provider>(
            "korean",
            std::addressof(calendar_instance<calendars::korean_calendar>())
        );
        provider.add<holiday_providers::chinese_calendar_holiday_provider>(
            "vietnamese",
            std::addressof(calendar_instance<calendars::vietnamese_calendar>())
        );
        provider.add<holiday_providers::tibetan_calendar_holiday_provider>(
            "tibetan",
            std::addressof(calendar_instance<calendars::tibetan_calendar>())
        );
        provider.add<holiday_providers::pawukon_calendar_holiday_provider>(
            "pawukon",
            std::addressof(calendar_instance<calendars::pawukon_calendar>())
        );

        provider.add<holiday_providers::coptic_based_calendar_holiday_provider>("coptic", &calendars::coptic_calendar);
        provider.add<holiday_providers::coptic_based_calendar_holiday_provider>("ethiopic", &calendars::ethiopic_calendar);
        provider.add<holiday_providers::simple_calendar_holiday_provider>("egyptian", &calendars::egyptian_calendar);
        provider.add<holiday_providers::simple_calendar_holiday_provider>("armenian", &calendars::armenian_calendar);

        return provider;
    }
}
