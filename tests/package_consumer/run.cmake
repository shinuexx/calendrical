# Installs the calendrical build tree into WORK_DIR/prefix, then configures, builds and runs the
# consumer project in SOURCE_DIR against it. Invoked by the package_consumer CTest test.
foreach(var BUILD_DIR SOURCE_DIR WORK_DIR GENERATOR CXX_COMPILER)
    if (NOT ${var})
        message(FATAL_ERROR "run.cmake: ${var} is required")
    endif()
endforeach()

set(prefix "${WORK_DIR}/prefix")
set(consumer_build "${WORK_DIR}/build")
file(REMOVE_RECURSE "${WORK_DIR}")

set(config_args "")
set(build_type_arg "")
if (CONFIG)
    set(config_args --config "${CONFIG}")
    set(build_type_arg "-DCMAKE_BUILD_TYPE=${CONFIG}")
endif()
set(platform_args "")
if (GENERATOR_PLATFORM)
    set(platform_args -A "${GENERATOR_PLATFORM}")
endif()

execute_process(
    COMMAND "${CMAKE_COMMAND}" --install "${BUILD_DIR}" --prefix "${prefix}" ${config_args}
    COMMAND_ERROR_IS_FATAL ANY)

execute_process(
    COMMAND "${CMAKE_COMMAND}" -S "${SOURCE_DIR}" -B "${consumer_build}"
        -G "${GENERATOR}" ${platform_args}
        "-DCMAKE_PREFIX_PATH=${prefix}"
        "-DCMAKE_CXX_COMPILER=${CXX_COMPILER}"
        "-DCMAKE_CXX_FLAGS=${CXX_FLAGS}"
        ${build_type_arg}
    COMMAND_ERROR_IS_FATAL ANY)

execute_process(
    COMMAND "${CMAKE_COMMAND}" --build "${consumer_build}" ${config_args}
    COMMAND_ERROR_IS_FATAL ANY)

find_program(consumer_exe NAMES consumer
    PATHS "${consumer_build}" "${consumer_build}/${CONFIG}"
    NO_DEFAULT_PATH REQUIRED)

execute_process(COMMAND "${consumer_exe}" COMMAND_ERROR_IS_FATAL ANY)
