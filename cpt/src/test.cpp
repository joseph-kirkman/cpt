#include "cpt/string.hpp"
#include "cpt/system.hpp"
#include "cpt/test.hpp"

//Test::Options
namespace cpt {

    Test::Options::Options():
        timer(false){}

    Test::Options::Options(bool timer):
        timer(timer){}
}

//Test::Info
namespace cpt {
    Test::Info::Info(const Path& program, const Path& input, const Path& output):
        program(program), input(input), output(output){}
}

//Test
namespace cpt {

    Test::Test(const Test::Info& info): info_(info){
        check_info();
    }

    Test::Test(const Path& program, const Path& input, const Path& output):
        info_(program, input, output){
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
        result_ = System::execute(System::redirect_input(info_.program, info_.input));
        answer_ = File::read(info_.output);
        String::trim(result_);
        String::trim(answer_);
    }

    const Test::Info& Test::info() const {
        return info_;
    }

    const std::string& Test::result() const {
        return result_;
    }

    const std::string& Test::answer() const {
        return answer_;
    }

    void Test::check_info(){
        if(!info_.program.is_file()){
            throw std::runtime_error(info_.program.str() + " file doesn't exist");
        }

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

    MultiTest::MultiTest(const Test::Info& info, const Test::Options& ops):
        info_(info), ops_(ops){}

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

        for(size_t i = 0; i < num_threads - 1; ++i){
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
                ops_.create(
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