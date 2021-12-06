#include "../utility.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <set>

void part1()
{
    std::cout << "Part 1\n";
    using std::vector;

    std::fstream input("input4");

    // First line is the random number sequence
    std::string randomSequence;
    getline(input, randomSequence);
    vector<std::string> sequence{ Utility::splitString(randomSequence, ",") };

    // Rest of the body contains the bingo boards
    
    // Vector of vectors of strings
    // boardStrings[i] represents the board at i
    vector<vector<std::string>> boardStrings;
    
    std::string read;
    size_t index{ 0 };
    getline(input, read); // throw away first read
    boardStrings.push_back(std::vector<std::string>{}); // first entry
    while(getline(input, read))
    {
        if (read == "")
        {
            index++;
            boardStrings.push_back(std::vector<std::string>{});
        }
        else
        {
            boardStrings.at(index).push_back(read);
        }
    }

    input.close();

    using Board = vector<vector<std::string>>;

    vector<Board> boards;

    for (auto boardString : boardStrings)
    {
        Board board;
        for (auto lineString : boardString)
        {
            vector<std::string> result{ Utility::splitString(lineString) };
            board.push_back(Utility::splitString(lineString));
        }
        boards.push_back(board);
    }

    size_t winner_index{};
    std::string lastCalled;
    bool winnerFound{ false };
    for (std::string bingoInput : sequence)
    {
        lastCalled = bingoInput;
        for (size_t boardIndex{}; Board &board : boards)
        {
            // For each board, mark if input is found with an asterisk
            for (vector<std::string> &line : board)
            {
                for (std::string &checking : line)
                {
                    if (checking == bingoInput)
                    {
                        checking += "*";//checking.push_back('*');
                    }
                }
            }
            
            // Next for each board, somehow check if there is a winner
            
            // check each horizontal line for a bingo
            for (size_t y{}; y < board.size(); y++)
            {
                size_t marked{};
                for (auto str : board[y])
                {
                    if (str.find('*') != std::string::npos)
                        marked++;
                }
                if (marked == board[y].size())
                {
                    winner_index = boardIndex;
                    winnerFound = true;
                    break;
                }
            }

            // check each vertical line for a bingo
            for (size_t x{}; x < board[0].size(); x++)
            {
                size_t marked{};
                for (size_t y{}; y < board.size(); y++)
                {
                    if (board[y][x].find('*') != std::string::npos)
                        marked++;
                }
                if (marked == board.size())
                {
                    winner_index = boardIndex;
                    winnerFound = true;
                    break;
                }
            }

            // Finally, check diagonals
            // NEVERMIND APPARENTLY WE AREN'T
            // size_t marked1{};
            // size_t marked2{};
            // for (size_t y{}; y < board.size(); y++)
            // {
            //     size_t x1{ y };
            //     size_t x2{ board.size() - y - 1};
            //     if (board[y][x1].find('*') != std::string::npos)
            //         marked1++;
            //     if (board[y][x2].find('*') != std::string::npos)
            //         marked2++;
            // }
            // if ((marked1 == board[0].size()) || (marked2 == board[0].size()))
            // {
            //     winner_index = boardIndex;
            //     winnerFound = true;
            //     std::cout << marked1 << ' ' << marked2 << '\n';
            //     break;
            // }

            if (winnerFound) break;

            boardIndex++;
        }
        if (winnerFound) break;
    }

    Board winningBoard = boards[winner_index];
    std::cout << "Board number " << winner_index << " got bingo!\n";
    int boardSum{};
    int lastCalledNum{ std::stoi(lastCalled) };
    for (auto e : winningBoard)
    {
        for (auto a : e)
        {
            if (a.find('*') == a.npos)
            {
                boardSum += std::stoi(a);
            }
        }
    }
    for (auto e : winningBoard)
    {
        for (auto a : e)
        {
            std::cout << a << '\t';
        }
        std::cout << '\n';
    }

    printf("Unmarked sum: %d, Last called number: %d, Final score: %d\n", boardSum, lastCalledNum, boardSum * lastCalledNum);
}

