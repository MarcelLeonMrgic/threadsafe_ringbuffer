cmake_minimum_required(VERSION 3.28)
project(threadsafe_ringbuffer)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# Download and make available GoogleTest
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Create the main executable
add_executable(threadsafe_ringbuffer
        ringbuffer.cpp
        ringbuffer.h
        # You can add other source files here if needed
)

# Configure GoogleTest
enable_testing()

# Create the test executable
add_executable(hello_test
        hello_test.cpp
        ringbuffer.cpp  # Include ringbuffer.cpp here
        ringbuffer.h
)

target_link_libraries(hello_test GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(hello_test)
