#include "commands/grep_command.h"

#include <cxxopts.hpp>
#include <fstream>
#include <regex>
#include <vector>

GrepCommand::GrepCommand(std::vector<std::string> args)
    : args_(std::move(args)) {}

int GrepCommand::execute(std::istream& input, std::ostream& output,
                         std::ostream& error) {
    cxxopts::Options options("grep", "Search for patterns in files");

    // clang-format off
    options.add_options()
        ("w,word-regexp",    "Match only whole words",
            cxxopts::value<bool>()->default_value("false"))
        ("i,ignore-case",    "Case-insensitive search",
            cxxopts::value<bool>()->default_value("false"))
        ("A,after-context",  "Print N lines after each matching line",
            cxxopts::value<int>()->default_value("0"))
        ("pattern", "Regular expression to search for",
            cxxopts::value<std::string>())
        ("file",    "File to search (reads from stdin if omitted)",
            cxxopts::value<std::string>()->default_value(""));
    // clang-format on

    options.parse_positional({"pattern", "file"});

    std::vector<std::string> argv_strs;
    argv_strs.push_back("grep");
    argv_strs.insert(argv_strs.end(), args_.begin(), args_.end());

    std::vector<const char*> argv_ptrs;
    argv_ptrs.reserve(argv_strs.size());
    for (const auto& s : argv_strs) {
        argv_ptrs.push_back(s.c_str());
    }

    int argc = static_cast<int>(argv_ptrs.size());

    cxxopts::ParseResult result;
    try {
        result = options.parse(argc, argv_ptrs.data());
    } catch (const std::exception& e) {
        error << "grep: " << e.what() << std::endl;
        return 1;
    }

    if (!result.count("pattern")) {
        error << "grep: missing pattern" << std::endl;
        return 1;
    }

    const bool whole_word = result["w"].as<bool>();
    const bool ignore_case = result["i"].as<bool>();
    const int after_ctx = result["A"].as<int>();
    std::string pattern = result["pattern"].as<std::string>();
    const std::string file = result["file"].as<std::string>();

    if (after_ctx < 0) {
        error << "grep: -A value must be non-negative" << std::endl;
        return 1;
    }

    if (whole_word) {
        pattern = "\\b(?:" + pattern + ")\\b";
    }

    auto flags = std::regex_constants::ECMAScript;
    if (ignore_case) {
        flags |= std::regex_constants::icase;
    }

    std::regex re;
    try {
        re = std::regex(pattern, flags);
    } catch (const std::regex_error& e) {
        error << "grep: invalid pattern: " << e.what() << std::endl;
        return 1;
    }

    std::istream* src = &input;
    std::ifstream file_stream;
    if (!file.empty() && file != "-") {
        file_stream.open(file);
        if (!file_stream.is_open()) {
            error << "grep: " << file << ": No such file or directory"
                  << std::endl;
            return 1;
        }
        src = &file_stream;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(*src, line)) {
        lines.push_back(line);
    }

    std::vector<bool> should_print(lines.size(), false);
    bool found_any = false;

    for (size_t i = 0; i < lines.size(); i++) {
        if (std::regex_search(lines[i], re)) {
            found_any = true;
            size_t end =
                std::min(i + static_cast<size_t>(after_ctx), lines.size() - 1);
            for (size_t j = i; j <= end; j++) {
                should_print[j] = true;
            }
        }
    }

    printWithSeparators(lines, should_print, after_ctx, output);

    return found_any ? 0 : 1;
}

void GrepCommand::printWithSeparators(const std::vector<std::string>& lines,
                                      const std::vector<bool>& should_print,
                                      int after_context,
                                      std::ostream& output) const {
    bool prev_printed = false;
    bool need_sep = false;

    for (size_t i = 0; i < lines.size(); i++) {
        if (should_print[i]) {
            if (need_sep && after_context > 0) {
                output << "--\n";
            }
            need_sep = false;
            output << lines[i] << "\n";
            prev_printed = true;
        } else if (prev_printed) {
            need_sep = true;
            prev_printed = false;
        }
    }
}