// We now want to find the LAST board that wins
void part2()
{
    std::cout << "Part 2\n";

    using std::vector;

    std::fstream input("input4");

    // First line is the random number sequence
    std::string randomSequence;
    getline(input, randomSequence);
    vector<std::string> sequence{ Utility::splitString(randomSequence, ",") };

    // Rest of the body contains the bingo boards
    
    // Vector of vectors of strings
    // boardStrings[i] represents the board at i
    vector<vector<std::string>> boardStrings;
    
    std::string read;
    size_t index{ 0 };
    getline(input, read); // throw away first read
    boardStrings.push_back(std::vector<std::string>{}); // first entry
    while(getline(input, read))
    {
        if (read == "")
        {
            index++;
            boardStrings.push_back(std::vector<std::string>{});
        }
        else
        {
            boardStrings.at(index).push_back(read);
        }
    }

    input.close();

    using Board = vector<vector<std::string>>;

    vector<Board> boards;

    for (auto boardString : boardStrings)
    {
        Board board;
        for (auto lineString : boardString)
        {
            vector<std::string> result{ Utility::splitString(lineString) };
            board.push_back(Utility::splitString(lineString));
        }
        boards.push_back(board);
    }

    size_t winner_index{};
    std::string lastCalled;
    bool winnerFound{ false };
    std::set<size_t> wonIndexes;
    for (std::string bingoInput : sequence)
    {
        lastCalled = bingoInput;
        for (size_t boardIndex{}; Board &board : boards)
        {
            // skip if already won
            auto find = std::find(wonIndexes.begin(), wonIndexes.end(), boardIndex);
            if (find == wonIndexes.end())
            {
                // For each board, mark if input is found with an asterisk
                for (vector<std::string> &line : board)
                {
                    for (std::string &checking : line)
                    {
                        if (checking == bingoInput)
                        {
                            checking += "*";//checking.push_back('*');
                        }
                    }
                }
                
                // Next for each board, somehow check if there is a winner
                
                // check each horizontal line for a bingo
                for (size_t y{}; y < board.size(); y++)
                {
                    size_t marked{};
                    for (auto str : board[y])
                    {
                        if (str.find('*') != std::string::npos)
                            marked++;
                    }
                    if (marked == board[y].size())
                    {
                        wonIndexes.insert(boardIndex);
                        // std::cout << "Board " << boardIndex << " got bingo\n";
                        winner_index = boardIndex;
                        winnerFound = (wonIndexes.size() == boards.size());
                        break;
                    }
                }

                // check each vertical line for a bingo
                for (size_t x{}; x < board[0].size(); x++)
                {
                    size_t marked{};
                    for (size_t y{}; y < board.size(); y++)
                    {
                        if (board[y][x].find('*') != std::string::npos)
                            marked++;
                    }
                    if (marked == board.size() && (!winnerFound))
                    {
                        wonIndexes.insert(boardIndex);
                        // std::cout << "Board " << boardIndex << " got bingo\n";
                        winner_index = boardIndex;
                        winnerFound = (wonIndexes.size() == boards.size());
                        break;
                    }
                }
            }
            if (winnerFound) break;

            boardIndex++;
        }
        if (winnerFound) break;
    }

    Board winningBoard = boards[winner_index];
    std::cout << "Board number " << winner_index << " finally got bingo!\n";
    int boardSum{};
    int lastCalledNum{ std::stoi(lastCalled) };
    for (auto e : winningBoard)
    {
        for (auto a : e)
        {
            if (a.find('*') == a.npos)
            {
                boardSum += std::stoi(a);
            }
        }
        // std::cout << '\n';
    }
    for (auto e : winningBoard)
    {
        for (auto a : e)
        {
            std::cout << a << '\t';
        }
        std::cout << '\n';
    }

    printf("Unmarked sum: %d, Last called number: %d, Final score: %d\n", boardSum, lastCalledNum, boardSum * lastCalledNum);
}

int main()
{
    part1();
    part2();

    return 0;
}