#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

template <typename State, typename Alphabet>
class StateMachine
{
public:
    void setStates(const std::vector<State> & states)
    {
        this->states = states;
    }

    void setAlphabet(const std::vector<Alphabet> & alphabet)
    {
        this->alphabet = alphabet;
    }

    void setInitial(const State & startState)
    {
        this->startState = startState;
    }

    void setFinal(const State & final)
    {
        finalState = final;
    }

    void setTransitions(const std::map<std::pair<State, Alphabet>, State> & transitions)
    {
        this->transitions = transitions;
    }

    bool checkString(const std::vector<Alphabet> & input) const
    {
        State currentState = startState;

        for (const auto & symbol : input)
        {
            auto transition = transitions.find({currentState, symbol});

            if (transition != transitions.end())
            {
                currentState = transition->second;
            }
            else
            {
                return false;
            }
        }

        return currentState == finalState;
    }

    bool checkSubstring(const std::string & input, const std::string & substring) const
    {
        std::vector<Alphabet> inputSymbols(input.begin(), input.end());
        for (size_t i = 0; i <= inputSymbols.size() - substring.length(); ++i)
        {
            if (checkString(
                    std::vector<Alphabet>(inputSymbols.begin() + i, inputSymbols.begin() + i + substring.length())))
            {
                return true;
            }
        }

        return false;
    }

private:
    std::vector<State> states;
    std::vector<Alphabet> alphabet;
    State startState;
    State finalState;
    std::map<std::pair<State, Alphabet>, State> transitions;
};
