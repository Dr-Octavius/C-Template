#include <algorithm>
#include <cctype>

#include "QueryParser.h"

// ai-gen start
std::shared_ptr<std::vector<std::string>>
QueryParserUtil::splitString(const std::string &str, char delimiter) {
    auto substrings = std::make_shared<std::vector<std::string>>();

    size_t start = 0;
    size_t end = str.find(delimiter);

    // Iterate through the string
    while (end != std::string::npos) {
        // Extract substring and add to the vector
        substrings->push_back(str.substr(start, end - start));
        // Update start position for the next iteration
        start = end + 1;
        // Find next occurrence of the delimiter
        end = str.find(delimiter, start);
    }

    // Add the last substring (after the last delimiter)
    substrings->push_back(str.substr(start));

    return substrings;
}
// ai-gen end

// ai-gen start(gpt, 0, e)
// prompt: https://platform.openai.com/playground/p/pRI5b3kZbrbYCRekyRMOjYRr?model=gpt-4&mode=chat
std::string QueryParserUtil::removeWhitespace(const std::string& str) {
    std::string result;
    std::copy_if(str.begin(), str.end(), std::back_inserter(result),
                 [](char c){ return !std::isspace(static_cast<unsigned char>(c)); });
    return result;
}
// ai-gen end
