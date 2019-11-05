#ifndef CPT_TEST_HPP
#define CPT_TEST_HPP

#include <memory>
#include <thread>
#include <queue>

#include "cpt/io.hpp"
#include "cpt/mixins.hpp"
#include "cpt/range.hpp"

namespace cpt {

    class Test {
    public:

        class Info {
        public:
            Info()=default;
            Info(const Path& program, const Path& input, const Path& output);
        public:
            Path program;
            Path input;
            Path output;        
        };

        class Options {
        public:
            Options();
            Options(bool timer);

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

    public:
        Test()=default;
        Test(const Info& info);
        Test(const Path& program, const Path& input, const Path& output);
        virtual ~Test()=default;
        bool passed() const;
        virtual void run();
        const Info& info() const;
        const std::string& result() const;
        const std::string& answer() const;
    private:
        void check_info();
    private:
        Info        info_;
        std::string result_;
        std::string answer_;
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
        MultiTest(const Test::Info& info, const Test::Options& ops);
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
        Test::Info              info_;
        Test::Options           ops_;
    };
}

#endif //CPT_TEST_HPP