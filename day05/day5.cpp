#include "../utility.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>

int signOf(int x)
{
    if (x == 0)
        return 0;
    return (x / abs(x));
}

struct Point2d
{
    int x{};
    int y{};

    bool operator == (const Point2d &other)
    {
        return (x == other.x && y == other.y);
    }

    void print() const { printf("(%d, %d)\n", x, y); }
    bool operator < (const Point2d &other) const {return (asString() < other.asString()); }
    std::string asString() const { return (std::to_string(x) + " " + std::to_string(y)); }
};

struct Line2d
{
    Point2d begin{};
    Point2d end{};

    Line2d(const std::string &toParse)
    {
    using Utility::splitString;
    // Expected size of 2
    std::vector<std::string> split{ splitString(toParse, " -> ")};

    std::vector<std::string> start{ splitString(split[0], ",") };
    std::vector<std::string> ending{ splitString(split[1], ",") };

    begin = {std::stoi(start[0]), std::stoi(start[1])};
    end = {std::stoi(ending[0]), std::stoi(ending[1])};
    }

    void print()
    {
        printf("(%d, %d) -> (%d, %d)\n", begin.x, begin.y, end.x, end.y);
    }

    bool isVertical() { return (begin.x == end.x); }
    bool isHorizontal() { return (begin.y == end.y); }
    bool isDiagonal() { return (abs(end.y - begin.y) == abs(end.x - begin.x)); }

    std::vector<Point2d> getPointsOnLine()
    {
        if (isVertical())
        {
            std::vector<Point2d> result;
            result.reserve(static_cast<size_t>(abs(end.y - begin.y)));
            
            int direction{ signOf(end.y - begin.y) };
            for (int i{ begin.y }; abs(end.y - i) > 0; i += direction)
            {
                result.push_back({end.x, i});
            }
            result.push_back({end.x, end.y});

            return result;
        }
        else if (isHorizontal())
        {
            std::vector<Point2d> result;
            result.reserve(static_cast<size_t>(abs(end.x - begin.x)));
            
            int direction{ signOf(end.x - begin.x) };
            for (int i{ begin.x }; abs(end.x - i) > 0; i += direction)
            {
                result.push_back({i, end.y});
            }
            result.push_back({end.x, end.y});

            return result;
        }
        else
            return {};
    }

    std::vector<Point2d> getPointsOnLineIncludingDiagonal()
    {
        if (isDiagonal())
        {
            std::vector<Point2d> result;
            result.reserve(static_cast<size_t>(
                sqrt(2 * (end.x - begin.x) * (end.x - begin.x))
            ));

            int directionX{ signOf(end.x - begin.x) };
            int directionY{ signOf(end.y - begin.y) };
            int x{ begin.x };
            int y{ begin.y };
            do
            {
                result.push_back({x, y});

                x += directionX;
                y += directionY;
            } while (x != end.x);
            result.push_back({x, y});

            return result;
        }
        else
            return getPointsOnLine();
    }
};

void part1()
{
    std::cout << "Part 1\n";
    std::ifstream read("input5");

    std::string input;
    std::vector<Point2d> coveredPoints;
    std::map<Point2d, int> pointsMap;
    int xMax{};
    int yMax{};
    while (getline(read, input))
    {
        Line2d line(input);
        for (Point2d point : line.getPointsOnLine())
        {
            coveredPoints.push_back(point);
            pointsMap[point] += 1;
        }
        if (line.begin.x > xMax)
            xMax = line.begin.x;
        else if (line.end.x > xMax)
            xMax = line.end.x;
        if (line.begin.y > yMax)
            yMax = line.begin.y;
        else if (line.end.y > yMax)
            yMax = line.end.y;
    }
    read.close();

    // Check how many times each point appeared
    int totalOccurrences{};
    auto pointsBegin{ coveredPoints.begin() };
    auto pointsEnd{ coveredPoints.end() };
    for (auto [str, num] : pointsMap)
    {
        if (num >= 2)
            totalOccurrences++;
    }
    printf("There are %d points at which at least two points overlap.\n", totalOccurrences);
}

void part2()
{
    std::cout << "Part 2\n";
    std::ifstream read("input5");

    std::string input;
    std::vector<Point2d> coveredPoints;
    std::map<Point2d, int> pointsMap;
    int xMax{};
    int yMax{};
    while (getline(read, input))
    {
        Line2d line(input);
        for (Point2d point : line.getPointsOnLineIncludingDiagonal())
        {
            coveredPoints.push_back(point);
            pointsMap[point] += 1;
        }
        if (line.begin.x > xMax)
            xMax = line.begin.x;
        else if (line.end.x > xMax)
            xMax = line.end.x;
        if (line.begin.y > yMax)
            yMax = line.begin.y;
        else if (line.end.y > yMax)
            yMax = line.end.y;
    }
    read.close();

    // Check how many times each point appeared
    int totalOccurrences{};
    auto pointsBegin{ coveredPoints.begin() };
    auto pointsEnd{ coveredPoints.end() };
    for (auto [str, num] : pointsMap)
    {
        if (num >= 2)
            totalOccurrences++;
    }
    printf("There are %d points at which at least two points overlap.\n", totalOccurrences);

    // uncomment for ASCII art (only use with test input)
    // for (int x{}; x <= xMax; x++)
    // {
    //     for (int y{}; y <= yMax; y++)
    //     {
    //         Point2d p{ y, x };
    //         std::cout << std::count(pointsBegin, pointsEnd, p);
    //     }
    //     std::cout << '\n';
    // }
}

int main()
{
    part1();
    part2();

    return 0;
}