#include "../utility.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <numeric>

void part1()
{
    std::cout << "Part1\n";

    std::fstream read("input7");
    std::string inputString;
    getline(read, inputString);
    read.close();
    std::vector<std::string> inputs{ Utility::splitString(inputString, ",") };
    std::vector<int> positions;
    positions.reserve(inputs.size());

    int highestPosition{};
    for (std::string input : inputs){
        int position{ std::stoi(input) };
        if (position > highestPosition) highestPosition = position;
        positions.push_back(position);
    }

    // Go through each position one by one and check which one is optimal for fuel consumption
    int lowestFuelConsumed{ std::numeric_limits<int>::max() };
    int idealPosition{};
    for (int pos{}; pos < highestPosition; pos++)
    {
        int fuelConsumed{};
        for (int position : positions)
            fuelConsumed += abs(pos - position);
        
        if (fuelConsumed < lowestFuelConsumed)
        {
            lowestFuelConsumed = fuelConsumed;
            idealPosition = pos;
        }
    }

    printf("The ideal position is %d which consumes %d fuel\n", idealPosition, lowestFuelConsumed);
}

int sumTo(int x)
{
    int sum{};
    for (int i{}; i <= x; i++)
        sum += i;

    return sum;
}

void part2()
{
    std::cout << "Part2\n";
    
    std::fstream read("input7");
    std::string inputString;
    getline(read, inputString);
    read.close();
    std::vector<std::string> inputs{ Utility::splitString(inputString, ",") };
    std::vector<int> positions;
    positions.reserve(inputs.size());

    int highestPosition{};
    for (std::string input : inputs){
        int position{ std::stoi(input) };
        if (position > highestPosition) highestPosition = position;
        positions.push_back(position);
    }

    // Go through each position one by one and check which one is optimal for fuel consumption
    int lowestFuelConsumed{ std::numeric_limits<int>::max() };
    int idealPosition{};
    for (int pos{}; pos < highestPosition; pos++)
    {
        int fuelConsumed{};
        for (int position : positions)
            // Only practical difference is the use of sumTo() to reflect the change in fuel consumption
            fuelConsumed += sumTo(abs(pos - position));
        
        if (fuelConsumed < lowestFuelConsumed)
        {
            lowestFuelConsumed = fuelConsumed;
            idealPosition = pos;
        }
    }

    printf("The ideal position is %d which consumes %d fuel\n", idealPosition, lowestFuelConsumed);
}

int main()
{
    part1();
    part2();

    return 0;
}