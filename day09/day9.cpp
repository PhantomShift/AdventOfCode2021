#include "../utility.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <tuple>
#include <algorithm>

/*
So this puzzle was a bit of a struggle because I did not read the line,
"all other locations will always be part of exactly one basin."

In all of the given examples, from the low-point to the rest of the basin, there was a reasonable
path by which all the points within the basin could be
1) reached without staying at the same height and
2) reached without increasing the height by more than 1
Thus, I wrongly assumed that if the height stayed the same and the difference in height was more than one,
it would not be considered part of the basin.
What corroborated my assumption was the fact that the test input provided did work with the code I had written.

However, that didn't end up working with the puzzle input and as I removed my assumptions one by one I finally
arrived at the correct answer, only to reread the puzzle page afterwards and realize
I could've avoided this struggle if I had just read the aforementioned line.

Derp.
*/

const std::string toRead{ "input9" };
constexpr int maxInteger{ std::numeric_limits<int>::max() };

using VectorHeightmap = std::vector<std::vector<int>>;
using Coordinate = std::tuple<size_t, size_t>;

std::vector<int> getNeighbors(size_t y, size_t x, const VectorHeightmap &area)
{
    std::vector<int> neighbors{
        y != 0 ? area[y-1][x] : maxInteger, // north
        x != area[y].size() - 1 ? area[y][x+1] : maxInteger, // east
        y != area.size() - 1 ? area[y+1][x] : maxInteger, // south
        x != 0 ? area[y][x-1] : maxInteger // west
    };

    return neighbors;
}

void part1()
{
    std::cout << "Part 1\n";

    std::fstream inputs(toRead);
    std::string readline;

    VectorHeightmap area;

    // Parse inputs
    while (getline(inputs, readline))
    {
        std::vector<int> row;
        for (char input : readline)
        {
            row.push_back(input - '0'); // turn character into digit
        }
        area.push_back(row);
    }
    inputs.close();

    // Figure out which coordinates are the lowest of their neighbors
    // tuple contains pairs <y, x>
    std::vector<Coordinate> lowestHeights;
    for (size_t y{}; y < area.size(); y++)
    {
        for (size_t x{}; x < area[y].size(); x++)
        {
            int height{ area[y][x] };
            std::vector<int> neighbors{ getNeighbors(y, x, area) };
            bool isLowest{ true };
            for (int neighbor : neighbors)
            {
                if (neighbor <= height) isLowest = false;
            }
            if (isLowest) lowestHeights.push_back({y, x});
        }
    }

    // Get sum of risk values
    int totalRisk{};
    for (auto [y, x] : lowestHeights) totalRisk += area[y][x] + 1;

    std::cout << "The sum of risk levels of all low points is " << totalRisk << '\n';
}


// Functions needed for part 2

std::vector<Coordinate> getNeighborCoords(Coordinate &source, const VectorHeightmap &area, std::vector<Coordinate> &blacklisted)
{
    auto [y, x] = source;
    std::vector<Coordinate> initialNeighbors{
        y != 0 ? Coordinate{y - 1, x} : source, // north
        x != area[y].size() - 1 ? Coordinate{y, x + 1} : source, // east
        y != area.size() - 1 ? Coordinate{y + 1, x} : source, // south
        x != 0 ? Coordinate{y, x - 1} : source // west
    };
    std::vector<Coordinate> toReturn;
    for (Coordinate &neighbor : initialNeighbors)
    {
        if (neighbor == source) continue;
        if (std::find(blacklisted.begin(), blacklisted.end(), neighbor) != blacklisted.end()) continue;
        toReturn.push_back(neighbor);
    }

    return toReturn;
}

std::vector<Coordinate> getBasinCoords(size_t initialY, size_t initialX, const VectorHeightmap &area)
{
    std::vector<Coordinate> checked;
    std::vector<Coordinate> toCheck{ Coordinate{initialY, initialX} };
    size_t totalNeighbors{};
    do
    {
        std::vector<Coordinate> newCoords;
        for (Coordinate &checking : toCheck)
        {
            std::vector<Coordinate> neighbors{ getNeighborCoords(checking, area, checked) };
            for (Coordinate &neighbor : neighbors)
            {
                auto [y, x] = neighbor;
                int neighborHeight{ area[y][x] };
                if (neighborHeight < 9) newCoords.push_back(neighbor);
            }
        }
        totalNeighbors = newCoords.size();
        for (Coordinate &justChecked : toCheck)
        {
            if (std::find(checked.begin(), checked.end(), justChecked) == checked.end())
                checked.push_back(justChecked);
        }
        toCheck = newCoords;
    } while (totalNeighbors > 0);

    return checked;
}

// linux/macOS only
void printRed(const std::string &toPrint)
{
    std::cout << "\033[1;31m"<<toPrint<<"\033[0m";
}
void renderBasin(std::vector<Coordinate> &basin, VectorHeightmap &area)
{
    auto b{ basin.begin() };
    auto e{ basin.end() };
    for (size_t y{}; y < area.size()-1; y++)
    {
        for (size_t x{}; x < area[y].size()-1; x++)
        {
            if (std::find(b, e, Coordinate{y, x}) != e)
                printRed(std::to_string(area[y][x]));
            else std::cout << area[y][x];
        }
        std::cout << '\n';
    }
}

void part2()
{
    std::cout << "Part 2\n";

    std::fstream inputs(toRead);
    std::string readline;

    VectorHeightmap area;

    // Parse inputs
    while (getline(inputs, readline))
    {
        std::vector<int> row;
        for (char input : readline)
        {
            row.push_back(input - '0'); // turn character into digit
        }
        area.push_back(row);
    }
    inputs.close();

    // Figure out which coordinates are the lowest of their neighbors
    // tuple contains pairs <y, x>
    std::vector<Coordinate> lowestHeights;
    for (size_t y{}; y < area.size(); y++)
    {
        for (size_t x{}; x < area[y].size(); x++)
        {
            int height{ area[y][x] };
            std::vector<int> neighbors{ getNeighbors(y, x, area) };
            bool isLowest{ true };
            for (int neighbor : neighbors)
            {
                if (neighbor <= height) isLowest = false;
            }
            if (isLowest) lowestHeights.push_back({y, x});
        }
    }

    // Determine basin sizes
    std::vector<size_t> basins;
    std::vector<Coordinate> allBasinCoords;
    for (auto [y, x] : lowestHeights)
    {
        std::vector<Coordinate> basin{ getBasinCoords(y, x, area) };
        allBasinCoords.insert(allBasinCoords.end(), basin.begin(), basin.end());
        basins.push_back(basin.size());
    }

    // uncomment to render all of the points that are part of a basin in red (linux/macOS only)
    // renderBasin(allBasinCoords, area);

    std::sort(basins.begin(), basins.end());
    size_t first{ basins[basins.size()-1]};
    size_t second{ basins[basins.size()-2]};
    size_t third{ basins[basins.size()-3]};

    std::cout << "First: " << first << ", second: " << second << ", third: " << third << '\n';
    std::cout << "The result of multiplication is " << first * second * third << '\n';
}

int main()
{
    part1();
    part2();

    return 0;
}