#pragma once

#include <string>
#include <vector>
#include <stdexcept>

/**
 * @brief Simple triplet structure to store FSA transitions
 * 
 */
struct triple
{
    std::string from, to, transition;
    triple(std::string from_, std::string to_, std::string transition_);
};


/**
 * @brief 3D matrix structure
 * 
 */
struct tensor
{
    std::vector<std::vector<std::vector<std::string>>> matrix;

    tensor(size_t size);
};


/**
 * @brief Performs Depth First Search
 *
 * @param states      - reference to set of states
 * @param transitions - reference to set of transitions
 *
 * @throws std::logic_error thrown if FSA is not a connected graph
 */
void dfs(const std::vector<std::string> &states, const std::vector<triple> &transitions);


/**
 * @brief Helper function for DFS
 *
 * @relatesalso dfs
 *
 * @param visited     - reference to set of boolean value
 * @param state       - current state
 * @param states      - reference to set of states
 * @param transitions - reference to set of transitions
 *
 */
void dfsVisit(std::vector<bool> &visited, const std::string &state, 
    const std::vector<std::string> &states, const std::vector<triple> &transitions);