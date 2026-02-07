#ifndef INPUT_PROCESSOR_H
#define INPUT_PROCESSOR_H

#include <istream>
#include <string>

/**
 * @brief Reads user input from standard input stream
 */
class InputProcessor {
public:
    /**
     * @brief Constructs InputProcessor with given input stream
     * @param input Input stream to read from
     */
    explicit InputProcessor(std::istream& input);

    /**
     * @brief Reads one line from input stream
     * @param line Output parameter to store read line
     * @return true if line was successfully read, false on EOF or error
     */
    bool readLine(std::string& line);

private:
    std::istream& input_;
};

#endif
