# CMake
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
	add_compile_options(/Zc:__cplusplus)
endif()

find_package (Boost 1.80.0 REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

include_directories ("../core/inc")
include_directories ("./inc")

add_executable (cli
	"main.cpp"
)

target_link_libraries(cli PRIVATE ${Boost_LIBRARIES})
target_link_libraries(cli PRIVATE libcalendrical)
