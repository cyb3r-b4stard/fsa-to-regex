#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "utils.h"


/**
 * @brief Finite State Automaton (FSA) class
 * 
 */
class FSA
{
private:
    std::vector<std::string> states, alphabet, startingStates, acceptingStates;
    std::vector<triple> transitions;

    size_t startingIndex;
public:

    FSA(const std::string& inputFile);

    /**
     * @brief Converts given FSA to Regular Expression
     * 
     * @param outputFile - path to output file
     */
    void toRegex(const std::string& outputFile);

    /**
     * @brief Splits string into set of states
     *
     * @param str - input string
     *
     * @returns set of words
     */
    std::vector<std::string> preprocessStates(const std::string &str);

    /**
     * @brief Splits string into set of transitions
     *
     * @param str - input string
     *
     * @returns set of transitions
     */
    std::vector<triple> preprocessTransitions(const std::string &str);
};