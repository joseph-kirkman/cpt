#include <algorithm>

#include "cpt/io.hpp"
#include "cpt/string.hpp"

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

    TestsValidator::TestsValidator(Range& range): range_(range){}

    std::string TestsValidator::operator()(const std::string& tests_str){
        auto ranges_str = String::split(tests_str, ',');

        for(auto& range_str: ranges_str){
            String::trim(range_str);
            auto range_atom = String::split(range_str, ':');

            if(range_atom.size() == 1){
                int range = std::stoi(range_atom[0]);
                range_.add(range);
            } else if(range_atom.size() == 2){
                const int len = 2;
                int range[len] = {0};

                std::transform(range_atom.begin(), range_atom.end(), range,
                              [](const std::string& atom){ return std::stoi(atom); });
                
                if(!std::all_of(range, range + len, [](int n){ return n > 0; })){
                    return "Range must be greater than zero";
                }

                if(!std::is_sorted(range, range + len)){
                    return "Range error a > b";
                }

                range_.add(range[0], range[1]);
            } else {
                return "Invalid range " + range_str;
            }
        }

        return "";
    }

    
    std::string MinTestsPerThreadValidator::operator()(const std::string& tests_per_thread_str){
        int tests_per_thread = std::stoi(tests_per_thread_str);

        if(tests_per_thread < 1){
            return "Number of tests per thread must be greater than zero";
        }

        return "";
    }
}