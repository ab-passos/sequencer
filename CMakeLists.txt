cmake_minimum_required(VERSION 3.4.3)
project (SequencePlatform)
find_package(Boost 1.45.0 COMPONENTS filesystem regex)
include_directories(${Boost_INCLUDE_DIRS})
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "-stdlib=libc++ -std=gnu++11")
set(CMAKE_C_COMPILER "gcc-4.2")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories("/usr/local/include")
link_directories("/usr/local/lib")

# Locate GMock
#find_package(GMock REQUIRED)
#include_directories(${GMOCK_INCLUDE_DIRS})
#include_directories("/usr/local/include")
#link_directories("/usr/local/lib")
set(GMOCK_INCLUDE_DIRS /usr/local/include/gmock)

set(GMOCK_BOTH_LIBRARIES /usr/local/lib/libgmock.a /usr/local/lib/libgmock_main.a /usr/local/lib/libgtest.a /usr/local/lib/libgtest_main.a)

set(CMAKE_BUILD_TYPE Release)

add_executable(sequencePlatform.exe sequencePlatform.cpp)
add_executable(sequencerTest.exe sequencerTest.cpp)
target_link_libraries(sequencerTest.exe ${GTEST_LIBRARIES} ${GMOCK_BOTH_LIBRARIES} pthread)
