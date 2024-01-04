#include "StateMachine.h"

using namespace std;

int main()
{
    StateMachine<int, char> automatonIntChar;
    int alphabetSize;
    cout << "Enter size of the alphabet: ";
    cin >> alphabetSize;

    if (alphabetSize <= 0)
    {
        cout << "Alphabet is invalid!" << endl;
        return 1;
    }

    vector<char> alphabetChar;
    cout << "Enter the alphabet characters (separate with spaces, max " << alphabetSize << "): ";

    for (size_t i = 0; i < alphabetSize; ++i)
    {
        char symbol;
        cin >> symbol;
        alphabetChar.push_back(symbol);
    }

    automatonIntChar.setStates({0, 1});
    automatonIntChar.setAlphabet(alphabetChar);
    automatonIntChar.setInitial(0);
    automatonIntChar.setFinal(0);

    map<pair<int, char>, int> transitionsIntChar;
    transitionsIntChar[{0, alphabetChar[0]}] = 1;
    transitionsIntChar[{1, alphabetChar[0]}] = 0;

    for (size_t i = 1; i < alphabetSize; ++i)
    {
        transitionsIntChar[{0, alphabetChar[i]}] = 0;
        transitionsIntChar[{1, alphabetChar[i]}] = 1;
    }

    automatonIntChar.setTransitions(transitionsIntChar);

    int stringLength;
    cout << "Enter the length of the string: ";
    cin >> stringLength;

    vector<char> inputChar;
    cout << "Enter the string to check (length " << stringLength << "): ";
    char symbolChar;
    for (size_t i = 0; i < stringLength; ++i)
    {
        cin >> symbolChar;
        inputChar.push_back(symbolChar);
    }

    if (automatonIntChar.checkString(inputChar))
    {
        cout << "String is accepted (amount of " << alphabetChar[0] << " is even)" << std::endl;
    }
    else
    {
        cout << "String is not accepted (amount of " << alphabetChar[0] << " is odd).\n" << std::endl;
    }

    cout << "\n\nFiniteStateMachine N1.2 (This automaton accepts a int string if "
            "the first symbol in the alphabet meets an even number of times.)"
         << endl;
    StateMachine<int, int> automatonIntInt;

    int alphabetSizeInt;
    cout << "Enter the size of the alphabet: ";
    cin >> alphabetSizeInt;

    if (alphabetSizeInt <= 0)
    {
        cout << "Invalid alphabet size. Exiting...\n";
        return 1;
    }

    vector<int> alphabetNumbers;
    cout << "Enter the alphabet characters (separate with spaces, max " << alphabetSizeInt << "): ";
    for (size_t i = 0; i < alphabetSizeInt; ++i)
    {
        int symbol;
        cin >> symbol;
        alphabetNumbers.push_back(symbol);
    }

    automatonIntInt.setStates({0, 1});
    automatonIntInt.setAlphabet(alphabetNumbers);
    automatonIntInt.setInitial(0);
    automatonIntInt.setFinal(0);

    map<pair<int, int>, int> transitionsIntInt;
    transitionsIntInt[{0, alphabetNumbers[0]}] = 1;
    transitionsIntInt[{1, alphabetNumbers[0]}] = 0;

    for (size_t i = 1; i < alphabetSizeInt; ++i)
    {
        transitionsIntInt[{0, alphabetNumbers[i]}] = 0;
        transitionsIntInt[{1, alphabetNumbers[i]}] = 1;
    }

    automatonIntInt.setTransitions(transitionsIntInt);

    int IntstringLength;
    cout << "Enter the length of the string: ";
    cin >> IntstringLength;

    // Validate FiniteAutomaton<int, int>
    vector<int> inputInt;
    cout << "Enter the string to check (length " << IntstringLength << "): (after each symbol push space)";
    int symbolInt;
    for (size_t i = 0; i < IntstringLength; ++i)
    {
        cin >> symbolInt;
        inputInt.push_back(symbolInt);
    }

    if (automatonIntInt.checkString(inputInt))
    {
        cout << "String is accepted (amount of " << alphabetNumbers[0] << " is even).\n";
    }
    else
    {
        cout << "String is not accepted (amount of " << alphabetNumbers[0] << " is odd).\n";
    }

    cout << "\n\nFiniteStateMachine N2 (This automaton find the substring in a "
            "string.)"
         << endl;

    string alphabetString;
    cout << "Enter the alphabet: ";
    cin >> alphabetString;

    // Convert the alphabet string to a vector of characters
    vector<char> alphabet(alphabetString.begin(), alphabetString.end());

    // Input the substring
    string substring;
    cout << "Enter the substring(without spaces): ";
    cin >> substring;

    // Input the main string
    string inputString;
    cout << "Enter the main string(without spaces): ";
    cin >> inputString;

    // Create an instance of FiniteAutomaton
    StateMachine<int, char> automaton;

    // Set up the automaton transitions based on user input
    automaton.setStates({});
    automaton.setAlphabet(alphabet);
    automaton.setInitial(0);
    automaton.setFinal(substring.length());

    // Inside the main function after setting up the transitions
    map<pair<int, char>, int> transitions;

    for (size_t state = 0; state <= substring.length(); ++state)
    {
        for (char symbol : alphabet)
        {
            size_t nextState = min(state + 1, substring.length());
            while (nextState > 0 && substring[nextState - 1] != symbol)
            {
                nextState =
                    automaton.checkString(vector<char>(substring.begin(), substring.begin() + nextState - 1)) ? 1 : 0;
            }
            transitions[{state, symbol}] = nextState;
        }
    }

    automaton.setTransitions(transitions);

    // Check if the substring is found
    if (automaton.checkSubstring(inputString, substring))
    {
        cout << "Substring " << substring << " was found in a string!" << endl;
    }
    else
    {
        cout << "Substring " << substring << " was not found in a string." << endl;
    }

    return 0;
}
