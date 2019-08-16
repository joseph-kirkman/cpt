#ifndef CPTC_CMD_HPP
#define CPTC_CMD_HPP

#include <string>

namespace cpt {
    struct ProgramValidator {
        std::string operator()(const std::string& program_str);
    };

    struct DirValidator {
        std::string operator()(const std::string& dir_str);
    };

    struct NumTestsValidator {
        std::string operator()(const std::string& num_tests_str);
    };    
}

#endif //CPTC_CMD_HPP