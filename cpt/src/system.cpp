#include <array>
#include <sstream>
#include "cpt/string.hpp"
#include "cpt/system.hpp"


// System
namespace cpt {

    std::pair<std::string, int> System::execute(const std::string& program) {
        std::array<char, 128> buffer{};
        std::string content;
        int exit_code = 0;

        auto close_file = [&](FILE* file){
            auto result = pclose(file);
            exit_code = WEXITSTATUS(result);
        };

        std::unique_ptr<FILE, decltype(close_file)> pipe(popen(program.c_str(), "r"), close_file);

        if (!pipe) {
            throw std::runtime_error("popen() failed");
        }

        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            content += buffer.data();
        }

        return std::make_pair(content, exit_code);
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