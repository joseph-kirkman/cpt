#include "cpt/range.hpp"

//Atom
namespace cpt {

    Range::Atom::Atom():
        number(0), direction(Range::Direction::forward){}

    Range::Atom::Atom(value_type number, Direction direction):
        number(number), direction(direction){}

}

//Range::Iterator
namespace cpt {

    Range::Iterator::Iterator():
        it_(), end_(), val_(0) {}

    Range::Iterator::Iterator(typename Range::container_iterator current,
                              typename Range::container_iterator end):
        it_(current), end_(end){

        if(it_ != end_){
            val_ = current->number;
        } else {
            val_ = 0;
        }

    }

    Range::Iterator::Iterator(const Iterator& other):
        it_(other.it_), end_(other.end_), val_(other.val_){}

    Range::Iterator& Range::Iterator::operator=(const Range::Iterator& other){
        if(this != &other){
            it_ = other.it_;
            end_ = other.end_;
            val_ = other.val_;
        }
        return *this;
    }

    Range::Iterator Range::Iterator::operator++(int){
        Range::Iterator temp(*this);
        ++(*this);
        return temp;
    }

    Range::Iterator& Range::Iterator::operator++(){

        if(it_ == end_){
            throw std::runtime_error("cannot iterate forward over the end");
        }

        auto next = it_ + 1;

        if(next == end_){
            throw std::runtime_error("range is not closed");
        }

        if(it_->direction != Direction::forward || next->direction != Direction::backward){
            throw std::runtime_error("invalid range direction");
        }

        if(val_ < next->number){
            ++val_;
        } else {
            it_ += 2;
            val_ = it_->number;
        }

        return *this;
    }

    typename Range::Iterator::reference Range::Iterator::operator*() {
        return val_;
    }

    typename Range::Iterator::pointer Range::Iterator::operator->() {
        return &val_;
    }

    bool Range::Iterator::operator==(const Range::Iterator& rhs){
        return (it_ == rhs.it_ && val_ == rhs.val_) || (it_ == end_ && rhs.it_ == end_);
    }

    bool Range::Iterator::operator!=(const Range::Iterator& rhs){
        return !(*this == rhs);
    }
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

    Range::Iterator Range::begin(){
        return Range::Iterator(atoms_.begin(), atoms_.end());
    }

    Range::Iterator Range::end(){
        return Range::Iterator(atoms_.end(), atoms_.end());
    }

    void Range::overlap(){

        std::sort(atoms_.begin(), atoms_.end(), [](const Atom& lhs, const Atom& rhs){ 
            return lhs.number <= rhs.number && lhs.direction <= rhs.direction;
        });

        auto next_level = [](container_iterator atom, int level){
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