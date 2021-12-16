#include "astar.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <list>

// linux/macOS only
void printRed(const std::string &toPrint)
{
    std::cout << "\033[1;31m"<<toPrint<<"\033[0m";
}
void printBlue(const std::string &toPrint)
{
    std::cout << "\033[1;34m"<<toPrint<<"\033[0m";
}

float distance(A_Star::node<int> a, A_Star::node<int> b)
{
    return static_cast<float>(b.value);
}

void part1(std::string filename)
{
    std::fstream inputs(filename);
    std::string readline;

    // generate map
    A_Star::map<int> map;
    size_t y = 0;
    while (getline(inputs, readline))
    {
        std::vector<A_Star::node<int>> toPush;
        for (size_t x = 0; char character : readline)
        {
            toPush.push_back({x, y, std::stoi(std::string{character})});
            x++;
        }
        map.nodes.push_back(toPush);
        y++;
    }
    inputs.close();

    A_Star::node<int> &start = map.nodes[0][0];
    A_Star::node<int> &end = map.nodes[map.nodes.size()-1][map.nodes[0].size()-1];

    A_Star::solve<int>(map, start, end, distance);

    int totalRisk = end.gscore;

    std::cout << "Total risk was: " << totalRisk << '\n';
}

void part2(std::string filename)
{
    std::fstream inputs(filename);
    std::string readline;

    // generate vector map
    std::vector<std::vector<int>> vector;
    while (getline(inputs, readline))
    {
        std::vector<int> toPush;
        for (char character : readline)
        {
            toPush.push_back(std::stoi(std::string{character}));
        }
        vector.push_back(toPush);
    }
    inputs.close();

    // expand map to 25 times the size
    // expand horizontally by 5
    for (size_t y = 0; y < vector.size(); y++)
    {
        std::vector<int> rowCopy = {vector[y]};
        for (int i = 0; i < 4; i++)
        {
            for (size_t x = 0; x < rowCopy.size(); x++)
            {
                vector[y].push_back((rowCopy[x] + i) % 9 + 1);
            }
        }
    }
    // expand vertically by 5
    size_t originalSize = vector.size();
    for (int i = 0; i < 4; i++)
    {
        for (size_t y = 0; y < originalSize; y++)
        {
            std::vector<int> toPush;
            for (size_t x = 0; x < vector[y].size(); x++)
            {
                toPush.push_back((vector[y][x] + i) % 9 + 1);
            }
            vector.push_back(toPush);
        }
    }


    A_Star::map<int> map;
    for (size_t y = 0; auto a : vector)
    {
        std::vector<A_Star::node<int>> toPush;
        for (size_t x = 0; auto b : a)
        {
            toPush.push_back(A_Star::node<int>{x, y, b});

            x++;
        }
        map.nodes.push_back(toPush);
        y++;
    }

    A_Star::node<int> &start = map.nodes[0][0];
    A_Star::node<int> &end = map.nodes[map.nodes.size()-1][map.nodes[0].size()-1];

    std::list<A_Star::coordinate> path = A_Star::solve<int>(map, start, end, distance);

    int totalRisk = end.gscore;

    // Comment out to print path (note that color only works with linux/macos)
    // Also the output for the real input is gigantic

//     for (A_Star::coordinate &coord : path)
//     {
//         vector[coord.second][coord.first] += 10;
//     }
//     for (size_t y = 0; auto a : vector)
//     {
//         for (size_t x = 0; auto b : a)
//         {
// #ifdef _WIN32
//             std::cout << b;
// #else
//             if (b > 9)
//                 printRed(std::to_string(b - 10));
//             else if (map.nodes[y][x].visited)
//                 printBlue(std::to_string(b));
//             else std::cout << b;
// #endif

//             x++;
//         }

//         std::cout << '\n';
//         y++;
//     }

    std::cout << "Total risk was: " << totalRisk << '\n';
}

int main()
{
    part1("input15");
    part2("input15");

    return 0;
}