#ifndef A_STAR_H_INCLUDED
#define A_STAR_H_INCLUDED

#include <vector>
#include <limits>
#include <list>

namespace A_Star
{
    typedef std::pair<size_t, size_t> coordinate;

    template<typename T>
    struct node
    {
        size_t x;
        size_t y;
        T value;
        bool visited = false;
        float gscore = std::numeric_limits<float>::max();
        float lscore = std::numeric_limits<float>::max();
        node<T>* parent = nullptr;

        bool operator<(const node<T> other) const;
        bool operator>(const node<T> other) const;
        bool operator==(const node<T> other) const;
    };

    const size_t maxSizeT = std::numeric_limits<size_t>::max();
    static constexpr coordinate out_of_bounds {maxSizeT, maxSizeT};

    template <typename T>
    struct map
    {
        std::vector<std::vector<node<T>>> nodes{};

        std::vector<node<T>*> getNeighbors(node<T> _node);
    };

    template <typename T>
    float defaultHeuristic(node<T> a, node<T> b);

    template <typename T>
    std::list<coordinate> solve(map<T> &nodemap, node<T> &start, node<T> &end, float (&distance) (node<T>, node<T>), float heuristic (node<T>, node<T>)=defaultHeuristic);
} // namespace A_Star

#include "astar.tpp"

#endif