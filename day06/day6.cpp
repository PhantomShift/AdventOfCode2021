#include "../utility.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>

void doProblem(int partNumber, int maxDays, const std::string &toRead)
{
    std::cout << "Part " << partNumber << '\n';
    
    std::fstream initialRead(toRead);
    std::string initialString;
    getline(initialRead, initialString);
    initialRead.close();

    std::array<long long, 9> school{0};

    for (std::string stringVal : Utility::splitString(initialString, ","))
    {
        school[static_cast<size_t>(std::stoi(stringVal))]++;
    }

    for (int days{}; days <= maxDays; days++)
    {
        long long toAdd{};
        long long toCycle{};
        for (size_t fish{}; fish < school.size(); fish++)
        {
            if (days != maxDays)
            {
                if (fish == 0U)
                {
                    toCycle = school[fish];
                    toAdd = school[fish];
                }
                else
                    school[fish-1U] = school[fish];
            }
        }
        // Do not add on last day
        if (days != maxDays)
        {
            school[school.size()-1U] = toAdd;
            school[school.size()-3U] += toCycle;
        }
    }

    long long total{};
    for (size_t i{}; i < school.size(); i++)
    {
        long long group{ school[i] };
        total += group;
        std::cout << "Group num: " << i << '\n';
        std::cout << "Total: " << total << '\n';
    }

    std::cout << "Total school size: " << total << '\n';
}

int main()
{
    doProblem(1, 80, "input6");
    // apparently part 2 is just the same but with bigger numbers
    doProblem(2, 256, "input6");

    return 0;
}