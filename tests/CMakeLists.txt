# CMake
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
	add_compile_options(/Zc:__cplusplus)
endif()

include_directories ("../core/inc")
include_directories ("../cli/inc")
include_directories ("./inc")

set(MAIN_SRC ../core/src/)
set(TESTS_SRC src/)

file(GLOB CALENDARS_TESTS ${TESTS_SRC}calendars/*.cc)
file(GLOB HOLIDAYS_TESTS ${TESTS_SRC}holidays/*.cc)
file(GLOB HINDU_ASTRONOMY_TESTS ${TESTS_SRC}hindu_astronomy/*.cc)
file(GLOB TIME_AND_ASTRONOMY_TESTS ${TESTS_SRC}time_and_astronomy/*.cc)
file(GLOB BASICS_TESTS ${TESTS_SRC}*.cc)

add_executable (tests 
	${CALENDARS_TESTS}
	${HOLIDAYS_TESTS}
	${TIME_AND_ASTRONOMY_TESTS}
	${HINDU_ASTRONOMY_TESTS}
	${BASICS_TESTS}
)

target_link_libraries(tests PRIVATE libcalendrical calendrical_cli_lib)

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
	FetchContent_MakeAvailable(googletest)
endif()

target_link_libraries(tests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

add_test(calendrical_tests tests)
