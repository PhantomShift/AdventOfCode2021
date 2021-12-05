#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>

// so essentially how delimiting can be implemented
std::vector<std::string> splitString(const std::string &input, const std::string &delimiter=" ", bool ignoreEmpty=true)
{
    std::vector<std::string> result;
    size_t inputLength{ input.length() };
    size_t delimiterLength{ delimiter.length() };
    result.reserve(inputLength / 4);
    
    size_t prevIndex{ 0 };
    size_t index;
    do
    {
        index = input.find(delimiter, prevIndex);
        std::string found{ input.substr(prevIndex, index) };
        if (found.length() > 0 || !ignoreEmpty)
            result.push_back(found);
        prevIndex = index + delimiterLength;
    } while (index != std::string::npos);

    return result;
}

struct ReadResult
{
    std::string direction;
    int units;
};

ReadResult readInstruction(const std::string& instruction)
{
    std::size_t spacePos{ instruction.find(' ') };
    std::string direction{ instruction.substr(0, spacePos) };
    int units{ std::stoi(instruction.substr(spacePos + 1, instruction.size())) };
    // std::cout << direction << ' ' << units << '\n';
    std::vector<std::string> test{ splitString(instruction) };
    for (std::string element : test)
        std::cout << element << '\n';
    return { direction, units };
}

int main(int argc, char* argv[])
{
    assert(argc > 1 && "input file needed");
    std::string input{ argv[1] };
    
    int horizontal{};
    int depth{};
    std::ifstream inputFile(input);
    std::string instruction;
    while (getline(inputFile, instruction))
    {
        auto[direction, units]{readInstruction(instruction)};
        if (direction == "forward")
            horizontal += units;
        else if (direction == "down")
            depth += units;
        else if (direction == "up")
            depth -= units;
    }

    inputFile.close();

    std::cout << "Horizontal position is " << horizontal << " and depth is " << depth << '\n';
    std::cout << "Multiplying these together results in " << horizontal * depth << '\n';

    std::ifstream inputReread(input);
    horizontal = 0;
    depth = 0;
    int aim{};
    while (getline(inputReread, instruction))
    {
        auto[direction, units]{readInstruction(instruction)};
        if (direction == "forward")
        {    
            horizontal += units;
            depth += units * aim;
        }
        else if (direction == "down")
            aim += units;
        else if (direction == "up")
            aim -= units;
    }
    std::cout << "Updated horizontal position is " << horizontal << " and depth is " << depth << '\n';
    std::cout << "Updated multiplication results in " << horizontal * depth << '\n';

    inputReread.close();

    return 0;
}