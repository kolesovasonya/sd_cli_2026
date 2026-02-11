cmake_minimum_required(VERSION 3.14)
project(cli_interpreter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES
    src/main.cpp
    src/input_processor.cpp
    src/lexer.cpp
    src/parser.cpp
    src/environment_manager.cpp
    src/command_factory.cpp
    src/command_executor.cpp
    src/process_manager.cpp
    src/commands/cat_command.cpp
    src/commands/wc_command.cpp
    src/commands/echo_command.cpp
    src/commands/pwd_command.cpp
    src/commands/exit_command.cpp
    src/commands/external_command.cpp
)

add_executable(cli_app ${SOURCES})

if(WIN32)
    target_compile_definitions(cli_app PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

enable_testing()

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

set(TEST_SOURCES
    test/test_main.cpp
    test/test_lexer.cpp
    test/test_parser.cpp
    test/test_commands.cpp
    test/test_quotes.cpp
    test/test_environment.cpp
    src/input_processor.cpp
    src/lexer.cpp
    src/parser.cpp
    src/environment_manager.cpp
    src/command_factory.cpp
    src/command_executor.cpp
    src/process_manager.cpp
    src/commands/cat_command.cpp
    src/commands/wc_command.cpp
    src/commands/echo_command.cpp
    src/commands/pwd_command.cpp
    src/commands/exit_command.cpp
    src/commands/external_command.cpp
)

add_executable(cli_tests ${TEST_SOURCES})
target_link_libraries(cli_tests gtest gtest_main)

add_test(NAME cli_tests COMMAND cli_tests)
