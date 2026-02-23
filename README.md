# C++ Command Line Interpreter (CLI)

## Project Overview

This project implements a simple Command Line Interpreter (CLI) in C++. The goal is to create a functional shell capable of executing basic commands, handling environment variables, managing I/O through pipelines, and interacting with external system programs.

## Key Features

The interpreter supports the following functionalities:

*   **Built-in Commands**:
    *   `cat [FILE]`: Displays the content of a file or stdin.
    *   `wc [FILE]`: Counts lines, words, and bytes in a file or stdin.
    *   `echo [ARGS]`: Prints arguments to the console.
    *   `pwd`: Prints the current working directory.
    *   `exit`: Terminates the interpreter.
    *   `grep [OPTIONS] PATTERN [FILE]`: Searches for lines matching a regular expression. Supported options:
        *   `-w` — match only whole words (bounded by non-word characters);
        *   `-i` — case-insensitive search;
        *   `-A N` — also print the N lines following each matching line; overlapping or adjacent context windows are merged, and non-adjacent groups are separated by `--`.
*   **Environment Variable Management**: Support for setting, modifying, and using environment variables (e.g., `NAME=value`, `echo $NAME`). Special variable `$?` contains the exit code of the last executed command.
*   **Quoting**: Handling of single (`'`) and double (`"`) quotes to escape special characters and define string literals.
*   **External Program Execution**: Automatic launch of any external executable program if the command is not a built-in one (e.g., `git status`).
*   **Pipelining**: Redirecting the output of one command to the input of another using the `|` operator (e.g., `cat file.txt | wc`, `echo hello | cat`). All commands in a pipeline run in parallel in separate processes. Returns the exit code of the last command. 
*   **Input/Output Stream Handling**: Flexible management of standard input, output, and error streams for commands.
*   **Exit Codes**: Capturing and respecting command exit codes to determine their execution status.

## Dependencies

The following libraries are fetched automatically at configure time via CMake `FetchContent` — no manual installation is required.

| Library | Version | Purpose |
|---------|---------|---------|
| [GoogleTest](https://github.com/google/googletest) | 1.12.1 | Unit testing framework |
| [cxxopts](https://github.com/jarro2783/cxxopts) | 3.2.0 | Command-line argument parsing for `grep` |

### Why cxxopts?

`grep` has a non-trivial set of flags that interact with each other, and
implementing a hand-rolled parser for them would be error-prone and hard to
extend. The task explicitly requires using an argument-parsing library.

Candidates considered:

* **getopt / getopt\_long** — POSIX-only, C-style API. Doesn't fit the
  C++17 codebase and is unavailable on Windows.
* **Boost.Program\_options** — full-featured, but Boost is a heavy
  dependency just for parsing a handful of flags.
* **CLI11** — excellent C++17 library; comparable to cxxopts in
  functionality, but slightly more verbose for simple use cases.
* **cxxopts** — header-only, MIT-licensed, modern C++17 API, integrates
  via a single `FetchContent_Declare` (consistent with how GoogleTest is
  already added). Its fluent builder API maps naturally to the small set
  of grep options, and its positional-argument support handles
  `pattern [file]` without extra boilerplate.

## Build Requirements

- CMake 3.14 or higher
- C++17 compatible compiler (GCC, Clang, MSVC)
- Make (optional, can use CMake directly)
- Internet connection on first build (FetchContent downloads GoogleTest and cxxopts)

## Build Instructions

### Using Make (Linux/macOS/Windows with Make)

```bash
make cli_app
```

### Using CMake directly

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Running

## Linux/macOS

```bash
./cli_app
```

## Windows

```bash
cli_app.exe
```

## Running Tests

### Using Make

```bash
make test
```

### Using CMake directly

```bash
cd build
ctest --output-on-failure
```

##

Higher School of Economics, 2026