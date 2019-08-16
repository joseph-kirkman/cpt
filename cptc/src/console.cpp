#include "cptc/console.hpp"

namespace cpt {

    const char* Console::Color::red = "\033[1;31m";
    const char* Console::Color::green = "\033[1;32m";
    const char* Console::Color::blue = "\033[1;34m";
    const char* Console::Color::end = "\033[0m";

    std::ostream* Console::os_ = &std::cout;

    void Console::set_stream(std::ostream* os){
        os_ = os;
    }

    void Console::print_all(){}

    void Console::println(){
        Console::print("\n");
    }

    void Console::println_all(){
        Console::println();
    }
}