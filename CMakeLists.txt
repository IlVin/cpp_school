cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)

# DEBUG|RELEASE|RELWITHDEBINFO|MINSIZEREL
# SET (CMAKE_BUILD_TYPE DEBUG)
set (CMAKE_BUILD_TYPE DEBUG)

# Application name
set (PROJECT CppSchool)

# Project name
project (${PROJECT})

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g") # debug mode

######  BOOST  ############
#set (Boost_USE_STATIC_LIBS ON)
#set (Boost_USE_MULTITHREADED ON)
#
#find_package (Boost COMPONENTS unit_test_framework REQUIRED)
#include_directories (${Boost_INCLUDE_DIRS})
###### /BOOST  ############

######  EXECUTABLE  ############
foreach (TEST "test01")
    add_executable ("${PROJECT}_${TEST}.exe" "${PROJECT_SOURCE_DIR}/${TEST}/main.cpp")
endforeach ()
###### /EXECUTABLE  ############






