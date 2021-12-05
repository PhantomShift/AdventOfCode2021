#include <iostream>
#include <fstream>
#include <limits>
#include <vector>

int main()
{
    std::vector<int> inputs;

    std::ifstream inputFile("input");
    int increases{ 0 };
    int previousInput{ std::numeric_limits<int>::max() };
    std::string input;
    while (getline(inputFile, input))
    {
        int newInput{ std::stoi(input) };
        if (newInput > previousInput)
            increases++;
        previousInput = newInput;
        inputs.push_back(previousInput);
    }

    inputFile.close();

    std::cout << "The depth read increased a total of " << increases << " times.\n";

    // Now, they want to comparisons of every three
    int tripleIncreases{ 0 };
    int prevTripleInput{ std::numeric_limits<int>::max() };
    size_t inputsSize{ inputs.size() };
    for (size_t index{2}; index < inputsSize; index++)
    {
        int newInput{ inputs[index] + inputs[index-1] + inputs[index-2] };
        if (newInput > prevTripleInput)
            tripleIncreases++;
        prevTripleInput = newInput;
    }

    std::cout << "In comparisons of three, the depth read increased a total of " << tripleIncreases << " times.\n";

    return 0;
}