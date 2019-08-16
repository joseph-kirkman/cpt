#ifndef CPT_TEST_HPP
#define CPT_TEST_HPP

#include "cpt/io.hpp"

namespace cpt {

    class Test {
    public:
        Test()=default;
        Test(const Path& program, const Path& input, const Path& output);
        bool passed() const;
        void run();
        const Path& program() const;
        const Path& input() const;
        const Path& output() const;
        const std::string& result() const;
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