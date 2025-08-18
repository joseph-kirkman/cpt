#ifndef CPT_TEST_HPP
#define CPT_TEST_HPP

#include <memory>
#include <thread>
#include <queue>

#include "cpt/config.hpp"
#include "cpt/io.hpp"
#include "cpt/mixins.hpp"
#include "cpt/range.hpp"
#include "cpt/program.hpp"

namespace cpt {

    class TestInfo {
    public:
        TestInfo()=default;
        ~TestInfo()=default;
        TestInfo& withProgram(const std::shared_ptr<Program>& program);
        TestInfo& withInput(const Path& input);
        TestInfo& withOutput(const Path& output);
    public:
        std::shared_ptr<Program> program;
        Path                     input;
        Path                     output;        
    };

    class TestFabric;

    class Test {
    public:
        Test()=default;
        explicit Test(const TestInfo& info);
        Test(const std::shared_ptr<Program>& program, const Path& input, const Path& output);
        virtual ~Test()=default;
        bool passed() const;
        virtual void run();
        const TestInfo& info() const;
        const std::string& result() const;
        const std::string& answer() const;
    private:
        void check_info();
    private:
        TestInfo    info_;
        std::string result_;
        std::string answer_;
    };

    class TestFabric {
        public:
            TestFabric();
            TestFabric& withTimer(bool timer);

            template <typename ...Args>
            Test* create(Args&&... args){
                if(timer){
                    return new Timeable<Test>(std::forward<Args>(args)...);
                }
                return new Test(std::forward<Args>(args)...);
            }

        public:
            bool timer;
    };

    class MultiTest {
    public:
        class Result {
        public:
            Result();
            Result(int index, const std::shared_ptr<Test>& test);
        public:
            int index;
            std::shared_ptr<Test> test;
        };
    public:
        MultiTest()=default;
        MultiTest(const TestInfo& info, const TestFabric& fabric);
        MultiTest(const MultiTest&)=delete;
        MultiTest& operator=(const MultiTest&)=delete;
        void run(const Range& tests_range, int num_threads);
        Result get();
    private:
        void process_range(typename Range::const_iterator begin, typename Range::const_iterator end);
    private:
        std::queue<Result>      tests_;
        std::mutex              mutex_;
        std::condition_variable cond_;
        TestInfo                info_;
        TestFabric              fabric_;
    };
}

#endif //CPT_TEST_HPP