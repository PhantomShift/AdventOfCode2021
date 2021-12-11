#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <cassert>
#include <algorithm>

const std::string toRead{ "input10" };

bool isOpenBracket(char bracket)
{
    switch(bracket)
    {
    case('('):
    case('['):
    case('{'):
    case('<'):
        return true;
    default: return false;
    }
}

char getClosingBracket(char openBracket)
{
    switch(openBracket)
    {
    case('('): return ')';
    case('['): return ']';
    case('{'): return '}';
    case('<'): return '>';
    }
    assert(false && "Unreachable");
}

int getScore(char character)
{
    switch(character)
    {
    case(')'): return 3;
    case(']'): return 57;
    case('}'): return 1197;
    case('>'): return 25137;
    }
    assert(false && "Unreachable");
}

// returns 0 if valid character, else the score of the invalid character
// pushes to stack if open bracket, pops if valid closing bracket
int validateCharacter(char character, std::stack<char> &characters)
{
    if (isOpenBracket(character))
    {
        characters.push(character);
        return 0;
    }
    else
    {
        if (character == getClosingBracket(characters.top())) {characters.pop(); return 0;}
        else return getScore(character); // line is corrupted
    }
}

void part1()
{
    printf("Part 1\n");

    std::fstream inputs(toRead);
    std::string readline;

    int score = 0;
    while (getline(inputs, readline))
    {
        std::stack<char> characters;

        for (char character : readline)
        {
            int charScore = validateCharacter(character, characters);
            if (charScore > 0)
            {
                score += charScore;
                break;
            }
        }
    }
    inputs.close();

    printf("The total score is %d\n", score);
}

long long getAutocompleteScore(long long currentScore, char character)
{
    currentScore = currentScore * 5;
    switch(character)
    {
    case(')'): currentScore += 1; break;
    case(']'): currentScore += 2; break;
    case('}'): currentScore += 3; break;
    case('>'): currentScore += 4; break;
    }
    return currentScore;
}

void part2()
{
    printf("Part 2\n");

    std::fstream inputs(toRead);
    std::string readline;

    std::vector<long long> scores;
    while (getline(inputs, readline))
    {
        std::stack<char> characters;
        bool isValid = true;
        for (char character : readline)
        {
            int charScore = validateCharacter(character, characters);
            if (charScore > 0)
            {
                isValid = false;
                break;
            }
        }
        if (isValid)
        {
            // Characters that are still in the stack need to be corrected
            long long score = 0; // As it turns out, the scores get REALLY big.
            while (characters.size() > 0U)
            {
                char character = characters.top();
                score = getAutocompleteScore(score, getClosingBracket(character));
                characters.pop();
            }
            scores.push_back(score);
        }
    }
    inputs.close();

    std::sort(scores.begin(), scores.end());

    long long midScore = scores[scores.size() / 2U];

    printf("The middle score is %lld\n", midScore);
}

int main()
{
    part1();
    part2();

    return 0;
}