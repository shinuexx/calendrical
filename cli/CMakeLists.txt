# CMake
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
	add_compile_options(/Zc:__cplusplus)
endif()

include_directories ("../core/inc")
include_directories ("./inc")

add_executable (calendrical_cli
	"main.cpp"
	"src/commands/command_dispatcher.cpp"
	"src/commands/convert_command.cpp"
	"src/commands/holidays_command.cpp"
	"src/commands/list_command.cpp"
	"src/converter_provider.cpp"
	"src/converter_provider_builder.cpp"
	"src/holiday_provider_provider.cpp"
	"src/holiday_provider_provider_builder.cpp"
)

target_link_libraries(calendrical_cli PRIVATE libcalendrical)
