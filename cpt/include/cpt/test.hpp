#ifndef CPT_TEST_HPP
#define CPT_TEST_HPP

#include "cpt/io.hpp"
#include "cpt/mixins.hpp"

namespace cpt {

    class TestOptions {
    public:
        TestOptions();
        TestOptions(bool timer);
    public:
        bool timer;
    };

    class Test {
    public:
        Test()=default;
        Test(const Path& program, const Path& input, const Path& output);
        virtual ~Test()=default;
        bool passed() const;
        virtual void run();
        const Path& program() const;
        const Path& input() const;
        const Path& output() const;
        const std::string& result() const;
        const std::string& answer() const;
    public:
        template <typename ...Args>
        static Test* create(const TestOptions& options, Args&&... args){
            if(options.timer){
                return new Timeable<Test>(std::forward<Args>(args)...);
            } else {
                return new Test(std::forward<Args>(args)...);
            }
        }
    private:
        Path        program_;
        Path        input_;
        Path        output_;
        std::string result_;
        std::string answer_;
    };
}

#endif //CPT_TEST_HPP