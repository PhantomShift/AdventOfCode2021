#include "../utility.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <tuple>
#include <cassert>
#include <algorithm>

void part1()
{
    std::cout << "Part 1\n";

    std::fstream read("input8");
    std::string input;

    int ones{};
    int fours{};
    int sevens{};
    int eights{};

    while (getline(read, input))
    {
        std::vector<std::string> split{ Utility::splitString(input, "|") };
        std::vector<std::string> splitOutputs{ Utility::splitString(split[1]) };

        for (std::string output : splitOutputs)
        {
            // std::cout << output << ',' << output.length() << '\n';
            switch(output.length())
            {
            case(2): ones += 1; break;
            case(3): sevens += 1; break;
            case(4): fours += 1; break;
            case(7): eights += 1; break;
            }
        }
    }

    read.close();

    std::cout << "The number of digits with a unique number of segments is " << (ones + fours + sevens + eights) << '\n';
}

// Determines if ALL CHARACTERS in a are also present in b
bool sharesCharactersWith(const std::string &a, const std::string &b)
{
    for (auto character : a)
    {
        if (b.find(character) == std::string::npos)
            return false;
    }
    return true;
}

// Determines if a is an anagram of b (contains the same letters but rearranged)
// Assumes all characters are alphabetical and lowercase
bool isAnagram(const std::string &a, const std::string &b)
{
    if (a.length() != b.length()) return false;
    auto aBegin{ a.begin() };
    auto aEnd{ a.end() };
    auto bBegin{ b.begin() };
    auto bEnd{ b.end() };

    for (char character{ 'a' }; character < 'z'; character++)
    {
        if (std::count(aBegin, aEnd, character) != std::count(bBegin, bEnd, character))
        {
            return false;
        }
    }

    return true;
}

// Returns a copy of a given string with the provided characters removed
std::string stripCharacters(std::string toCopy, const std::string &toRemove)
{
    for (char character : toRemove)
    {
        size_t index{ toCopy.find(character) };
        while (index != std::string::npos)
        {
            toCopy.replace(index, 1, "");
            index = toCopy.find(character);
        }
    }
    return toCopy;
}

// Returns tuple that contains the top right and bottom right characters
std::tuple<char, char> determineRightSide(const std::string &one, const std::string &eight, const std::vector<std::string> &signals)
{
    for (std::string signal : signals)
    {
        // Attempting to find 6
        if (signal.length() != 6) continue; // number is not 0, 6 or 9
        if (sharesCharactersWith(one, signal)) continue; // number is 0 or 9
        
        // Number is 6
        if (signal.find(one[0]) == std::string::npos ) // one[0] is top right
        {
            return std::tuple<char, char>{one[0], one[1]};
        }
        else // one[0] is bottom right
        {
            return std::tuple<char, char>{one[1], one[0]};
        }
    }
    assert(false && "Unreachable");
}
// Top, top left and bottom right is now determined

// Returns tuple that contains the bottom left and bottom characters
std::tuple<char, char> determineBottom(char topRight, char bottomRight, const std::string &four, const std::string &seven, const std::string &eight, const std::vector<std::string> &signals)
{
    for (std::string signal : signals)
    {
        // Attempting to find 5
        if (signal.length() != 5) continue; // number is not 2, 3 or 5
        if (signal.find(topRight) != std::string::npos) continue; // number is 3
        if (signal.find(bottomRight) == std::string::npos) continue; // number is 2
        
        // Number is 5
        std::string bottomString{ stripCharacters(signal, four + seven) }; // By stripping the characters within 4 and 7 from 5, only the bottom should remain
        char bottomChar{ bottomString[0] };

        // We can also determine the bottom left now since the bottom and bottom left candidates can be derived from stripping 4 and 7 from 8
        std::string bottomCandidates{ stripCharacters(eight, four + seven) };
        std::string bottomLeftString{ stripCharacters(bottomCandidates, bottomString) };
        char bottomLeftChar{ bottomLeftString[0] };

        return std::tuple<char, char>{ bottomLeftChar, bottomChar };
    }

    assert(false && "Unreachable");
}
// Top, top left, bottom right, bottom left and bottom is now determined

