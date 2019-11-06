#ifndef CPT_SYS_HPP
#define CPT_SYS_HPP

#include <string>
#include "cpt/io.hpp"

namespace cpt {

    class System {
    public:
        static std::pair<std::string, int> execute(const std::string& program);
        static std::string redirect_input(const Path& program, const Path& input);
    };
    
}

#endif //CPT_SYS_HPP