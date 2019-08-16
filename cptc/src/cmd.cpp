#include "cpt/io.hpp"
#include "cptc/cmd.hpp"

namespace cpt { 

    std::string ProgramValidator::operator()(const std::string& program_str){
        Path program(program_str);

        if(!program.is_file()){
            return program_str + " program doesn't exist";
        }

        return "";
    }

    std::string DirValidator::operator()(const std::string& dir_str){
        Path dir(dir_str);

        if(dir.empty()){
            return "";
        }

        if(!dir.is_dir()){
            return dir_str + " test directory doesn't exist";
        }

        return "";
    }


    std::string NumTestsValidator::operator()(const std::string& num_tests_str){
        int num_tests = std::stoi(num_tests_str);

        if(num_tests < 1){
            return "Number of tests must be greater than zero";
        }

        return "";
    }

}