#ifndef CPTC_CMD_HPP
#define CPTC_CMD_HPP

#include <string>
#include "cpt/range.hpp"

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

    struct TestsValidator {
        TestsValidator(Range& range);
        std::string operator()(const std::string& tests_str);
    private:
        Range& range_;
    };

    struct MinTestsPerThreadValidator {
        std::string operator()(const std::string& tests_per_thread_str);
    };

    struct BinDirValidator {
        std::string operator()(const std::string& bin_dir_str);
    };
}

#endif //CPTC_CMD_HPP