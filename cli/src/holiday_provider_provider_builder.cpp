#include "holiday_provider_provider_builder.hpp"

#include "calendars/astro_bahai_calendar.hpp"
#include "calendars/bahai_calendar.hpp"
#include "calendars/hebrew_calendar.hpp"
#include "calendars/islamic_calendar.hpp"
#include "calendars/modern_hindu_lunar_calendar.hpp"
#include "calendars/persian_calendar.hpp"
#include "holiday_providers/bahai_based_calendar_holiday_provider.hpp"
#include "holiday_providers/gregorian_calendar_holiday_provider.hpp"
#include "holiday_providers/hebrew_based_calendar_holiday_provider.hpp"
#include "holiday_providers/islamic_based_calendar_holiday_provider.hpp"
#include "holiday_providers/julian_calendar_holiday_provider.hpp"
#include "holiday_providers/modern_hindu_lunar_calendar_holiday_provider.hpp"
#include "holiday_providers/persian_based_calendar_holiday_provider.hpp"
#include "holiday_providers/simple_calendar_holiday_provider.hpp"

namespace calendrical::cli {
    namespace {
        template<typename CalendarT>
        const CalendarT& calendar_instance() {
            static const CalendarT instance{};
            return instance;
        }
    }

    holiday_provider_provider build_holiday_provider_provider() {
        holiday_provider_provider provider{};

        provider.add<holiday_providers::gregorian_calendar_holiday_provider>("gregorian");
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
        provider.add<holiday_providers::modern_hindu_lunar_calendar_holiday_provider>(
            "modern_hindu_lunar",
            std::addressof(calendar_instance<calendars::modern_hindu_lunar_calendar>())
        );

        provider.add<holiday_providers::coptic_based_calendar_holiday_provider>("coptic", &calendars::coptic_calendar);
        provider.add<holiday_providers::coptic_based_calendar_holiday_provider>("ethiopic", &calendars::ethiopic_calendar);
        provider.add<holiday_providers::simple_calendar_holiday_provider>("egyptian", &calendars::egyptian_calendar);
        provider.add<holiday_providers::simple_calendar_holiday_provider>("armenian", &calendars::armenian_calendar);

        return provider;
    }
}
