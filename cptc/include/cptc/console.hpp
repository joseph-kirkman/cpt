#ifndef CPTC_CONSOLE_HPP
#define CPTC_CONSOLE_HPP

#include <string>
#include <iostream>

namespace cpt {
    class Console {
    public:
        struct Color {
            static const char* red;
            static const char* green;
            static const char* blue;
            static const char* end;
        };

        typedef const char* ColorType;
    public:

        static void set_stream(std::ostream* os);

        template <typename T>
        static void print(const T& message){
            if(!os_){
                throw std::runtime_error("output stream is empty");
            }
            (*os_) << message;
        }

        template <typename T>
        static void print(const T& message, ColorType color){
            if(!os_){
                throw std::runtime_error("output stream is empty");
            }
            (*os_) << color << message << Console::Color::end;
        }

        static void print_all();

        template <typename T, typename... Args>
        static void print_all(const T& message, Args&&... args){
            Console::print(message);
            Console::print_all(std::forward<Args>(args)...);
        }

        template <typename T>
        static void println(const T& message){
            Console::print(message);
            Console::println();
        }

        static void println();

        template <typename T>
        static void println(const T& message, ColorType color){
            Console::print(message, color);
            Console::println();
        }

        static void println_all();

        template <typename T, typename... Args>
        static void println_all(const T& message, Args&&... args){
            Console::print(message);
            Console::println_all(std::forward<Args>(args)...);
        }

    private:
        static std::ostream* os_;
    };    
}

#endif //CPTC_CONSOLE_HPP