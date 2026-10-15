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
    src/calendars/akan_calendar.cpp
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
    src/calendars/pawukon_calendar.cpp
    src/calendars/persian_based_calendar.cpp
    src/calendars/persian_calendar.cpp
    src/calendars/roman_calendar.cpp
    src/calendars/samaritan_calendar.cpp
    src/calendars/saudi_islamic_calendar.cpp
    src/calendars/simple_calendar.cpp
    src/calendars/tibetan_calendar.cpp
    src/calendars/vietnamese_calendar.cpp
    src/datetypes/akan_name.cpp
    src/datetypes/pawukon_date.cpp
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
    src/holiday_providers/pawukon_calendar_holiday_provider.cpp
    src/holiday_providers/persian_based_calendar_holiday_provider.cpp
    src/holiday_providers/samaritan_calendar_holiday_provider.cpp
    src/holiday_providers/simple_calendar_holiday_provider.cpp
    src/holiday_providers/tibetan_calendar_holiday_provider.cpp
    src/holiday_providers/us_holiday_provider.cpp
)

add_library(calendrical::calendrical ALIAS calendrical)

# Version and build information (see version.hpp).
# version_config.hpp is generated once per configure; build_info.cpp is refreshed on every build
# by cmake/build_info.cmake, which rewrites it only when its content changes.
set(CALENDRICAL_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
configure_file("${PROJECT_SOURCE_DIR}/cmake/version_config.hpp.in"
    "${CALENDRICAL_GENERATED_DIR}/inc/version_config.hpp" @ONLY)

find_package(Git QUIET)
set(CALENDRICAL_BUILD_INFO_CPP "${CALENDRICAL_GENERATED_DIR}/src/build_info.cpp")
set(CALENDRICAL_BUILD_INFO_ARGS
    "-DSOURCE_DIR=${PROJECT_SOURCE_DIR}"
    "-DINPUT=${PROJECT_SOURCE_DIR}/cmake/build_info.cpp.in"
    "-DOUTPUT=${CALENDRICAL_BUILD_INFO_CPP}"
    "-DVERSION=${PROJECT_VERSION}"
    "-DCOMPILER=${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}"
    "-DPLATFORM=${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}"
    "-DWITH_TIMESTAMP=${CALENDRICAL_BUILD_TIMESTAMP}"
    "-DGIT_EXECUTABLE=${GIT_EXECUTABLE}"
)
# Generate once now so the source exists for every generator, then refresh on each build.
execute_process(COMMAND "${CMAKE_COMMAND}" ${CALENDRICAL_BUILD_INFO_ARGS}
    -P "${PROJECT_SOURCE_DIR}/cmake/build_info.cmake")
add_custom_target(calendrical_build_info
    COMMAND "${CMAKE_COMMAND}" ${CALENDRICAL_BUILD_INFO_ARGS}
        -P "${PROJECT_SOURCE_DIR}/cmake/build_info.cmake"
    BYPRODUCTS "${CALENDRICAL_BUILD_INFO_CPP}"
    COMMENT "Collecting calendrical build information"
    VERBATIM
)
add_dependencies(calendrical calendrical_build_info)
target_sources(calendrical PRIVATE src/version.cpp "${CALENDRICAL_BUILD_INFO_CPP}")
set_source_files_properties(src/version.cpp
    PROPERTIES COMPILE_DEFINITIONS "CALENDRICAL_BUILD_TYPE=\"$<CONFIG>\"")

target_compile_features(calendrical PUBLIC cxx_std_20)

target_link_libraries(calendrical PRIVATE $<BUILD_INTERFACE:calendrical_warning_flags>)

target_include_directories(calendrical
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<BUILD_INTERFACE:${CALENDRICAL_GENERATED_DIR}/inc>
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
install(FILES "${CALENDRICAL_GENERATED_DIR}/inc/version_config.hpp"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/calendrical
)
