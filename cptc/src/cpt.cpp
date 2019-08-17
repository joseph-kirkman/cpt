#include <algorithm>
#include <iostream>
#include <vector>

#include <CLI11.hpp>
#include <cpt/cpt.hpp>
#include <cptc/cptc.hpp>

int main(int argc, char** argv){
    CLI::App app("Competitive Programming Tester", "cptc");

    try {
        cpt::Path program;

        cpt::Path input("i.txt");
        cpt::Path output("o.txt");
        cpt::Path dir = cpt::Path::current_path();
        int num_tests = 1;
        bool silent = false;

        std::string tests_str;
        cpt::Range tests_range;

        app.add_option("program", program, "Program for testing")
                      ->required(true)->check(cpt::ProgramValidator());
        
        app.add_option("-i,--input", input, "Input file (i.txt by default)");
        
        app.add_option("-o,--output", output, "Output file (o.txt by default)");
        
        app.add_option("-d,--dir", dir, "Directory with tests (current directory by default)")
                      ->check(cpt::DirValidator());
        
        app.add_option("-n,--num", num_tests,
                      "Number of tests (i[1..n].txt and o[1..n].txt files will be used)")
                      ->check(cpt::NumTestsValidator());

        app.add_flag("-s, --silent", silent, "Do not print messages while testing");

        app.add_option("-t,--tests", tests_str, "Tests ranges divided by comma (a:b, n, ...)")
                      ->check(cpt::TestsValidator(tests_range));

        app.parse(argc, argv);

        input = dir / input;
        output = dir / output;

        if(tests_range.empty()){
            tests_range.add(1, num_tests);
        }

        if(!silent){
            cpt::Console::print("Program = ");
            cpt::Console::println(program.str(), cpt::Console::Color::blue);
            cpt::Console::print("Number of tests = ");
            cpt::Console::println(tests_range.size(), cpt::Console::Color::blue);
            cpt::Console::print("Tests directory = ");
            cpt::Console::println(dir, cpt::Console::Color::blue);
            cpt::Console::println();
        }

        std::vector<cpt::Test> tests;

        for(int i: tests_range){
            std::string num_str = std::to_string(i);
            tests.emplace_back(program, 
                               input.add_suffix(num_str),
                               output.add_suffix(num_str)
            );
        }

        size_t test_index = 0;

        for(int i: tests_range){
            auto test = tests[test_index++];
            test.run();
            
            cpt::Console::print_all(i, ": ");

            if(test.passed()){
                cpt::Console::println("OK", cpt::Console::Color::green);
                cpt::Console::println();
            } else {
                cpt::Console::println("FAILED", cpt::Console::Color::red);
                cpt::Console::println();

                if(!silent){
                    cpt::Console::println("Got:", cpt::Console::Color::red);
                    cpt::Console::println(test.answer());
                    cpt::Console::println();
                    cpt::Console::println("Expected:", cpt::Console::Color::green);
                    cpt::Console::println(test.result());
                    cpt::Console::println();
                }
            }
        }

    } catch(const CLI::Error& e){
        return app.exit(e);
    } catch(const std::exception& e){
        std::cout << app.get_name() << ": " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

