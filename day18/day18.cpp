#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>

using std::string, std::to_string;

std::pair<string, string> splitOnce(const string &toSplit, const string &delimeter="", size_t start=0U)
{
    // just split at index
    if (delimeter == "")
        return { toSplit.substr(0, start), toSplit.substr(start) };
    
    size_t index = toSplit.find(delimeter, start);
    return { toSplit.substr(0, index), toSplit.substr(index + delimeter.size()) };
}

string repeat(const string& toRepeat, uint reps)
{
    string res = "";
    for (uint i; i < reps; i++) res += toRepeat;
    return res;
}

// operate as strings

string reduceSnailfish(string fish, bool executeOnce=false)
{
    int braces = 0;
    bool mustExplode = false;
    size_t index = 0;
    for (char character : fish)
    {
        if (character == '[') braces++;
        else if (character == ']') braces--;
        if (braces == 5) { mustExplode = true; break; }
        index++;
    }
    if (mustExplode)
    {
        uint leftDigits = 1 + isdigit(fish[index+2]);
        uint rightDigits = 1 + isdigit(fish[index+3+leftDigits]);
        int left = std::stoi(fish.substr(index+1, leftDigits));
        int right = std::stoi(fish.substr(index+2+leftDigits, rightDigits));

        if (fish[index-1] == ',') // left side always contains a number
        {
            size_t addingLeftDigits = 1 + std::isdigit(fish[index-3]);
            int addingToLeft = std::stoi(fish.substr(index-addingLeftDigits-1, addingLeftDigits));
            // [[[1,[12,13]]],11]
            // [[[1,[,]]],11]

            // 1,[12,13]
            string toReplace = fish.substr(index-addingLeftDigits-1, 4 + addingLeftDigits + leftDigits + rightDigits);
            auto [leftSide, rightSide] = splitOnce(fish, toReplace, index-addingLeftDigits-1);
            // left : [[[
            // right: ]],11]
            size_t rightIndex = 0;
            while(!std::isdigit(rightSide[rightIndex]) && rightIndex < rightSide.size())
                rightIndex++;
            if (rightIndex < rightSide.size())
            {
                // elements on right side MAY contain two digits
                size_t digits = 1 + std::isdigit(rightSide[rightIndex+1]);
                int addingToRight = std::stoi(rightSide.substr(rightIndex, digits));
                rightSide.replace(rightIndex, digits, to_string(right + addingToRight));
            }
            
            string newNumber = to_string(left + addingToLeft);

            //              [[[        13           ,0    ]],24]
            string result = leftSide + newNumber + ",0" + rightSide;
            // [[[13,0]],24]
            return { executeOnce ? result : reduceSnailfish(result) };
        }
        else // right side contains EITHER a number OR a pair
        {
            // elements to the right MAY contain two digit numbers
            // [1[[12,13],14]]
            // [1[[,],14]]
            size_t addRightIndex = 4 + leftDigits + rightDigits;

            bool elementRightIsPair = !std::isdigit(fish[index+addRightIndex]);
            if (elementRightIsPair) addRightIndex++;

            size_t addRightDigits = 1 + std::isdigit(fish[index+addRightIndex+1]);
            int addRight = std::stoi(fish.substr(index+addRightIndex, addRightDigits));

            // [12,13],14
            string toReplace = fish.substr(index, addRightIndex+addRightDigits);
            auto [leftSide, rightSide] = splitOnce(fish, toReplace, index);
            // left : [1[
            // right: ]]
            size_t leftIndex = leftSide.size();
            while(!std::isdigit(leftSide[leftIndex]) && leftIndex > 0)
                leftIndex--;
            if (leftIndex > 0)
            {
                size_t digits = 1 + std::isdigit(leftSide[leftIndex-1]);
                int addingToLeft = std::stoi(fish.substr(leftIndex - digits + 1, digits));
                leftSide.replace(leftIndex - digits + 1, digits, to_string(left + addingToLeft));
            }

            string newNumber = to_string(right + addRight);

            //              [13[        0,    27          ]]
            string result = leftSide + ( elementRightIsPair ? "0,[" : "0," ) + newNumber + rightSide;
            // [13[0,27]]
            return { executeOnce ? result : reduceSnailfish(result) };
        }
    }
    
    bool mustSplit = false;
    index = 0;
    for (char character : fish)
    {
        // if both this character and the next is a digit, it is greater than 9
        if (std::isdigit(character) && std::isdigit(fish[index+1]))
        {
            mustSplit = true;
            break;
        }
        index++;
    }
    if (mustSplit)
    {
        // index is currently at the start of a two-digit number
        int toSplit = std::stoi(fish.substr(index, 2));
        string result = fish;
        result.replace(
            index, 2,
            "[" + to_string(toSplit / 2) + "," + to_string((toSplit+1) / 2) + "]"
        );

        return { executeOnce ? result : reduceSnailfish(result) };
    }

    return fish;
}

