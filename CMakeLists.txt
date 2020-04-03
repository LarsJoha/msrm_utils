cmake_minimum_required(VERSION 3.10.2)
project(cpp_utils
    LANGUAGES CXX
    )

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_LINK_DEPENDS_NO_SHARED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wreturn-type")

include_directories(include)

set(SOURCES src/conversion.cpp src/files.cpp src/json.cpp src/math.cpp src/network.cpp src/output.cpp src/system.cpp)

add_library(msrm_cpp_utils STATIC ${SOURCES})
add_executable(unit_tests src/unit_test.cpp src/conversion.cpp src/files.cpp)

target_precompile_headers(msrm_cpp_utils PRIVATE ${CMAKE_SOURCE_DIR}/include/commons/common_headers.hpp)
#target_precompile_headers(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/include/commons/common_headers.hpp)
