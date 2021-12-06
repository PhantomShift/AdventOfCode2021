#ifndef UTILITY_INCLUDED
#define UTILITY_INCLUDED

#include <vector>
#include <string>

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
}

#endif