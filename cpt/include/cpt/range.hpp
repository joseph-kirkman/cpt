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
        typedef typename std::vector<Atom>::iterator container_iterator;

        class Iterator:
            public std::iterator<std::forward_iterator_tag, value_type> {
        public:
            Iterator();
            Iterator(container_iterator current, container_iterator end);
            Iterator(const Iterator& other);
            ~Iterator()=default;
            Iterator& operator=(const Iterator& other);

            Iterator operator++(int);
            Iterator& operator++();
            reference operator*();
            pointer operator->();
            bool operator==(const Iterator& rhs);
            bool operator!=(const Iterator& rhs);
        private:
            container_iterator it_;
            container_iterator end_;
            value_type         val_;
        };
    public:
        void add(value_type n);
        void add(value_type n, value_type m);
        bool empty() const;
        size_t size() const;
        Iterator begin();
        Iterator end();
    private:
        void overlap();
    private:
        std::vector<Atom> atoms_;
    };
}

#endif //CPT_RANGE_HPP