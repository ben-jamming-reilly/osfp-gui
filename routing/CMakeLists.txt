cmake_minimum_required(VERSION 2.8.12)

project(ospf_tests)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/build) 

# Download rapidjson for parsing and creating JSON objects
# for communicating with the node application
include("${CMAKE_SOURCE_DIR}/CMakeLists.txt.rapidjson")
include_directories("${CMAKE_BINARY_DIR}/build/rapidjson-prefix/src/rapidjson/include")

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.gtest build/googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/build/googletest-download"
)
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/build/googletest-download"
)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds the following targets:
# gtest, gtest_main, gmock and gmock_main
add_subdirectory("${CMAKE_BINARY_DIR}/build/googletest-src"
                 "${CMAKE_BINARY_DIR}/build/googletest-build"
)

# The gtest/gmock targets carry header search path dependencies
# automatically when using CMake 2.8.11 or later. Otherwise we
# have to add them here ourselves.
if(CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
                        "${gmock_SOURCE_DIR}/include"
    )
endif()

enable_testing()

# output executable in routing/ directory
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR})

# build and link executable
add_executable(ospf_test.out ospf_test.cpp ospf.cpp)
add_dependencies(ospf_test.out rapidjson)
target_link_libraries(ospf_test.out gtest gtest_main)
include(GoogleTest)
gtest_discover_tests(ospf_test.out)