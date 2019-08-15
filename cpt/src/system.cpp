#include <array>
#include <sstream>
#include "cpt/system.hpp"

namespace cpt {

    std::string System::execute(const std::string& program) {
        std::array<char, 128> buffer{};
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(program.c_str(), "r"), pclose);

        if (!pipe) {
            throw std::runtime_error("popen() failed");
        }

        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }

        return result;
    }

    std::string System::redirect_input(const Path& program, const Path& input){
        std::stringstream ss;

        if(!program.is_absolute()){
            ss << "./";
        }

        ss << program.str() << " <" << input.str();
        return ss.str();
    }
}