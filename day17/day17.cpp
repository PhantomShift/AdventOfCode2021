#include "../utility.h"
#include <iostream>
#include <fstream>
#include <numeric>
#include <vector>

template <typename num>
int sign(num n)
{
    if (n > 0) return 1;
    if (n < 0) return -1;
    return 0;
}

template <typename num=int>
struct vector2
{
    num x = 0;
    num y = 0;

    vector2<num> operator+(const vector2<num> other) const { return {this->x + other.x, this->y + other.y}; }
    vector2<num> operator-(const vector2<num> other) const { return {this->x - other.x, this->y - other.y}; }
    void operator+=(const vector2<num> other)
    {
        *this = *this + other;
    }
    void operator-=(const vector2<num> other)
    {
        *this = *this - other;
    }

    vector2<num> operator*(num op) const { return {this->x * op, this->y * op}; }
    void operator*=(num op) { this->x *= op; this->y *= op; }

    void print() { std::cout << '{' << x << ", " << y << "}\n"; }

    bool isInBounds(const vector2<num> a, const vector2<num> b) const
    {
        return (
            x >= std::min(a.x, b.x) &&
            x <= std::max(a.x, b.x) &&
            y >= std::min(a.y, b.y) &&
            y <= std::max(a.y, b.y)
        );
    }
};

// Returns seconds and whether it was within x bounds
template <typename num=int>
std::pair<num, bool> simulateToXBounds(vector2<num> area1, vector2<num> area2, num vx, num step=1)
{
    // the case in which after 1 second it is immediately within the target zone
    if (vx >= std::min(area1.x, area2.x) && vx <= std::max(area1.x, area2.x)) return {1, true};

    num xPos = 0;
    num elapsedTime = 0;
    while (xPos < std::min(area1.x, area2.x) && vx > 0)
    {
        xPos += vx * step;
        vx -= sign(vx) * step;
        elapsedTime += step;
    }

    return {elapsedTime, (xPos >= std::min(area1.x, area2.x) && xPos <= std::max(area1.x, area2.x))};
}

// returns final point and the maximum y value on the path
template <typename num=int>
std::pair<vector2<num>, num> simulateToArea(vector2<num> area1, vector2<num> area2, vector2<num> vel, num step=1)
{
    // the case in which after 1 second it is immediately within the target zone
    if (vel.isInBounds(area1, area2))
        return {vel, 0};

    vector2<num> point = {0, 0};
    num elapsedTime = 0;
    num yMax = 0;
    while (!point.isInBounds(area1, area2) && point.y > std::min(area1.y, area2.y))
    {
        point += vel * step;
        if (point.y > yMax) yMax = point.y;
        vel.x -= sign(vel.x);
        vel.y -= step;
        elapsedTime += step;
    }

    return {point, yMax};
}

void getMaximumYPaths(vector2<int> area1, vector2<int> area2)
{
    std::vector<int> feasibleXVels;
    int xvel = 1;
    std::pair<int, bool> result;
    while (xvel <= std::max(area1.x, area2.x))
    {
        result = simulateToXBounds(area1, area2, xvel);
        if (result.second) feasibleXVels.push_back(xvel);
        xvel++;
    }

    int globalYMax = 0;
    std::vector<vector2<int>> feasibleVelocities;
    for (int xv : feasibleXVels)
    {
        int yv = std::min(area1.y, area2.y);
        while (-yv > std::min(area1.y, area2.y))
        {
            auto [point, yMax] = simulateToArea(area1, area2, {xv, yv});
            if (point.isInBounds(area1, area2))
            {
                if (yMax > globalYMax) globalYMax = yMax;
                while (true)
                {
                    auto [temp, yMax2] = simulateToArea(area1, area2, {xv, yv});
                    if (temp.isInBounds(area1, area2))
                    {
                        if (yMax2 > globalYMax) globalYMax = yMax2;
                        feasibleVelocities.push_back({xv, yv});
                    }
                    else break;
                    yv++;
                }
            }
            yv ++;
        }
    }

    std::cout << "Max found: " << globalYMax << '\n';
    std::cout << "Feasible initial velocities found: " << feasibleVelocities.size() << '\n';
}

void doProblem(std::string inputFile)
{
    std::fstream reading(inputFile);
    std::string readline;
    getline(reading, readline);
    reading.close();

    std::string bounds = readline.substr(13);
    std::vector<std::string> numbers = Utility::splitString(bounds, ", ");
    std::vector<std::string> xNumbers = Utility::splitString(Utility::splitString(numbers[0], "=")[1], "..");
    std::vector<std::string> yNumbers = Utility::splitString(Utility::splitString(numbers[1], "=")[1], "..");

    vector2 area1 = {std::stoi(xNumbers[0]), std::stoi(yNumbers[0])};
    vector2 area2 = {std::stoi(xNumbers[1]), std::stoi(yNumbers[1])};

    getMaximumYPaths(area1, area2);

}

int main()
{
    doProblem("input17");

    return 0;
}