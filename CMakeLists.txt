cmake_minimum_required(VERSION 3.28)
project(untitled1)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# Herunterladen und Bereitstellen von GoogleTest
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Erstellen des Hauptprogramms
add_executable(untitled1
        ringbuffer.cpp
        ringbuffer.h
        hello_test.cpp
)

# Konfiguration für GoogleTest
enable_testing()

add_executable(hello_test hello_test.cpp)
target_link_libraries(hello_test GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(hello_test)
