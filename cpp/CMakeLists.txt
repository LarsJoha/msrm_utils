cmake_minimum_required(VERSION 3.16.0)
project(msrm_utils
    LANGUAGES CXX
    )

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wreturn-type")
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
set(CMAKE_DEBUG_POSTFIX _dbg)

find_program(FPING fping)
message(${FPING})
if(NOT FPING)
    message(FATAL_ERROR "fping not found!")
endif()

include(cotire)


set(SOURCES src/conversion.cpp src/files.cpp src/json.cpp src/math.cpp src/network.cpp src/output.cpp src/system.cpp src/benchmarking.cpp src/geometry.cpp)
set(HEADER include/msrm_utils/conversion.hpp include/msrm_utils/files.hpp include/msrm_utils/json.hpp include/msrm_utils/math.hpp
    include/msrm_utils/network.hpp include/msrm_utils/output.hpp include/msrm_utils/system.hpp include/msrm_utils/benchmarking.hpp include/msrm_utils/geometry.hpp)

add_library(msrm_cpp_utils STATIC ${SOURCES})
target_include_directories(msrm_cpp_utils PUBLIC include)
cotire(msrm_cpp_utils)
add_executable(unit_tests src/unit_test.cpp unit_tests/test_conversion.cpp unit_tests/test_network.cpp
    unit_tests/test_files.cpp unit_tests/test_system.cpp unit_tests/test_math.cpp unit_tests/test_benchmarking.cpp unit_tests/test_geometry.cpp)
cotire(unit_tests)
target_link_libraries(unit_tests msrm_cpp_utils boost_system pthread crypto)

set_target_properties(msrm_cpp_utils PROPERTIES PUBLIC_HEADER "${HEADER}")

target_precompile_headers(msrm_cpp_utils PRIVATE ${CMAKE_SOURCE_DIR}/include/commons/common_headers.hpp)

install(TARGETS msrm_cpp_utils CONFIGURATIONS Release DESTINATION /usr/local/lib PUBLIC_HEADER DESTINATION /usr/local/include/msrm_utils)
install(TARGETS msrm_cpp_utils CONFIGURATIONS Debug DESTINATION /usr/local/lib PUBLIC_HEADER DESTINATION /usr/local/include/msrm_utils)
