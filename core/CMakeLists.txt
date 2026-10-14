add_library(calendrical
    src/angle.cpp
    src/chinese_marriage_augury.cpp
    src/fixed_date.cpp
    src/hebrew_month.cpp
    src/helpers.cpp
    src/location.cpp
    src/lunar_phase.cpp
    src/moment.cpp
    src/month.cpp
    src/roman_event.cpp
    src/season.cpp
    src/time_and_astronomy.cpp
    src/weekday.cpp
    src/calendars/arithmetic_french_calendar.cpp
    src/calendars/arithmetic_persian_calendar.cpp
    src/calendars/astro_bahai_calendar.cpp
    src/calendars/astro_hindu_lunar_calendar.cpp
    src/calendars/astro_hindu_solar_calendar.cpp
    src/calendars/babylonian_calendar.cpp
    src/calendars/bahai_based_calendar.cpp
    src/calendars/bahai_calendar.cpp
    src/calendars/chinese_base.cpp
    src/calendars/chinese_based_calendar.cpp
    src/calendars/chinese_calendar.cpp
    src/calendars/coptic_based_calendar.cpp
    src/calendars/french_based_calendar.cpp
    src/calendars/french_calendar.cpp
    src/calendars/gregorian_calendar.cpp
    src/calendars/hebrew_based_calendar.cpp
    src/calendars/hebrew_calendar.cpp
    src/calendars/hindu_base.cpp
    src/calendars/icelandic_calendar.cpp
    src/calendars/islamic_calendar.cpp
    src/calendars/iso_calendar.cpp
    src/calendars/japanese_calendar.cpp
    src/calendars/julian_calendar.cpp
    src/calendars/korean_calendar.cpp
    src/calendars/modern_hindu_based.cpp
    src/calendars/modern_hindu_lunar_calendar.cpp
    src/calendars/modern_hindu_solar_calendar.cpp
    src/calendars/observational_hebrew_calendar.cpp
    src/calendars/observational_islamic_calendar.cpp
    src/calendars/old_hindu_lunar_calendar.cpp
    src/calendars/old_hindu_solar_calendar.cpp
    src/calendars/persian_based_calendar.cpp
    src/calendars/persian_calendar.cpp
    src/calendars/roman_calendar.cpp
    src/calendars/samaritan_calendar.cpp
    src/calendars/saudi_islamic_calendar.cpp
    src/calendars/simple_calendar.cpp
    src/calendars/tibetan_calendar.cpp
    src/calendars/vietnamese_calendar.cpp
    src/datetypes/sexagesimal_name.cpp
    src/datetypes/stringifiers.cpp
    src/holiday_providers/bahai_based_calendar_holiday_provider.cpp
    src/holiday_providers/chinese_calendar_holiday_provider.cpp
    src/holiday_providers/coptic_based_calendar_holiday_provider.cpp
    src/holiday_providers/gregorian_calendar_holiday_provider.cpp
    src/holiday_providers/hebrew_based_calendar_holiday_provider.cpp
    src/holiday_providers/islamic_based_calendar_holiday_provider.cpp
    src/holiday_providers/julian_calendar_holiday_provider.cpp
    src/holiday_providers/modern_hindu_lunar_calendar_holiday_provider.cpp
    src/holiday_providers/modern_hindu_solar_calendar_holiday_provider.cpp
    src/holiday_providers/persian_based_calendar_holiday_provider.cpp
    src/holiday_providers/samaritan_calendar_holiday_provider.cpp
    src/holiday_providers/simple_calendar_holiday_provider.cpp
    src/holiday_providers/tibetan_calendar_holiday_provider.cpp
)

add_library(calendrical::calendrical ALIAS calendrical)

target_compile_features(calendrical PUBLIC cxx_std_20)

target_link_libraries(calendrical PRIVATE $<BUILD_INTERFACE:calendrical_warning_flags>)

target_include_directories(calendrical
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/calendrical>
)

install(TARGETS calendrical
    EXPORT calendricalTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/calendrical
)

# Headers include each other by bare name ("types.hpp"), so they get their own directory
# instead of landing directly in <prefix>/include.
install(DIRECTORY inc/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/calendrical
)
