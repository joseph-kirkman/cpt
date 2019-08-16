#include <vector>
#include <sstream>
#include "cpt/string.hpp"

namespace cpt {

    std::vector<std::string> String::split(const std::string& str, char delim){
        std::vector<std::string> tokens;
        std::istringstream str_stream(str);
        std::string token;

        while(std::getline(str_stream, token, delim)){
            if(!token.empty()){
                tokens.emplace_back(std::move(token));
            }
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