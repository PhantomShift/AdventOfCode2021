#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::string;

string getRatingBinary(std::vector<string> checkReadings, char charGenerator(int, int))
{
    std::vector<string> validReadings;
    size_t checkPosition{ 0 };
    while (checkReadings.size() > 1)
    {
        // determine counts
        int ones{};
        int zeroes{};
        for (string binary : checkReadings)
        {
            if (binary.at(checkPosition) == '1')
                ones++;
            else
                zeroes++;
        }
        char validChar{ charGenerator(ones, zeroes) };

        // filter readings
        for (string binary : checkReadings)
        {
            if (binary.at(checkPosition) == validChar)
                validReadings.push_back(binary);
        }
        checkReadings = validReadings;
        validReadings.clear();
        checkPosition++;
    }
    return checkReadings.at(0);
}

int main()
{
    std::vector<string> savedReadings;

    std::vector<int> counts;
    std::vector<int> totals;

    std::ifstream readings("input3");

    // get number of times 1 appears
    string input;
    getline(readings, input);
    counts.resize(input.length());
    totals.resize(input.length());
    do
    {
        for (size_t i{}; char character : input)
        {
            if (character == '1')
                counts[i] += 1;
            totals[i] += 1;
            i++;
        }
        savedReadings.push_back(input);
    } while (getline(readings, input));

    readings.close();

    // Determine binary characters
    string gammaBinary;
    string epsilonBinary;
    for (size_t i{}; int total: totals)
    {
        float count{ static_cast<float>(counts[i]) };
        bool isGreater{ count > (total / 2.0f) };
        if (isGreater)
        {
            gammaBinary += '1';
            epsilonBinary += '0';
        }
        else
        {
            gammaBinary += '0';
            epsilonBinary += '1';
        }
        i++;
    }

    // Convert binary strings to integers
    int gamma{ std::stoi(gammaBinary, 0, 2) };
    int epsilon{ std::stoi(epsilonBinary, 0, 2)};

    std::cout << "The gamma value is " << gamma << '\n';
    std::cout << "The binary is " << gammaBinary << '\n';
    std::cout << "The epsilon value is " << epsilon << '\n';
    std::cout << "The binary is " << epsilonBinary << '\n';
    std::cout << "Multiplying results in " << (gamma * epsilon) << '\n';

    // Part 2 - Oxygen and CO2 ratings    
    string oxygenBinary{ getRatingBinary(savedReadings, [](int ones, int zeroes) -> char {
        return ones >= zeroes ? '1' : '0';
    }) };
    int oxygenRating{ std::stoi(oxygenBinary, 0, 2 )};
    std::cout << "Result of processing for oxygen is " << oxygenBinary << '\n';
    std::cout << "The value in decimal is " << oxygenRating << '\n';

    string carbonBinary{ getRatingBinary(savedReadings, [](int ones, int zeroes) -> char {
        return ones >= zeroes ? '0' : '1';
    }) };
    int carbonRating{ std::stoi(carbonBinary, 0, 2) };
    std::cout << "Result of processing for carbon is " << carbonBinary << '\n';
    std::cout << "The value in decimal is " << carbonRating << '\n';

    std::cout << "Multiplying results in " << (oxygenRating * carbonRating) << '\n';

    return 0;
}