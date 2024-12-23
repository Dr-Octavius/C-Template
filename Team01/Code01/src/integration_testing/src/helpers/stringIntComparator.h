#ifndef STRING_INT_COMPARATOR
#define STRING_INT_COMPARATOR

// ai-gen start(gpt, 1, e)
// prompt: -
#include <string>
#include <cctype> // For std::isdigit
#include <algorithm> // For std::all_of

// Check if a string represents an integer number
inline bool isNumber(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

// Check if a string is a single alphabet letter
inline bool isSingleLetter(const std::string& s) {
    return s.length() == 1 && std::isalpha(s[0]);
}

// Comprehensive comparator
inline bool stringIntComparator(const std::string& str1, const std::string& str2) {
    // Numbers first
    bool isNum1 = isNumber(str1), isNum2 = isNumber(str2);
    if (isNum1 && !isNum2) return true;
    if (!isNum1 && isNum2) return false;
    if (isNum1 && isNum2) return std::stoi(str1) < std::stoi(str2);

    // Then single letters
    bool isLetter1 = isSingleLetter(str1), isLetter2 = isSingleLetter(str2);
    if (isLetter1 && !isLetter2) return true;
    if (!isLetter1 && isLetter2) return false;
    if (isLetter1 && isLetter2) return str1 < str2;

    // Words last
    return str1 < str2;
}

// ai-gen end

#endif // STRING_INT_COMPARATOR