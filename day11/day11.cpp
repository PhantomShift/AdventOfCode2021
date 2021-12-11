#include <iostream>
#include <fstream>
#include <vector>

const std::string toRead{ "input11" };

using Map = std::vector<std::vector<int>>;

int flashOctopus(int yPos, int xPos, Map &map)
{
    int flashes = 1;
    size_t mapYMax = map.size();
    size_t mapXMax = map[0].size();
    for (int y = -1; y <= 1; y++)
    {
        for (int x = -1; x <= 1; x++)
        {
            size_t checkY = static_cast<size_t>(yPos + y);
            size_t checkX = static_cast<size_t>(xPos + x);
            // Ignore if out of bounds or if current position
            // Originally had a goto statement here to skip which is why it's kind of awkward
            if (!(checkX >= mapXMax || checkY >= mapYMax || (x == 0 && y == 0)))
            {
                map[checkY][checkX] += 1;
                if (map[checkY][checkX] == 10) flashes += flashOctopus(checkY, checkX, map);
            }
        }
    }
    return flashes;
}

void part1(int maxSteps = 100)
{
    printf("Part 1\n");

    std::fstream inputs(toRead);
    std::string readline;

    // Read lines and place in map
    Map map;
    while (getline(inputs, readline))
    {
        std::vector<int> toPush;
        for (char number : readline) toPush.push_back(number - '0');
        map.push_back(toPush);
    }

    int totalFlashes = 0;
    for (int step = 0; step < maxSteps; step++)
    {
        // Determine which ones are about to flash
        std::vector<std::pair<int, int>> toFlash;
        for (int y = 0; std::vector<int> &line : map)
        {
            for (int x = 0; int &number : line) // reference to add 1
            {
                number += 1;
                if (number > 9) toFlash.push_back(std::pair<int, int>{y, x});
                x++;
            }
            y++;
        }

        for (auto [y, x] : toFlash)
        {
            totalFlashes += flashOctopus(y, x, map);
        }

        for (std::vector<int> &line : map)
        {
            for (int &number : line)
            {
                if (number > 9) number = 0;
            }
        }
    }

    printf("The total number of flashes is %d\n", totalFlashes);
}

void part2()
{
    printf("Part 2\n");

    std::fstream inputs(toRead);
    std::string readline;

    // Read lines and place in map
    Map map;
    while (getline(inputs, readline))
    {
        std::vector<int> toPush;
        for (char number : readline) toPush.push_back(number - '0');
        map.push_back(toPush);
    }

    bool synchronized = false;
    int step = 0;
    while(!synchronized)
    {
        // Essentially we keep going until all the octopi have an energy level of 0
        synchronized = true;
        // Determine which ones are about to flash
        std::vector<std::pair<int, int>> toFlash;
        for (int y = 0; std::vector<int> &line : map)
        {
            for (int x = 0; int &number : line) // reference to add 1
            {
                number += 1;
                if (number > 9) toFlash.push_back(std::pair<int, int>{y, x});
                x++;
            }
            y++;
        }

        for (auto [y, x] : toFlash) flashOctopus(y, x, map);

        for (std::vector<int> &line : map)
        {
            for (int &number : line)
            {
                if (number > 9) number = 0;
                else synchronized = false;
            }
        }
        step++;
    }

    printf("The octopi become synchronized at step %d\n", step);
}

int main()
{
    part1();
    part2();

    return 0;
}