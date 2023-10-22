# CMake
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
	add_compile_options(/Zc:__cplusplus)
endif()

find_package (Boost 1.80.0 REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

include_directories ("./include")

set(MAIN_SRC src/)

file(GLOB SRCS ${MAIN_SRC}*.cpp)
file(GLOB CALENDARS ${MAIN_SRC}calendars/*.cpp)
file(GLOB DATETYPES ${MAIN_SRC}datetypes/*.cpp)
file(GLOB PROVIDERS ${MAIN_SRC}holiday_providers/*.cpp)

add_executable (generic 
	"./main.cpp" 
	${DATETYPES}
	${CALENDARS}
	${PROVIDERS}
	${SRCS} )

target_link_libraries(generic PRIVATE ${Boost_LIBRARIES})