string addSnailish(const string &a, const string &b)
{
    return reduceSnailfish("[" + a + "," + b + "]");
}

int calculateMagnitude(string snailfish)
{
    while (!std::isdigit(snailfish[0]))
    {
        int maxBraces = 0;
        int braces = 0;
        size_t pos = 0;
        for (size_t index = 0; char character : snailfish)
        {
            switch(character)
            {
            case(']'): braces--; break;
            case('['):
            {
                braces++;
                if (braces > maxBraces) { maxBraces = braces; pos = index; }
                break;
            }
            }
            index++;
        }

        size_t size = 0;
        while (snailfish[pos + size] != ']')
            size++;
        auto [left, right] = splitOnce(snailfish.substr(pos+1, size-1), ",");
        int magnitude = std::stoi(left) * 3 + std::stoi(right) * 2;
        snailfish.replace(pos, size+1, to_string(magnitude));
    }

    return std::stoi(snailfish);
}

void test()
{
    std::vector<std::pair<string, string>> cases
    {
        {"[[[[[9,8],1],2],3],4]", "[[[[0,9],2],3],4]"},
        {"[7,[6,[5,[4,[3,2]]]]]", "[7,[6,[5,[7,0]]]]"},
        {"[[6,[5,[4,[3,2]]]],1]", "[[6,[5,[7,0]]],3]"},
        {"[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]", "[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]"},
        {"[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]", "[[3,[2,[8,0]]],[9,[5,[7,0]]]]"},
    };

    for (auto &[input, expected] : cases)
    {
        assert(reduceSnailfish(input, true) == expected);
    }

    assert(reduceSnailfish("[[[[[4,3],4],4],[7,[[8,4],9]]],[1,1]]") == "[[[[0,7],4],[[7,8],[6,0]]],[8,1]]");
    assert(addSnailish("[[[[4,3],4],4],[7,[[8,4],9]]]", "[1,1]") == "[[[[0,7],4],[[7,8],[6,0]]],[8,1]]");

    std::cout << addSnailish("[[[0,[4,5]],[0,0]],[[[4,5],[2,6]],[9,5]]]", "[7,[[[3,7],[4,3]],[[6,3],[8,8]]]]") << '\n';

    std::vector<std::pair<string, int>> magnitudeCases
    {
        {"[[1,2],[[3,4],5]]", 143},
        {"[[[[0,7],4],[[7,8],[6,0]]],[8,1]]", 1384},
        {"[[[[1,1],[2,2]],[3,3]],[4,4]]", 445},
        {"[[[[3,0],[5,3]],[4,4]],[5,5]]", 791},
        {"[[[[5,0],[7,4]],[5,5]],[6,6]]", 1137},
        {"[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]", 3488},
    };
    for (auto &[input, expected] : magnitudeCases)
    {
        assert(calculateMagnitude(input) == expected);
    }
    
    std::cout << "Passed all tests\n";
}

void part1(const string &filename)
{
    std::fstream input(filename);
    string readline;
    string fish;
    getline(input, fish);
    while (getline(input, readline))
        fish = addSnailish(fish, readline);
    
    std::cout << "Resulting fish: " << fish << '\n';
    std::cout << "Magnitude: " << calculateMagnitude(fish) << '\n';
}

void part2(const string &filename)
{
    std::fstream input(filename);
    string readline;
    std::vector<string> snailfishes;
    while (getline(input, readline))
        snailfishes.push_back(readline);
    
    int greatest = 0;
    for (string fish1 : snailfishes)
    {
        for (string fish2 : snailfishes)
        {
            if (fish1 != fish2)
            {
                int magnitude = calculateMagnitude(addSnailish(fish1, fish2));
                if (magnitude > greatest) greatest = magnitude;
            }
        }
    }

    std::cout << "Greatest magnitude possible with a sum of two is " << greatest << '\n';
}

int main()
{
    // test();
    part1("input18");
    part2("input18");

    return 0;
}