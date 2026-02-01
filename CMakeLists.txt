cmake_minimum_required(VERSION 3.10)
project(MyCliApp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

add_executable(my_cli_app main.cpp)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.12.1
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(my_cli_app_test test/main_test.cpp)

target_link_libraries(my_cli_app_test PRIVATE GTest::gtest_main)

add_test(NAME MyCliAppTests COMMAND my_cli_app_test)