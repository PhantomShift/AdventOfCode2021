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
}

#endif