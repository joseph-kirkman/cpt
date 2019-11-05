#include "cpt/range.hpp"

//Atom
namespace cpt {

    Range::Atom::Atom():
        number(0), direction(Range::Direction::forward){}

    Range::Atom::Atom(value_type number, Direction direction):
        number(number), direction(direction){}

}

//Range
namespace cpt {

    void Range::add(value_type n){
        add(n, n);
    }

    void Range::add(value_type n, value_type m){
        atoms_.emplace_back(n, Direction::forward);
        atoms_.emplace_back(m, Direction::backward);
        overlap();
    }

    bool Range::empty() const{
        return atoms_.empty();
    }

    size_t Range::size() const{
        size_t result = 0;

        for(size_t i = 0; i < atoms_.size(); i += 2){
            result += atoms_[i + 1].number - atoms_[i].number + 1;
        }

        return result;
    }

    typename Range::iterator Range::begin(){
        return iterator(atoms_.begin(), atoms_.end());
    }

    typename Range::iterator Range::end(){
        return iterator(atoms_.end(), atoms_.end());
    }

    typename Range::const_iterator Range::cbegin() const {
        return const_iterator(atoms_.cbegin(), atoms_.cend());
    }

    typename Range::const_iterator Range::cend() const {
        return const_iterator(atoms_.cend(), atoms_.cend());
    }

    void Range::overlap(){

        std::sort(atoms_.begin(), atoms_.end(), [](const Atom& lhs, const Atom& rhs){ 
            return lhs.number <= rhs.number && lhs.direction <= rhs.direction;
        });

        auto next_level = [](typename container_type::iterator atom, int level){
            return level + (atom->direction == Direction::forward ? 1 : -1);
        };

        int level = 0;
        
        for(auto atom = atoms_.begin(); atom != atoms_.end();){
            int new_level = next_level(atom, level);

            if(new_level > 1 || level > 1){
                atoms_.erase(atom);
            } else {
                ++atom;
            }

            level = new_level;
        }

    }
}