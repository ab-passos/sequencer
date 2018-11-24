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

add_executable(sequencePlatform sequencePlatform.cpp)

#add_executable(problem2 problem2.cpp)
#add_executable(problem3 problem3.cpp)
#add_executable(problem4 problem4.cpp)
#add_executable(problem5 problem5.cpp)
#add_executable(problem6 problem6.cpp)
#add_executable(lambdas-default-capture-modes lambdas-default-capture-modes.cpp)
#add_executable(tagDispatching tagDispatching.cpp)
#add_executable(functionSemantics functionSemantics.cpp)
#add_executable(mockFactory mockFactory.cpp)
#add_executable(testFactory testFactory.cpp)
#target_link_libraries(problem6 ${GTEST_LIBRARIES} pthread)
#target_link_libraries(tagDispatching ${GTEST_LIBRARIES} pthread)
#target_link_libraries(mockFactory ${GTEST_LIBRARIES} ${GMOCK_BOTH_LIBRARIES} pthread)
#target_link_libraries(testFactory ${GTEST_LIBRARIES} ${GMOCK_BOTH_LIBRARIES} pthread)
