# Minimal required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(RingBufferProject VERSION 1.0)

# Set the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add the executable for the project
add_executable(ringbuffer_test ringbuffer.c)

# Include header files
target_include_directories(ringbuffer_test PUBLIC "${PROJECT_SOURCE_DIR}")
