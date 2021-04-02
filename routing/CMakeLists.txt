cmake_minimum_required(VERSION 3.14)
project(ospf_tests)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

# fetch most recent version of GTest
include(FetchContent)
FetchContent_Declare(
	gtest
	URL	https://github.com/google/googletest/archive/6c5c455.zip
)

# For Windows: prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest)

enable_testing()

# output executable in routing/ directory
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR})

# build and link executable
add_executable(ospf_test.out ospf_test.cpp ospf.cpp)
target_link_libraries(ospf_test.out gtest gtest_main)
include(GoogleTest)
gtest_discover_tests(ospf_test.out)
