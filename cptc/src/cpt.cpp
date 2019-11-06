#include <algorithm>
#include <iostream>
#include <vector>

#include <CLI11.hpp>
#include <cpt/cpt.hpp>
#include <cptc/cptc.hpp>

int main(int argc, char** argv){
    CLI::App app("Competitive Programming Tester", "cptc");

    try {
        auto test_info = cpt::TestInfo()
                            .withInput(cpt::Path("i.txt"))
                            .withOutput(cpt::Path("o.txt"));
        cpt::Path program;
        cpt::Path dir = cpt::Path::current_path();
        int num_tests = 1;
        bool silent = false;

        std::string tests_str;
        cpt::Range tests_range;

        cpt::TestFabric test_fabric;
        int min_tests_per_thread = 3;
        bool single_thread = false;

        app.add_option("program", program, "Program for testing")
                      ->required(true)->check(cpt::ProgramValidator());
        
        app.add_option("-i,--input", test_info.input, "Input file (i.txt by default)");
        
        app.add_option("-o,--output", test_info.output, "Output file (o.txt by default)");
        
        app.add_option("-d,--dir", dir, "Directory with tests (current directory by default)")
                      ->check(cpt::DirValidator());
        
        app.add_option("-n,--num", num_tests,
                      "Number of tests (i[1..n].txt and o[1..n].txt files will be used)")
                      ->check(cpt::NumTestsValidator());

        app.add_flag("-s, --silent", silent, "Do not print additional test info");

        app.add_option("-t,--tests", tests_str, "Tests ranges divided by comma (a:b, n, ...)")
                      ->check(cpt::TestsValidator(tests_range));
        
        app.add_flag("-e,--time", test_fabric.timer, "Elapse test's time execution");

        app.add_option("-m,--min-per-thread", min_tests_per_thread,
                      "Minimum number of tests per thread (3 by default)")
                      ->check(cpt::MinTestsPerThreadValidator());
        
        app.add_flag("--single-thread", single_thread, "All tests are executed on a single thread");

        app.parse(argc, argv);

        test_info.input = dir / test_info.input;
        test_info.output = dir / test_info.output;

        if(tests_range.empty()){
            tests_range.add(1, num_tests);
        } else {
            num_tests = tests_range.size();
        }

        test_info.program = cpt::Program::fromFile(program);
        test_info.program->init();

        cpt::Console::print("Program = ");
        cpt::Console::println(program, cpt::Console::Color::blue);
        cpt::Console::print("Number of tests = ");
        cpt::Console::println(num_tests, cpt::Console::Color::blue);
        cpt::Console::print("Tests directory = ");
        cpt::Console::println(dir, cpt::Console::Color::blue);
        cpt::Console::println();

        int num_threads = 1;
        min_tests_per_thread = std::min(num_tests, min_tests_per_thread);

        if(!single_thread){
            int max_threads = (num_tests - 1) / min_tests_per_thread + 1;
            int hardware_threads = std::thread::hardware_concurrency();
            num_threads = std::min(hardware_threads > 0 ? hardware_threads : 2, max_threads);  
        }

        cpt::MultiTest mt(test_info, test_fabric);
        mt.run(tests_range, num_threads);

        int tests_done = 0;
        int succeed = 0;
        std::set<int> failed;

        while(tests_done < num_tests){
            auto result = mt.get();
            auto test = result.test;
            ++tests_done;

            cpt::Console::print_all(result.index, ": ");

            if(test->passed()){
                ++succeed;
                cpt::Console::println("OK", cpt::Console::Color::green);
                cpt::Console::println();
            } else {
                failed.insert(result.index);
                cpt::Console::println("FAILED", cpt::Console::Color::red);
                cpt::Console::println();

                if(!silent){
                    cpt::Console::println("Got:");
                    cpt::Console::println(test->result(), cpt::Console::Color::blue);
                    cpt::Console::println();
                    cpt::Console::println("Expected:");
                    cpt::Console::println(test->answer(), cpt::Console::Color::blue);
                    cpt::Console::println();
                }
            }

            if(!silent && test_fabric.timer){
                cpt::Console::print("Time = ");
                cpt::Console::print(
                    static_cast<cpt::Timeable<cpt::Test>*>(test.get())->time(),
                    cpt::Console::Color::blue
                );
                cpt::Console::println("ms", cpt::Console::Color::blue);
                cpt::Console::println();
            }
        }

        cpt::Console::print("Succeeded = ");
        cpt::Console::println(std::to_string(succeed) + "/" + std::to_string(tests_done), cpt::Console::Color::blue);
        
        if(!failed.empty()){
            cpt::Console::print("Failed = ");

            for(auto& idx: failed){
                cpt::Console::print(std::to_string(idx) + " ", cpt::Console::Color::blue);
            }

            cpt::Console::println();
        }

    } catch(const CLI::Error& e){
        return app.exit(e);
    } catch(const std::exception& e){
        std::cout << app.get_name() << ": " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

