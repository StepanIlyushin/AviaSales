cmake_minimum_required(VERSION 3.14)
project(AviaSales)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors -Wall -Wextra -Werror -std=c++20")

set(SOURCES 
    aviasales.cpp
    database.cpp
    datetime.cpp
)

add_executable(aviasales ${SOURCES})

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests tests.cpp database.cpp datetime.cpp)

target_link_libraries(
    tests
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)

add_custom_target(testsall ALL
  DEPENDS tests
  COMMAND tests
)