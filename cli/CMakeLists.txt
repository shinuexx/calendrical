# CMake
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
	add_compile_options(/Zc:__cplusplus)
endif()

include_directories ("../core/inc")
include_directories ("./inc")

add_executable (cli
	"main.cpp"
)

target_link_libraries(cli PRIVATE libcalendrical)
