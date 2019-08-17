#ifndef CPT_MIXINS_HPP
#define CPT_MIXINS_HPP

#include <chrono>

namespace cpt {

    template <class Base>
    class Timeable: public Base {
    public:
        typedef long long time_type;

        using Base::Base;

        void run() override {
            auto then = std::chrono::steady_clock::now();
            Base::run();
            auto now = std::chrono::steady_clock::now();
            ms_ = std::chrono::duration_cast<std::chrono::milliseconds>(now - then).count();
        }

        time_type time() const {
            return ms_;
        }

    protected:
        time_type ms_ = 0;
    };

}

#endif //CPT_MIXINS_HPP