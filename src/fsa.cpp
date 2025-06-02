#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <iostream>

#include "utils.h"
#include "fsa.h"

FSA::FSA(const std::string& inputFile)
{
    std::ifstream input(inputFile);

    std::string states_, alphabet_, startingStates_, acceptingStates_, transitions_, buffer;

    getline(input, states_);
    getline(input, alphabet_);
    getline(input, startingStates_);
    getline(input, acceptingStates_);
    getline(input, transitions_);

    /* Check for excess lines in input file */
    try 
    {
        if (getline(input, buffer))
            throw std::logic_error("E0");
    }
    catch (const std::exception &e) 
    {
        input.close();
        throw;
    }

    acceptingStates = preprocessStates(acceptingStates_);
    startingStates  = preprocessStates(startingStates_);
    states          = preprocessStates(states_);
    alphabet        = preprocessStates(alphabet_);
    transitions     = preprocessTransitions(transitions_);

}

void FSA::toRegex(const std::string& outputFile)
{
    std::ofstream output(outputFile);
    std::string result;
    tensor regex(states.size());
    std::vector<std::vector<std::string>> regexInitial(states.size(), std::vector<std::string>(states.size(), ""));

    /* Check connectivity of FSA */
    try
    {
        dfs(states, transitions);
    }
    catch (std::logic_error &error)
    {
        throw;
    }

    for (size_t i = 0; i < transitions.size(); ++i)
    {
        bool validTransition = false;

        /* Check for edges with same transitions from state */
        for (size_t j = i + 1; j < transitions.size(); ++j)
        {
            if (transitions[i].from == transitions[j].from &&
                transitions[i].transition == transitions[j].transition)
            {
                throw std::logic_error("E5");
            }
        }

        /* Check if transitions are present in the alphabet */
        for (size_t j = 0; j < alphabet.size(); ++j)
        {
            if (transitions[i].transition == alphabet[j])
                validTransition = true;
        }

        if (!validTransition)
            throw std::logic_error("E3" + transitions[i].transition);
    }

    /* Check that every state from transition table is present in the set of states */
    for (size_t i = 0; i < transitions.size(); ++i)
    {
        if (!std::any_of(states.begin(), states.end(), [&](const auto &x){ return x == transitions[i].from; }))
        {
            throw std::logic_error("E1" + transitions[i].from);
        }
        else if (!std::any_of(states.begin(), states.end(), [&](const auto &x){ return x == transitions[i].to; }))
        {
            throw std::logic_error("E1" + transitions[i].to);
        }
    }

    /* Check that every state from accepting set is present in the set of states*/
    for (size_t i = 0; i < acceptingStates.size(); ++i)
    {
        if (!std::any_of(states.begin(), states.end(), [&](const auto &x){ return x == acceptingStates[i]; }))
        {
            throw std::logic_error("E1" + acceptingStates[i]);
        }
    }

    /* Check that starting state is defined*/
    if (startingStates.empty())
    {
        throw std::logic_error("E4");
    }
    else if (!std::any_of(states.begin(), states.end(), [&](const auto &x){ return x == startingStates[0]; }))
    {
        throw std::logic_error("E1" + startingStates[0]);
    }

    /* Set up initial values of regex for k = -1 */
    for (size_t i = 0; i < transitions.size(); ++i)
    {
        size_t from, to;

        for (size_t j = 0; j < states.size(); ++j)
        {
            if (states[j] == transitions[i].from)
            {
                from = j;
            }
        }

        for (size_t j = 0; j < states.size(); ++j)
        {
            if (states[j] == transitions[i].to)
            {
                to = j;
            }
        }

        if (!regexInitial[from][to].empty())
        {
            regexInitial[from][to] += "|" + transitions[i].transition;
        }
        else
        {
            regexInitial[from][to] = transitions[i].transition;
        }
    }

    for (size_t i = 0; i < states.size(); ++i)
    {
        if (regexInitial[i][i].empty())
        {
            regexInitial[i][i] = "eps";
        }
        else
        {
            regexInitial[i][i] += "|eps";
        }

        for (size_t j = 0; j < states.size(); ++j)
        {
            if (i != j && regexInitial[i][j].empty())
            {
                regexInitial[i][j] = "{}";
            }
        }
    }

    /* Calculate regex */
    for (size_t k = 0; k < states.size(); ++k)
    {
        for (size_t i = 0; i < states.size(); ++i)
        {
            for (size_t j = 0; j < states.size(); ++j)
            {
                if (k > 0)
                {
                    regex.matrix[i][j][k] = "(" + regex.matrix[i][k][k - 1] + ")(" +
                                            regex.matrix[k][k][k - 1] + ")*(" +
                                            regex.matrix[k][j][k - 1] + ")|(" +
                                            regex.matrix[i][j][k - 1] + ")";
                }
                else
                {
                    regex.matrix[i][j][k] = "(" + regexInitial[i][k] + ")(" +
                                            regexInitial[k][k] + ")*(" +
                                            regexInitial[k][j] + ")|(" +
                                            regexInitial[i][j] + ")";
                }
            }
        }
    }

    /* Find index of starting state in set of states*/
    for (size_t i = 0; i < states.size(); ++i)
    {
        if (states[i] == startingStates[0])
        {
            startingIndex = i;
            break;
        }
    }

    /* Translate FSA to Regular Expression */
    for (size_t i = 0; i < acceptingStates.size(); ++i)
    {
        size_t index;

        for (size_t j = 0; j < states.size(); ++j)
        {
            if (states[j] == acceptingStates[i])
            {
                index = j;
            }
        }

        if (result.empty())
        {
            result = regex.matrix[startingIndex][index][states.size() - 1];
        }
        else
        {
            result += "|" + regex.matrix[startingIndex][index][states.size() - 1];
        }
    }

    if (result.empty())
    {
        result = "{}";
    }

    output << result;

    output.close();
}

std::vector<std::string> FSA::preprocessStates(const std::string &str)
{
    std::vector<std::string> processed;
    size_t left(0), right(0);

    while (left < str.size() && str[left] != '[')
    {
        left++;
        right++;
    }

    left++;
    right++;

    while (right < str.size()) 
    {
        if (str[right] == ',' || str[right] == ']' && left < right) 
        {
            processed.push_back(str.substr(left, right - left));
            left = right + 1;
        }
        right++;
    }

    return processed;
}

std::vector<triple> FSA::preprocessTransitions(const std::string &str)
{
    std::vector<std::string> transitionsSplitted(preprocessStates(str));
    std::vector<triple>      processed;

    for (const std::string &transition: transitionsSplitted)
    {
        size_t first(transition.find(">")), last(transition.rfind(">"));

        std::string from(transition.substr(0, first)),
                    edgeLabel(transition.substr(first + 1, last - first - 1)),
                    to(transition.substr(last + 1));

        processed.emplace_back(from, to, edgeLabel);
    }

    return processed;
}