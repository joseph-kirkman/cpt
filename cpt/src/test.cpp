#include "cpt/string.hpp"
#include "cpt/system.hpp"
#include "cpt/test.hpp"

//TestOptions
namespace cpt {

    TestOptions::TestOptions():
        timer(false){}

    TestOptions::TestOptions(bool timer):
        timer(timer){}
}

//Test
namespace cpt {

    Test::Test(const Path& program, const Path& input, const Path& output):
        program_(program), input_(input), output_(output){

        if(!program_.is_file()){
            throw std::runtime_error(program.str() + " file doesn't exist");
        }

        if(!input_.is_file()){
            throw std::runtime_error(input.str() + " file doesn't exist");
        }

        if(!output_.is_file()){
            throw std::runtime_error(output.str() + " file doesn't exist");
        }

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
        result_ = System::execute(System::redirect_input(program_, input_));
        answer_ = File::read(output_);
        String::trim(result_);
        String::trim(answer_);
    }

    const Path& Test::program() const {
        return program_;
    }

    const Path& Test::input() const {
        return input_;
    }

    const Path& Test::output() const {
        return output_;
    }

    const std::string& Test::result() const {
        return result_;
    }

    const std::string& Test::answer() const {
        return answer_;
    }

}