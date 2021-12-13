#ifndef UTILITY_INCLUDED
#define UTILITY_INCLUDED

#include <vector>
#include <string>
#include <chrono>
#include <limits>

namespace Utility
{
    std::vector<std::string> splitString(const std::string &input, const std::string &delimiter=" ", bool ignoreEmpty=true)
    {
        std::vector<std::string> result;
        size_t inputLength{ input.length() };
        size_t delimiterLength{ delimiter.length() };
        result.reserve(inputLength / 4);
        
        size_t prevIndex{ 0 };
        size_t index{ input.find(delimiter, prevIndex) };
        do
        {
            std::string found{ input.substr(prevIndex, index - prevIndex) };
            // printf("%s ", found.c_str());
            if (found.length() > 0 || (!ignoreEmpty))
                result.push_back(found);
            prevIndex = index + delimiterLength;
            index = input.find(delimiter, prevIndex);
        } while (index != std::string::npos);
        // requires one more push
        result.push_back(input.substr(prevIndex, index - prevIndex));

        return result;
    }

    // From https://stackoverflow.com/a/55838758
    // Ended up not using it but still seems pretty useful;
    template <class T>
    class VectorProxy
    {
        std::vector<T> &v1;
        std::vector<T> &v2;
    public:
        VectorProxy(std::vector<T> &ref1, std::vector<T> &ref2) :
            v1{ ref1 }, v2{ ref2 } {}
        
        const T& operator[](const size_t i) const {
            return (i < v1.size() ? v1[i] : v2[i - v1.size()]);
        }

        const size_t size() const {
            return v1.size() + v2.size();
        }
    };


    // From https://www.learncpp.com/cpp-tutorial/timing-your-code/
    class Timer
    {
    private:
        // Type aliases to make accessing nested type easier
        using clock_type = std::chrono::steady_clock;
        using second_type = std::chrono::duration<double, std::ratio<1> >;

        std::chrono::time_point<clock_type> m_beg;

    public:
        Timer() : m_beg { clock_type::now() }
        {
        }

        void reset()
        {
            m_beg = clock_type::now();
        }

        double elapsed() const
        {
            return std::chrono::duration_cast<second_type>(clock_type::now() - m_beg).count();
        }
    };


    template <typename T>
    size_t vectorFind(std::vector<T> &vector, T &toFind, size_t start=0U)
    {
        for (size_t index = start; index < vector.size(); index++)
            if (vector[index] == toFind) return index;

        return vector.size();
    }

    template <typename T>
    size_t vectorRfind(std::vector<T> &vector, T &toFind, size_t start=std::numeric_limits<size_t>::max())
    {
        start = (vector.size() > start ? start  : vector.size() - 1);
        for (size_t index = start; index >= 0; index--)
            if (vector[index] == toFind) return index;
        
        return vector.size();
    }
}

#endif