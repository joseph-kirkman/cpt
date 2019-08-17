#include <vector>
#include <sstream>
#include "cpt/string.hpp"

namespace cpt {

    std::vector<std::string> String::split(const std::string& str, char delim){
        std::vector<std::string> tokens;

        size_t last = 0;
        size_t pos = str.find_first_of(delim, last);

        while (last != std::string::npos) {
            if (pos != last)
                tokens.emplace_back(str.substr(last, pos - last));
            last = pos;
            if (last == std::string::npos || last + 1 == str.length())
                break;
            pos = str.find_first_of(delim, ++last);
        }

        return tokens;
    }

    void String::trim_left(std::string& str){
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](char c){
            return !std::isspace(c);
        }));
    }

    void String::trim_right(std::string& str){
        str.erase(std::find_if(str.rbegin(), str.rend(), [](char c){
            return !std::isspace(c);
        }).base(), str.end());
    }

    void String::trim(std::string& str){
        trim_left(str);
        trim_right(str);
    }
}