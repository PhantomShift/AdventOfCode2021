#include "astar.h"
#include <vector>
#include <map>
#include <limits>
#include <list>
#include <cmath>
#include <queue>

// A* algorithm implementation
// Largely inspired and educated by javidx9
// Video: https://www.youtube.com/watch?v=icZj67PTFhc

namespace A_Star
{
    typedef std::pair<size_t, size_t> coordinate;

    template <typename T>
    bool node<T>::operator<(const node<T> other) const
    {
        return (this->gscore < other.gscore);
    }
    template <typename T>
    bool node<T>::operator>(const node<T> other) const {return !(*this < other);}
    template <typename T>
    bool node<T>::operator==(const node<T> other) const
    {
        return (this->x == other.x && this->y == other.y && this->value == other.value);
    }


    template <typename T>
    std::vector<node<T>*> map<T>::getNeighbors(node<T> _node)
    {
        size_t x = _node.x;
        size_t y = _node.y;

        std::vector<node<T>*> result;
        if (y != 0) result.push_back(&nodes[y - 1][x]);
        if (x < nodes[y].size() - 1 ) result.push_back(&nodes[y][x + 1]);
        if (y < nodes.size() - 1) result.push_back(&nodes[y + 1][x]);
        if (x != 0) result.push_back(&nodes[y][x - 1]);

        return result;
    }
    
    // Simple straight path distance from a to b based on coordinates
    template <typename T>
    float defaultHeuristic(node<T> a, node<T> b)
    {
        return std::sqrt(static_cast<float>((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)));
    }

    // param distance function determines the distance between neighboring nodes
    // returns coordinates in order from start to end
    template <typename T>
    std::list<coordinate> solve(map<T> &nodemap, node<T> &start, node<T> &end, float (&distance) (node<T>, node<T>), float heuristic (node<T>, node<T>))
    {
        // nodes must be reset in case they are reused
        for (std::vector<node<T>> &row : nodemap.nodes)
        {
            for (node<T> &node : row)
            {
                node.gscore = std::numeric_limits<float>::max();
                node.lscore = std::numeric_limits<float>::max();
                node.visited = false;
                node.parent = nullptr;
            }
        }
        
        start.lscore = 0.0f;
        start.gscore = heuristic(start, end);

        std::priority_queue<node<T>, std::vector<node<T>>, std::greater<node<T>>> unchecked;
        unchecked.push(start);
        while (!unchecked.empty())
        {
            node<T> &current = nodemap.nodes[unchecked.top().y][unchecked.top().x];
            if (current == end) break;
            unchecked.pop();
            current.visited = true;

            for (node<T> *neighbor : nodemap.getNeighbors(current))
            {
                float potentialscore = current.lscore + distance(current, *neighbor);

                if (potentialscore < neighbor->lscore)
                {
                    neighbor->parent = &current;
                    neighbor->lscore = potentialscore;

                    neighbor->gscore = neighbor->lscore + heuristic(*neighbor, end);

                    unchecked.push(nodemap.nodes[neighbor->y][neighbor->x]);
                }
            }
        }

        // Recreates path as coordinates
        std::list<coordinate> path { coordinate{end.x, end.y} };
        node<T> *nodePtr = end.parent;
        while (nodePtr != nullptr)
        {
            path.push_front(coordinate{nodePtr->x, nodePtr->y});
            nodePtr = nodePtr->parent;
        }

        return path;
    }

} // namespace A_Star