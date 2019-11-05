#ifndef CPT_RANGE_HPP
#define CPT_RANGE_HPP

#include <vector>

namespace cpt {
    class Range {
    public:
        typedef int value_type;
    private:

        enum Direction {
            forward, backward
        };

        class Atom {
        public:
            Atom();
            Atom(value_type number, Direction direction);
        public:
            value_type number;
            Direction  direction;
        };
    public:
        typedef std::vector<Atom> container_type;
    private:
        template <bool Const=false>
        class Iterator{
        private:
            typedef typename std::conditional<
                                Const,
                                typename container_type::const_iterator,
                                typename container_type::iterator
                             >::type container_iterator;
        public:
            typedef std::forward_iterator_tag iterator_category;
            typedef Range::value_type value_type;
            typedef std::ptrdiff_t difference_type;
            typedef typename std::conditional<Const, const value_type&, value_type&>::type reference;
            typedef typename std::conditional<Const, const value_type*, value_type*>::type pointer;
        public:
            Iterator():
                it_(), end_(), val_(0) {}

            Iterator(container_iterator current, container_iterator end):
                it_(current), end_(end){

                if(it_ != end_){
                    val_ = current->number;
                } else {
                    val_ = 0;
                }
            }

            Iterator(const Iterator& other): 
                it_(other.it_), end_(other.end_), val_(other.val_){}
            
            ~Iterator()=default;

            Iterator& operator=(const Iterator& other){
                if(this != &other){
                    it_ = other.it_;
                    end_ = other.end_;
                    val_ = other.val_;
                }
                return *this;
            }

            Iterator operator++(int){
                Iterator<Const> temp(*this);
                ++(*this);
                return temp;                
            }

            Iterator& operator++(){
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

            reference operator*(){
                return val_;
            }

            pointer operator->(){
                return &val_;
            }

            bool operator==(const Iterator& rhs){
                return (it_ == rhs.it_ && val_ == rhs.val_) || (it_ == end_ && rhs.it_ == end_);
            }

            bool operator!=(const Iterator& rhs){
                return !(*this == rhs);
            }
        private:
            container_iterator it_;
            container_iterator end_;
            value_type         val_;
        };
    public:
        typedef Iterator<false> iterator;
        typedef Iterator<true> const_iterator;
    public:
        void add(value_type n);
        void add(value_type n, value_type m);
        bool empty() const;
        size_t size() const;
        iterator begin();
        iterator end();
        const_iterator cbegin() const;
        const_iterator cend() const;
    private:
        void overlap();
    private:
        container_type atoms_;
    };
}

#endif //CPT_RANGE_HPP