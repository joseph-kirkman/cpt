#ifndef CPT_TEST_HPP
#define CPT_TEST_HPP

#include "cpt/io.hpp"

namespace cpt {

    class Test {
    public:
        Test(const std::string& program, const std::string& input, const std::string& output);
        bool passed() const;
        void run();
        std::string& result();
        const std::string& result() const;
        std::string& answer();
        const std::string& answer() const;
    private:
        Path        program_;
        Path        input_;
        Path        output_;
        std::string result_;
        std::string answer_;
    };
}

#endif //CPT_TEST_HPP