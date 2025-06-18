#pragma once

#include <string_view>
#include <string>
#include <algorithm>
#include <cctype>

class StringUtil {
public:
    static constexpr bool equals(std::string_view a, std::string_view b) {
        return a == b;
    }

    static bool equalsIgnoreCase(std::string_view a, std::string_view b) {
        return toLower(a) == toLower(b);
    }

    static constexpr bool startsWith(std::string_view str, std::string_view prefix) {
        return str.size() >= prefix.size() &&
            std::equal(prefix.begin(), prefix.end(), str.begin());
    }

    static constexpr bool endsWith(std::string_view str, std::string_view suffix) {
        return str.size() >= suffix.size() &&
            std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
    }

    static std::string toLower(std::string_view str) {
        std::string result(str);
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return result;
    }

    static std::string toUpper(std::string_view str) {
        std::string result(str);
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return std::toupper(c); });
        return result;
    }

    static std::string trim(std::string_view str) {
        auto begin = std::find_if_not(str.begin(), str.end(),
            [](unsigned char c) { return std::isspace(c); });

        auto end = std::find_if_not(str.rbegin(), str.rend(),
            [](unsigned char c) { return std::isspace(c); }).base();

        if (begin >= end)
            return "";

        return std::string(begin, end);
    }

    static bool isEmpty(std::string_view str) {
        return trim(str).empty();
    }
};
