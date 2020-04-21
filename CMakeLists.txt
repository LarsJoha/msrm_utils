cmake_minimum_required(VERSION 3.16.0)
project(cpp_utils
    LANGUAGES CXX
    )

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wreturn-type")
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

find_program(FPING fping)
message(${FPING})
if(NOT FPING)
    message(FATAL_ERROR "fping not found!")
endif()

include(cotire)

include_directories(include)

set(SOURCES src/conversion.cpp src/files.cpp src/json.cpp src/math.cpp src/network.cpp src/output.cpp src/system.cpp)
set(HEADER include/cpp_utils/conversion.hpp include/cpp_utils/files.hpp include/cpp_utils/json.hpp include/cpp_utils/math.hpp
    include/cpp_utils/network.hpp include/cpp_utils/output.hpp include/cpp_utils/system.hpp)

add_library(msrm_cpp_utils STATIC ${SOURCES})
cotire(msrm_cpp_utils)
add_executable(unit_tests src/unit_test.cpp unit_tests/test_conversion.cpp unit_tests/test_network.cpp unit_tests/test_files.cpp unit_tests/test_system.cpp unit_tests/test_math.cpp)
cotire(unit_tests)
target_link_libraries(unit_tests msrm_cpp_utils boost_system pthread crypto)

set_target_properties(msrm_cpp_utils PROPERTIES PUBLIC_HEADER "${HEADER}")

target_precompile_headers(msrm_cpp_utils PRIVATE ${CMAKE_SOURCE_DIR}/include/commons/common_headers.hpp)

install(TARGETS msrm_cpp_utils DESTINATION /usr/local/lib PUBLIC_HEADER DESTINATION /usr/local/include/msrm_utils)
#target_precompile_headers(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/include/commons/common_headers.hpp)
