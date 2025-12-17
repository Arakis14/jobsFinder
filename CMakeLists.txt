cmake_minimum_required(VERSION 3.15)

# Project name
project(JobsFinder LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directory
include_directories(${CMAKE_SOURCE_DIR}/include)

# Gather all source files automatically
file(GLOB SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Create executable
add_executable(${PROJECT_NAME} ${SRC_FILES})