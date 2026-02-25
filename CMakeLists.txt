cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0079 NEW)
if (POLICY CMP0167)
  cmake_policy(SET CMP0167 OLD)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(calendrical)

include(CTest)

add_subdirectory(core)
add_subdirectory(cli)
add_subdirectory(tests)

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
      "${CMAKE_SOURCE_DIR}/cli/inc"
      "${CMAKE_SOURCE_DIR}/README.md"
    )

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
