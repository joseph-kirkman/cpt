#include "cpt/string.hpp"
#include "cpt/system.hpp"
#include "cpt/test.hpp"

namespace cpt {

    Test::Test(const std::string& program, const std::string& input, const std::string& output):
        program_(program), input_(input), output_(output){

        if(!program_.exists()){
            throw std::runtime_error(program + " file doesn't exist");
        }

        if(!input_.exists()){
            throw std::runtime_error(input + " file doesn't exist");
        }

        if(!output_.exists()){
            throw std::runtime_error(output + " file doesn't exist");
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

    std::string& Test::result(){
        return result_;
    }

    const std::string& Test::result() const {
        return result_;
    }

    std::string& Test::answer() {
        return answer_;
    }

    const std::string& Test::answer() const {
        return answer_;
    }

}