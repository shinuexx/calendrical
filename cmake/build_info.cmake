# Collects build information (git state, build time) and writes OUTPUT from INPUT.
#
# Runs on every build (see core/CMakeLists.txt), so the commit hash is never stale.
# configure_file only rewrites OUTPUT when its content changes, so an unchanged commit and a
# disabled timestamp (WITH_TIMESTAMP=OFF) cause no recompilation.
#
# Inputs: SOURCE_DIR, INPUT, OUTPUT, VERSION, COMPILER, PLATFORM, WITH_TIMESTAMP, GIT_EXECUTABLE.
# The timestamp honours SOURCE_DATE_EPOCH for reproducible builds.

foreach(var SOURCE_DIR INPUT OUTPUT VERSION)
    if (NOT DEFINED ${var} OR "${${var}}" STREQUAL "")
        message(FATAL_ERROR "build_info.cmake: ${var} is required")
    endif()
endforeach()

# Keeps a value safe inside a C++ string literal.
function(calendrical_escape out value)
    string(REPLACE "\\" "\\\\" value "${value}")
    string(REPLACE "\"" "\\\"" value "${value}")
    set(${out} "${value}" PARENT_SCOPE)
endfunction()

set(commit "unknown")
set(branch "unknown")
set(dirty "false")

if (GIT_EXECUTABLE)
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" rev-parse HEAD
        WORKING_DIRECTORY "${SOURCE_DIR}"
        RESULT_VARIABLE rc OUTPUT_VARIABLE out ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
    if (rc EQUAL 0 AND NOT out STREQUAL "")
        set(commit "${out}")

        execute_process(
            COMMAND "${GIT_EXECUTABLE}" rev-parse --abbrev-ref HEAD
            WORKING_DIRECTORY "${SOURCE_DIR}"
            RESULT_VARIABLE rc OUTPUT_VARIABLE out ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
        if (rc EQUAL 0 AND NOT out STREQUAL "")
            set(branch "${out}")
        endif()

        # Untracked files (such as an in-tree build directory) do not make the build dirty.
        execute_process(
            COMMAND "${GIT_EXECUTABLE}" status --porcelain --untracked-files=no
            WORKING_DIRECTORY "${SOURCE_DIR}"
            RESULT_VARIABLE rc OUTPUT_VARIABLE out ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
        if (rc EQUAL 0 AND NOT out STREQUAL "")
            set(dirty "true")
        endif()
    endif()
endif()

if (WITH_TIMESTAMP)
    string(TIMESTAMP datetime "%Y-%m-%dT%H:%M:%SZ" UTC)
else()
    set(datetime "unknown")
endif()

foreach(var COMPILER PLATFORM)
    if (NOT DEFINED ${var} OR "${${var}}" STREQUAL "")
        set(${var} "unknown")
    endif()
endforeach()

calendrical_escape(CALENDRICAL_VERSION "${VERSION}")
calendrical_escape(CALENDRICAL_BUILD_DATETIME "${datetime}")
calendrical_escape(CALENDRICAL_GIT_COMMIT "${commit}")
calendrical_escape(CALENDRICAL_GIT_BRANCH "${branch}")
set(CALENDRICAL_GIT_DIRTY "${dirty}")
calendrical_escape(CALENDRICAL_COMPILER "${COMPILER}")
calendrical_escape(CALENDRICAL_PLATFORM "${PLATFORM}")

configure_file("${INPUT}" "${OUTPUT}" @ONLY)
