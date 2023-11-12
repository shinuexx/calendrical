# CMake
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
	add_compile_options(/Zc:__cplusplus)
endif()

find_package (Boost 1.80.0 REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

include_directories ("../calendrical/include")
include_directories ("./inc")

set(MAIN_SRC ../calendrical/src/)
set(TESTS_SRC src/)

file(GLOB SRCS ${MAIN_SRC}*.cpp)
file(GLOB CALENDARS ${MAIN_SRC}calendars/*.cpp)
file(GLOB DATETYPES ${MAIN_SRC}datetypes/*.cpp)
file(GLOB PROVIDERS ${MAIN_SRC}holiday_providers/*.cpp)

file(GLOB CALENDARS_TESTS ${TESTS_SRC}calendars/*.cc)
file(GLOB BASICS_TESTS ${TESTS_SRC}*.cc)

add_executable (tests 
	"main.cc"
	${DATETYPES}
	${CALENDARS}
	${PROVIDERS}
	${SRCS}
	${CALENDARS_TESTS}
	${BASICS_TESTS}
)

target_link_libraries(tests PRIVATE ${Boost_LIBRARIES})

enable_testing()

find_package(GTest CONFIG REQUIRED)
target_link_libraries(tests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

add_test(calendrical_tests tests)