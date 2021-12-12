#include "../utility.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>

bool isUpperCase(const std::string &str)
{
    for (char character : str)
        if (character > 'Z') return false;
    return true;
}

struct Node
{
    std::string id;
    std::vector<std::string> neighbors;
};

// Adds node if it doesnt exist already
void addNode(const std::string &id, std::vector<Node> &list)
{
    for (Node &node : list)
        if (node.id == id) return;
    list.push_back(Node{id});
}

Node& getNodeById(const std::string &id, std::vector<Node> &list)
{
    for (size_t index = 0; Node &node : list)
    {
        if (node.id == id) return list[index];
        index++;
    }
    assert(false && "Unreachable");
}

std::vector<Node> generateNodes(const std::string &fileName)
{
    std::fstream inputs(fileName);
    std::string readline;

    std::vector<Node> result;
    while (getline(inputs, readline))
    {
        std::vector<std::string> split = Utility::splitString(readline, "-");
        for (std::string &id : split) addNode(id, result);
        // should be two inputs
        Node &first = getNodeById(split[0], result);
        Node &second = getNodeById(split[1], result);
        first.neighbors.push_back(second.id);
        second.neighbors.push_back(first.id);
    }
    inputs.close();

    return result;
}

std::vector<std::string> traverseNodes(const Node &current, std::string path, std::vector<Node> &nodes)
{
    if (current.id == "end") return {path + "end"};
    path += current.id + ',';

    std::vector<std::string> result;
    for (const std::string &neighbor : current.neighbors)
    {
        if (neighbor == "start") continue;
        else if (!(isUpperCase(neighbor)) && (path.find(neighbor) != std::string::npos)) continue;
        std::vector<std::string> retrieved = traverseNodes(getNodeById(neighbor, nodes), path, nodes);
        result.insert(result.end(), retrieved.begin(), retrieved.end());
    }

    return result;
}

std::vector<std::string> traverseNodesUpdated(const Node &current, std::string path, bool backTracked, std::vector<Node> &nodes)
{
    if (current.id == "end") return {path + "end"};
    if (!backTracked && (!isUpperCase(current.id)))
    {
        size_t pos = path.find(current.id);
        if (pos != std::string::npos && path.find(current.id, pos) != std::string::npos)
            backTracked = true;
    }
    path += current.id + ',';

    std::vector<std::string> result;
    for (const std::string &neighbor : current.neighbors)
    {
        if (neighbor == "start") continue;
        else if (!(isUpperCase(neighbor)) && (path.find(neighbor) != std::string::npos))
        {
            if (backTracked) continue;
        }
        std::vector<std::string> retrieved = traverseNodesUpdated(getNodeById(neighbor, nodes), path, backTracked, nodes);
        result.insert(result.end(), retrieved.begin(), retrieved.end());
    }

    return result;
}

void doProblem()
{
    std::vector<Node> nodes = generateNodes("input12");

    Node &start = getNodeById("start", nodes);

    std::vector<std::string> paths = traverseNodes(start, "", nodes);
    std::vector<std::string> pathsUpdated = traverseNodesUpdated(start, "", false, nodes);

    std::cout << "Paths (part 1): " << paths.size() << '\n';
    std::cout << "Updated paths (part 2): " << pathsUpdated.size() << '\n';
}

int main()
{
    doProblem();

    return 0;
}