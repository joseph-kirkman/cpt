#include "cpt/string.hpp"
#include "cpt/system.hpp"
#include "cpt/test.hpp"

//TestFabric
namespace cpt {

    TestFabric::TestFabric():
        timer(false){}

    TestFabric& TestFabric::withTimer(bool timer){
        this->timer = timer;
        return *this;
    }
}

//TestInfo
namespace cpt {

    TestInfo& TestInfo::withProgram(const std::shared_ptr<Program>& program){
        this->program = program;
        return *this;
    }

    TestInfo& TestInfo::withInput(const Path& input){
        this->input = input;
        return *this;
    }

    TestInfo& TestInfo::withOutput(const Path& output){
        this->output = output;
        return *this;
    }
}

//Test
namespace cpt {

    Test::Test(const TestInfo& info):
        info_(info){
        check_info();
    }

    Test::Test(const std::shared_ptr<Program>& program, const Path& input, const Path& output){
        info_ = TestInfo().withProgram(program).withInput(input).withOutput(output);
        check_info();
    }

    bool Test::passed() const {
        auto result_v = String::split(result_, '\n');
        auto answer_v = String::split(answer_, '\n');

        if(result_v.size() != answer_v.size())
            return false;

        for(size_t i = 0; i < result_v.size(); ++i){
            String::trim(result_v[i]);
            String::trim(answer_v[i]);

            if(result_v[i] != answer_v[i])
                return false;
        }

        return true;
    }

    void Test::run(){
        result_ = info_.program->run(info_.input);
        answer_ = File::read(info_.output);
        String::trim(result_);
        String::trim(answer_);
    }

    const TestInfo& Test::info() const {
        return info_;
    }

    const std::string& Test::result() const {
        return result_;
    }

    const std::string& Test::answer() const {
        return answer_;
    }

    void Test::check_info(){

        if(!info_.input.is_file()){
            throw std::runtime_error(info_.input.str() + " file doesn't exist");
        }

        if(!info_.output.is_file()){
            throw std::runtime_error(info_.output.str() + " file doesn't exist");
        }
    }
}

//MultiTest
namespace cpt {

    MultiTest::Result::Result():
        index(0), test(nullptr){}

    MultiTest::Result::Result(int index, const std::shared_ptr<Test>& test):
        index(index), test(test){}

    MultiTest::MultiTest(const TestInfo& info, const TestFabric& fabric):
        info_(info), fabric_(fabric){}

    void MultiTest::run(const Range& tests_range, int num_threads){
        int block_size = tests_range.size() / num_threads;
        auto block_start = tests_range.cbegin();
        auto run_thread = [&](typename Range::const_iterator begin,
                              typename Range::const_iterator end){
            std::thread thread(
                &MultiTest::process_range,
                this, 
                begin,
                end
            );
            thread.detach();
        };

        for(int i = 0; i < num_threads - 1; ++i){
            auto block_end = block_start;
            std::advance(block_end, block_size);
            run_thread(block_start, block_end);
            block_start = block_end;
        }

        run_thread(block_start, tests_range.cend());
    }

    void MultiTest::process_range(typename Range::const_iterator begin,
                                  typename Range::const_iterator end){
        for(auto it = begin; it != end; ++it){
            std::string num_str = std::to_string(*it);
            std::shared_ptr<Test> test(
                fabric_.create(
                    info_.program,
                    info_.input.add_suffix(num_str),
                    info_.output.add_suffix(num_str)
                )
            );
            std::unique_lock<std::mutex> locker(mutex_);
            test->run();
            tests_.push(Result(*it, test));
            cond_.notify_one();
        }
    }

    typename MultiTest::Result MultiTest::get(){
        std::unique_lock<std::mutex> locker(mutex_);

        cond_.wait(locker, [&](){
            return !tests_.empty();
        });

        auto test_p = tests_.front();
        tests_.pop();
        return test_p;
    }
}