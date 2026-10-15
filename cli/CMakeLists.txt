add_library(calendrical_cli_lib STATIC
    src/commands/command_dispatcher.cpp
    src/commands/convert_command.cpp
    src/commands/holidays_command.cpp
    src/commands/interactive_command.cpp
    src/commands/list_command.cpp
    src/converter_provider.cpp
    src/converter_provider_builder.cpp
    src/holiday_provider_provider.cpp
    src/holiday_provider_provider_builder.cpp
)

target_compile_features(calendrical_cli_lib PUBLIC cxx_std_20)

target_include_directories(calendrical_cli_lib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
)

target_link_libraries(calendrical_cli_lib
    PUBLIC
        calendrical
    PRIVATE
        calendrical_warning_flags
)

add_executable(calendrical_cli main.cpp)
target_compile_features(calendrical_cli PRIVATE cxx_std_20)
target_link_libraries(calendrical_cli
    PRIVATE
        calendrical_cli_lib
        calendrical_warning_flags
)

install(TARGETS calendrical_cli
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