// Returns tuple that contains the middle and top left characters
std::tuple<char, char> determineMiddleAndTopLeft(const std::string &backwardsC, const std::string &four, const std::vector<std::string> &signals)
{
    for (std::string signal : signals)
    {
        // Attempting to find 3
        if (signal.length() != 5) continue; // number is not 2, 3 or 5
        // Three literally contains all of the determined characters that form a C at the moment
        if (!sharesCharactersWith(backwardsC, signal)) continue;
        
        // Number is 3
        // Stripping all of the known characters that form a C from 3 will return the middle
        std::string middleString{ stripCharacters(signal, backwardsC) };
        char middleChar{ middleString[0] };

        // The top left character can also be determined from here by stripping the characters of 3 from 4
        std::string topLeftString{ stripCharacters(four, signal) };
        char topLeftChar{ topLeftString[0] };

        return std::tuple<char, char>{ middleChar, topLeftChar };
    }    

    assert(false && "Unreachable");
}
// All characters are now known

class SignalParser
{
    std::array<std::string, 10> combinations{};

public:
    SignalParser(char top, char topLeft, char topRight, char middle, char bottom, char bottomLeft, char bottomRight)
    {
        combinations[0] = { top, topLeft, topRight, bottomLeft, bottomRight, bottom };
        combinations[1] = { topRight, bottomRight };
        combinations[2] = { top, topRight, middle, bottomLeft, bottom };
        combinations[3] = { top, topRight, middle, bottomRight, bottom };
        combinations[4] = { topLeft, topRight, middle, bottomRight };
        combinations[5] = { top, topLeft, middle, bottomRight, bottom };
        combinations[6] = { top, topLeft, middle, bottomLeft, bottomRight, bottom };
        combinations[7] = { top, topRight, bottomRight };
        combinations[8] = { top, topLeft, topRight, middle, bottomLeft, bottomRight, bottom };
        combinations[9] = { top, topLeft, topRight, middle, bottomRight, bottom };
    }

    std::string parse(const std::string &signal)
    {
        for (int i{}; i < 10; i++)
        {
            if (isAnagram(signal, combinations[static_cast<size_t>(i)])) return std::to_string(i);
        }
        assert(false && "Unreachable");
    }
};

void part2()
{
    std::cout << "Part 2\n";

    std::fstream read("input8");
    std::string input;

    int outputSum{};

    while (getline(read, input))
    {
        std::vector<std::string> split{ Utility::splitString(input, "|") };
        std::vector<std::string> signals{ Utility::splitString(split[0]) };
        std::vector<std::string> splitOutputs{ Utility::splitString(split[1]) };

        // Acquire one, four, seven, and eight from the given signals
        std::string one;
        std::string four;
        std::string seven;
        std::string eight;
        for (std::string signal: signals)
        {
            switch(signal.length())
            {
            case(2): one = signal; break;
            case(3): seven = signal; break;
            case(4): four = signal; break;
            case(7): eight = signal; break;
            }
        }

        char top{ stripCharacters(seven, one)[0] };
        auto[topRight, bottomRight]{ determineRightSide(one, eight, signals) };
        auto[bottomLeft, bottom]{ determineBottom(topRight, bottomRight, four, seven, eight, signals) };
        std::string backwardsC{ top, topRight, bottomRight, bottom };
        auto[middle, topLeft]{ determineMiddleAndTopLeft(backwardsC, four, signals) };

        // Create signal parser
        SignalParser Parser{ top, topLeft, topRight, middle, bottom, bottomLeft, bottomRight };

        // Determine digits from output
        std::string digits;
        for (std::string output : splitOutputs)
        {
            digits += Parser.parse(output);
        }

        // std::cout << split[0] << ": " << digits << '\n';

        outputSum += std::stoi(digits);
    }
    read.close();

    std::cout << "The total of the given outputs was " << outputSum << '\n';

}

int main()
{
    part1();
    part2();

    return 0;
}