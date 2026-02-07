#include "input_processor.h"

InputProcessor::InputProcessor(std::istream& input) : input_(input) {}

bool InputProcessor::readLine(std::string& line) {
    return static_cast<bool>(std::getline(input_, line));
}
