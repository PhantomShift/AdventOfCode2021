#include "../utility.h"
#include <iostream>
#include <fstream>
#include <map>
#include <limits>

const std::string toRead{ "input14" };

void doProblem(int iterations = 10)
{
    printf("Iterations to do: %d\n", iterations);

    std::fstream inputs(toRead);
    std::string readline;

    getline(inputs, readline);
    std::string polymer = readline;

    getline(inputs, readline); // throw away empty line

    std::map<std::string, std::pair<std::string, std::string>> rules;
    std::map<std::string, long long> totals;
    while (getline(inputs, readline))
    {
        std::vector<std::string> split { Utility::splitString(readline, " -> ") };
        std::string first = {split[0][0], split[1][0]};
        std::string second = {split[1][0], split[0][1]};
        rules[split[0]] = {first, second};
    }
    inputs.close();
    
    // Get initial totals
    for (size_t i = 0; i < polymer.size() - 1; i++)
    {
        std::string pair = polymer.substr(i, 2U);
        totals[pair] += 1;
    }

    // Do iterations
    for (int i = 0; i < iterations; i++)
    {
        std::map<std::string, long long> newTotals;
        for (auto [str, total] : totals)
        {
            auto [first, second] = rules[str];
            newTotals[first] += total;
            newTotals[second] += total;
        }
        totals.clear();
        for (auto [str, total] : newTotals) totals[str] = total;
    }

    // Determine totals of individual characters
    std::map<char, long long> charTotals;
    for (auto [pair, total] : totals)
        charTotals[pair[1]] += total;

    // Get greatest/lowest
    long long greatest = 0;
    long long lowest = std::numeric_limits<long long>::max();
    for (auto [character, total] : charTotals)
    {
        if (total > greatest) greatest = total;
        if (total < lowest) lowest = total;
    }
    
    printf("Result of subtracting least common from most common is %lld\n", greatest - lowest);
}


int main()
{
    doProblem();
    printf("\n");
    doProblem(40);
    
    return 0;
}