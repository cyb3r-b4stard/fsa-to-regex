#include <string>
#include <vector>
#include <stack>

#include "utils.h"

triple::triple(std::string from_, std::string to_, std::string transition_)
    : from(from_)
    , to(to_)
    , transition(transition_)
{}

tensor::tensor(size_t size)
    : matrix(
        std::vector<std::vector<std::vector<std::string>>>(size, 
            std::vector<std::vector<std::string>>(size, std::vector<std::string>(size, "")))
        )
{}

void dfs(const std::vector<std::string> &states, const std::vector<triple> &transitions)
{
    std::vector<bool> visited(states.size(), false);

    visited[0] = true;
    dfsVisit(visited, states[0], states, transitions);

    for (size_t i = 0; i < states.size(); ++i)
    {
        if (!visited[i])
            throw std::logic_error("E2");
    }
}

void dfsVisit(std::vector<bool> &visited, const std::string &state, 
    const std::vector<std::string> &states, const std::vector<triple> &transitions)
{
    for (size_t i = 0; i < transitions.size(); ++i)
    {
        if (transitions[i].from == state)
        {
            for (size_t j = 0; j < states.size(); ++j)
            {
                if (states[j] == transitions[i].to && !visited[j])
                {
                    visited[j] = true;
                    dfsVisit(visited, states[j], states, transitions);
                }
            }
        }
    }
}