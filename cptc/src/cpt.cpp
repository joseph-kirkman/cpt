#include <algorithm>
#include <iostream>
#include <vector>

#include <CLI11.hpp>
#include <cpt/cpt.hpp>
#include <cptc/cptc.hpp>

int main(int argc, char** argv){
    CLI::App app("Competitive Programming Tester", "cptc");

    try {
        cpt::Test::Info test_info(cpt::Path(), cpt::Path("i.txt"), cpt::Path("o.txt"));
        cpt::Path dir = cpt::Path::current_path();
        int num_tests = 1;
        bool silent = false;

        std::string tests_str;
        cpt::Range tests_range;

        cpt::Test::Options test_ops;
        int min_tests_per_thread = 3;
        bool single_thread = false;

        app.add_option("program", test_info.program, "Program for testing")
                      ->required(true)->check(cpt::ProgramValidator());
        
        app.add_option("-i,--input", test_info.input, "Input file (i.txt by default)");
        
        app.add_option("-o,--output", test_info.output, "Output file (o.txt by default)");
        
        app.add_option("-d,--dir", dir, "Directory with tests (current directory by default)")
                      ->check(cpt::DirValidator());
        
        app.add_option("-n,--num", num_tests,
                      "Number of tests (i[1..n].txt and o[1..n].txt files will be used)")
                      ->check(cpt::NumTestsValidator());

        app.add_flag("-s, --silent", silent, "Do not print messages while testing");

        app.add_option("-t,--tests", tests_str, "Tests ranges divided by comma (a:b, n, ...)")
                      ->check(cpt::TestsValidator(tests_range));
        
        app.add_flag("-e,--time", test_ops.timer, "Elapse test's time execution");

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

        if(!silent){
            cpt::Console::print("Program = ");
            cpt::Console::println(test_info.program.str(), cpt::Console::Color::blue);
            cpt::Console::print("Number of tests = ");
            cpt::Console::println(num_tests, cpt::Console::Color::blue);
            cpt::Console::print("Tests directory = ");
            cpt::Console::println(dir, cpt::Console::Color::blue);
            cpt::Console::println();
        }

        int num_threads = 1;

        if(!single_thread){
            int max_threads = (num_tests - 1) / min_tests_per_thread + 1;
            int hardware_threads = std::thread::hardware_concurrency();
            num_threads = std::min(hardware_threads > 0 ? hardware_threads : 2, max_threads);  
        }

        int block_size = num_tests / num_threads;
        auto block_start = tests_range.begin();

        cpt::MultiTest mt(test_info, test_ops);

        for(size_t i = 0; i < num_threads - 1; ++i){
            auto block_end = block_start;
            std::advance(block_end, block_size);
            
            std::thread thread(
                &cpt::MultiTest::process_range,
                &mt, 
                block_start,
                block_end
            );
            thread.detach();
            block_start = block_end;
        }

        mt.process_range(
            block_start,
            tests_range.end()
        );

        int tests_done = 0;

        while(tests_done < num_tests){
            auto result = mt.get();
            auto test = result.test;
            ++tests_done;

            cpt::Console::print_all(result.index, ": ");

            if(test->passed()){
                cpt::Console::println("OK", cpt::Console::Color::green);
                cpt::Console::println();
            } else {
                cpt::Console::println("FAILED", cpt::Console::Color::red);
                cpt::Console::println();

                if(!silent){
                    cpt::Console::println("Got:");
                    cpt::Console::println(test->answer(), cpt::Console::Color::blue);
                    cpt::Console::println();
                    cpt::Console::println("Expected:");
                    cpt::Console::println(test->result(), cpt::Console::Color::blue);
                    cpt::Console::println();
                }
            }

            if(!silent && test_ops.timer){
                cpt::Console::print("Time = ");
                cpt::Console::print(
                    static_cast<cpt::Timeable<cpt::Test>*>(test.get())->time(),
                    cpt::Console::Color::blue
                );
                cpt::Console::println("ms", cpt::Console::Color::blue);
                cpt::Console::println();
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

