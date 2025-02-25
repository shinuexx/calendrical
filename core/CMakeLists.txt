# CMake
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
	add_compile_options(/Zc:__cplusplus)
	add_compile_options(/utf-8)
endif()

find_package (Boost 1.80.0 REQUIRED)

message("boost lib: ${Boost_LIBRARIES}")
message("boost inc: ${Boost_INCLUDE_DIR}")

include_directories (${Boost_INCLUDE_DIR})

include_directories ("./inc")

set(MAIN_SRC src/)
set(LIB_NAME libcalendrical)

file(GLOB_RECURSE SRCS ${MAIN_SRC}*.cpp)

add_library(${LIB_NAME}
	${SRCS})

target_link_libraries(${LIB_NAME} PRIVATE ${Boost_LIBRARIES})
