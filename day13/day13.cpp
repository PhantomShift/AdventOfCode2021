#include "../utility.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

const std::string toRead = "input13";

using Coordinate = std::pair<int, int>;

Coordinate getPairBounds(std::vector<Coordinate> &pairs)
{
    int xMax = 0;
    int yMax = 0;
    for (auto [x, y] : pairs)
    {
        if (x > xMax) xMax = x;
        if (y > yMax) yMax = y;
    }
    return {xMax, yMax};
}

void doProblem()
{
    std::fstream inputs(toRead);
    std::string readline;

    std::vector<Coordinate> coordinates;
    while (getline(inputs, readline))
    {
        if (readline == "") break;

        std::vector<std::string> split{ Utility::splitString(readline, ",") };
        coordinates.push_back(Coordinate{ std::stoi(split[0]), std::stoi(split[1]) });
    }

    bool firstInstruction = true;
    while (getline(inputs, readline))
    {
        std::string instruction{ readline.substr(11U) };
        std::vector<std::string> split{ Utility::splitString(instruction, "=") };
        std::string direction{ split[0] };
        int position = std::stoi(split[1]);

        if (direction == "y")
        {
            for (auto &[x, y] : coordinates) y = position - abs( position - y );
        }
        else if (direction == "x")
        {
            for (auto &[x, y] : coordinates) x = position - abs( position - x);
        }
        

        // remove duplicates
        for (size_t index = 0; index < coordinates.size(); index++)
        {
            size_t reverseFind = Utility::vectorRfind(coordinates, coordinates[index]);
            if (reverseFind != index) coordinates.erase(coordinates.begin() + static_cast<int>(reverseFind));
        }

        if (firstInstruction)
        {
            std::cout << "Number of coordinates after first fold: " << coordinates.size() << '\n';
            firstInstruction = false;
        }
    }
    inputs.close();


    std::cout << coordinates.size() << '\n';
    Coordinate bounds{ getPairBounds(coordinates) };
    std::string toPrint;
    for (int y = 0; y < bounds.second + 1; y++)
    {
        for (int x = 0; x < bounds.first + 1; x++)
        {
            Coordinate coordinate{x, y};
            if (Utility::vectorFind(coordinates, coordinate) != coordinates.size())
            {
                toPrint += '#';
            }
            else toPrint += '.';
        }
        toPrint += '\n';
    }
    std::cout << toPrint;
}

int main()
{
    doProblem();

    return 0;
}