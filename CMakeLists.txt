cmake_minimum_required(VERSION 3.20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# The VERSION file is the single source of the library version.
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" CALENDRICAL_VERSION LIMIT_COUNT 1)
string(STRIP "${CALENDRICAL_VERSION}" CALENDRICAL_VERSION)
if (NOT CALENDRICAL_VERSION MATCHES "^[0-9]+\\.[0-9]+\\.[0-9]+$")
  message(FATAL_ERROR "VERSION must contain major.minor.patch, got '${CALENDRICAL_VERSION}'")
endif()
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION")

project(calendrical VERSION ${CALENDRICAL_VERSION} LANGUAGES CXX)

option(CALENDRICAL_BUILD_TIMESTAMP
  "Record the build time in the library (OFF avoids recompiling build info on every build)" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)
include(CTest)

# Compiler warnings interface target
add_library(calendrical_warning_flags INTERFACE)
add_library(calendrical::warning_flags ALIAS calendrical_warning_flags)

if (MSVC)
  target_compile_options(calendrical_warning_flags INTERFACE
    /W4
    /permissive-
    /utf-8
    /Zc:__cplusplus
  )
else()
  target_compile_options(calendrical_warning_flags INTERFACE
    -Wall
    -Wextra
    -Wpedantic
  )
endif()

add_subdirectory(core)
add_subdirectory(cli)
if (BUILD_TESTING)
  add_subdirectory(tests)
endif()

# Package export and installation configuration
install(EXPORT calendricalTargets
  FILE calendricalTargets.cmake
  NAMESPACE calendrical::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/calendrical
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/calendricalConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/calendricalConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/calendricalConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/calendrical
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/calendricalConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/calendricalConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/calendrical
)

option(CALENDRICAL_BUILD_DOCS "Build API documentation with Doxygen" ON)

if (CALENDRICAL_BUILD_DOCS)
  find_package(Doxygen QUIET)
  if (DOXYGEN_FOUND)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_LATEX NO)
    set(DOXYGEN_RECURSIVE YES)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md")

    set(DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/docs")
    set(DOXYGEN_PROJECT_NAME "calendrical")
    set(DOXYGEN_PROJECT_BRIEF "Calendrical calculations library and CLI")
    set(DOXYGEN_INPUT
      "${CMAKE_SOURCE_DIR}/core/inc"
      "${CMAKE_BINARY_DIR}/core/generated/inc"
      "${CMAKE_SOURCE_DIR}/docs/namespaces.dox"
      "${CMAKE_SOURCE_DIR}/README.md"
    )
    string(REPLACE ";" " " DOXYGEN_INPUT_STR "${DOXYGEN_INPUT}")

    configure_file(
      "${CMAKE_SOURCE_DIR}/docs/Doxyfile.in"
      "${CMAKE_BINARY_DIR}/Doxyfile"
      @ONLY
    )

    add_custom_target(docs
      COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_BINARY_DIR}/Doxyfile"
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM
    )
  else()
    message(STATUS "Doxygen not found: target 'docs' will not be available")
  endif()
endif()
