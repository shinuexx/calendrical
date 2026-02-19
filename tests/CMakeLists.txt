# CMake
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
	add_compile_options(/Zc:__cplusplus)
endif()

include_directories ("../core/inc")
include_directories ("./inc")

set(MAIN_SRC ../core/src/)
set(TESTS_SRC src/)

file(GLOB CALENDARS_TESTS ${TESTS_SRC}calendars/*.cc)
file(GLOB HOLIDAYS_TESTS ${TESTS_SRC}holidays/*.cc)
file(GLOB HINDU_ASTRONOMY_TESTS ${TESTS_SRC}hindu_astronomy/*.cc)
file(GLOB TIME_AND_ASTRONOMY_TESTS ${TESTS_SRC}time_and_astronomy/*.cc)
file(GLOB BASICS_TESTS ${TESTS_SRC}*.cc)

add_executable (tests 
	"main.cc"
	${CALENDARS_TESTS}
	${HOLIDAYS_TESTS}
	${TIME_AND_ASTRONOMY_TESTS}
	${HINDU_ASTRONOMY_TESTS}
	${BASICS_TESTS}
)

target_link_libraries(tests PRIVATE libcalendrical)

enable_testing()

find_package(GTest REQUIRED CONFIG)

target_link_libraries(tests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

add_test(calendrical_tests tests)