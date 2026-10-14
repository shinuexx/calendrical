set(TESTS_SRC src/)

file(GLOB CALENDARS_TESTS CONFIGURE_DEPENDS ${TESTS_SRC}calendars/*.cc)
file(GLOB HOLIDAYS_TESTS CONFIGURE_DEPENDS ${TESTS_SRC}holidays/*.cc)
file(GLOB HINDU_ASTRONOMY_TESTS CONFIGURE_DEPENDS ${TESTS_SRC}hindu_astronomy/*.cc)
file(GLOB TIME_AND_ASTRONOMY_TESTS CONFIGURE_DEPENDS ${TESTS_SRC}time_and_astronomy/*.cc)
file(GLOB BASICS_TESTS CONFIGURE_DEPENDS ${TESTS_SRC}*.cc)

add_executable(tests
    ${CALENDARS_TESTS}
    ${HOLIDAYS_TESTS}
    ${TIME_AND_ASTRONOMY_TESTS}
    ${HINDU_ASTRONOMY_TESTS}
    ${BASICS_TESTS}
)

target_compile_features(tests PRIVATE cxx_std_20)

target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(tests
    PRIVATE
        calendrical
        calendrical_cli_lib
        calendrical_warning_flags
)

target_compile_definitions(tests PRIVATE
    CALENDRICAL_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/.."
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/../all-holidays.csv"
    "${CMAKE_CURRENT_BINARY_DIR}/all-holidays.csv"
    COPYONLY
)

enable_testing()

find_package(GTest CONFIG QUIET)
if (NOT GTest_FOUND)
    find_package(GTest MODULE QUIET)
endif()

if (NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    # Keep parent project compiler/linker settings on Windows.
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    set(INSTALL_GMOCK OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

target_link_libraries(tests
    PRIVATE
        GTest::gtest
        GTest::gtest_main
        GTest::gmock
        GTest::gmock_main
)

add_test(NAME calendrical_tests COMMAND tests)
