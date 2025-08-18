#ifndef CPT_STRING_HPP
#define CPT_STRING_HPP

#include <string>

namespace cpt {
    class String {
    public:
        static std::vector<std::string> split(const std::string& str, char delim);
        static void trim_left(std::string& str);
        static void trim_right(std::string& str);
        static void trim(std::string& str);
        static std::string trim_left(const std::string& str);
        static std::string trim_right(const std::string& str);
        static std::string trim(const std::string& str);
    };
}

#endif //CPT_STRING_HPP