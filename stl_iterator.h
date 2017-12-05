//
// Created by zyc on 17-12-4.
//

#ifndef TINY_STL_ITERATOR_H
#define TINY_STL_ITERATOR_H

#include <cstddef>

namespace tiny_std {
    /*
     * * * * * * * * * * * * * * * * * * * * * *
     * iterator_category
     * * * * * * * * * * * * * * * * * * * * * *
     */
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    /*
     * * * * * * * * * * * * * * * * * * * * * *
     * base iterator class
     * * * * * * * * * * * * * * * * * * * * * *
     */
    template<class Category, class T, class Distance = ptrdiff_t,
            class Pointer = T*, class Reference = T&>
    struct iterator {
        typedef Category iterator_category;
        typedef T value_type;
        typedef Distance difference_type;
        typedef Pointer pointer;
        typedef Reference reference;
    };

    template<class T, class Distance>
    struct input_iterator {
        typedef input_iterator_tag iterator_category;
        typedef T value_type;
        typedef Distance difference_type;
        typedef T *pointer;
        typedef T &reference;
    };

    struct output_iterator {
        typedef output_iterator_tag iterator_category;
        typedef void value_type;
        typedef void difference_type;
        typedef void pointer;
        typedef void reference;
    };

    template<class T, class Distance>
    struct forward_iterator {
        typedef forward_iterator_tag iterator_category;
        typedef T value_type;
        typedef Distance difference_type;
        typedef T *pointer;
        typedef T &reference;
    };

    template<class T, class Distance>
    struct bidirectional_iterator {
        typedef bidirectional_iterator_tag iterator_category;
        typedef T value_type;
        typedef Distance difference_type;
        typedef T *pointer;
        typedef T &reference;
    };

    template<class T, class Distance>
    struct random_access_iterator {
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef Distance difference_type;
        typedef T *pointer;
        typedef T &reference;
    };

    /*
     * * * * * * * * * * * * * * * * * * * * * *
     * iterator_traits
     * * * * * * * * * * * * * * * * * * * * * *
     */
    template<class Iterator>
    struct iterator_traits {
        typedef typename Iterator::iterator_category iterator_category;
        typedef typename Iterator::value_type value_type;
        typedef typename Iterator::difference_type difference_type;
        typedef typename Iterator::pointer pointer;
        typedef typename Iterator::reference reference;
    };

    template<class T>
    struct iterator_traits<T *> {
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef T *pointer;
        typedef T &reference;
    };

    template<class T>
    struct iterator_traits<const T *> {
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef T *pointer;
        typedef T &reference;
    };

    /*
     * * * * * * * * * * * * * * * * * * * * * *
     * iterator_category(iterator)
     * * * * * * * * * * * * * * * * * * * * * *
     */
    template<class Iterator>
    inline typename iterator_traits<Iterator>::iterator_category
    iterator_category(Iterator &) {
        typedef typename iterator_traits<Iterator>::iterator_category category;
        return category();
    }

    template<class T, class Distance>
    inline input_iterator_tag
    iterator_category(const input_iterator<T, Distance> &) {
        return input_iterator_tag();
    };

    inline output_iterator_tag
    iterator_category(const output_iterator &) {
        return output_iterator_tag();
    }

    template<class T, class Distance>
    inline forward_iterator_tag
    iterator_category(const forward_iterator<T, Distance> &) {
        return forward_iterator_tag();
    };

    template<class T, class Distance>
    inline bidirectional_iterator_tag
    iterator_category(const bidirectional_iterator<T, Distance> &) {
        return bidirectional_iterator_tag();
    };

    template<class T, class Distance>
    inline random_access_iterator_tag
    iterator_category(const random_access_iterator<T, Distance>) {
        return random_access_iterator_tag();
    };

    template<class T>
    inline random_access_iterator_tag
    iterator_category(const T *) {
        return random_access_iterator_tag();
    }

    /*
     * * * * * * * * * * * * * * * * * * * * * *
     * distance_type(iterator)
     * * * * * * * * * * * * * * * * * * * * * *
     */
    template<class Iterator>
    inline typename iterator_traits<Iterator>::difference_type *
    distance_type(const Iterator &) {
        return static_cast<typename iterator_traits<Iterator>::difference_type *>(0);
    }

    template<class T, class Distance>
    inline Distance *
    distance_type(const input_iterator<T, Distance> &) {
        return (Distance *) (0);
    }

    template<class T, class Distance>
    inline Distance *
    distance_type(const forward_iterator<T, Distance> &) {
        return (Distance *) (0);
    }

    template<class T, class Distance>
    inline Distance *
    distance_type(const bidirectional_iterator<T, Distance> &) {
        return (Distance *) (0);
    }

    template<class T, class Distance>
    inline Distance *
    distance_type(const random_access_iterator<T, Distance> &) {
        return (Distance *) (0);
    }

    template<class T>
    inline ptrdiff_t *distance_type(const T *);

    template<class T>
    ptrdiff_t *distance_type(const T *) {
        return (ptrdiff_t *) (0);
    }

    /*
     * * * * * * * * * * * * * * * * * * * * * *
     * value_type(iterator)
     * * * * * * * * * * * * * * * * * * * * * *
     */
    template <class Iterator>
    inline typename iterator_traits<Iterator>::value_type *
    value_type(const Iterator &) {
        return static_cast<typename iterator_traits<Iterator>::value_type *>(0);
    }

    template <class T, class Distance>
    inline T *value_type(const input_iterator<T, Distance> &) {
        return (T *) (0);
    };

    template <class T, class Distance>
    inline T *value_type(const forward_iterator<T, Distance> &) {
        return (T *) (0);
    }

    template <class T, class Distance>
    inline T *value_type(const bidirectional_iterator<T, Distance> &) {
        return (T *) (0);
    }

    template <class T, class Distance>
    inline T *value_type(const random_access_iterator<T, Distance> &) {
        return (T *) (0);
    }

    template <class T>
    inline T *value_type(const T *) {
        return (T *) (0);
    }

    /*
     * * * * * * * * * * * * * * * * * * * * * *
     * distance(iterator first, iterator last)
     * * * * * * * * * * * * * * * * * * * * * *
     */
    template <class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last) {
        typedef typename iterator_traits<InputIterator>::iterator_category category;
        return __distance(first, last, category());
    }

    template <class InputIterator, class Distance>
    inline typename iterator_traits<InputIterator>::difference_type
    __distance(InputIterator first, InputIterator last, input_iterator_tag) {
        typename iterator_traits<InputIterator>::difference_type n = 0;
        while (first != last) {
            ++first; ++n;
        }
        return n;
    };

    template <class RandomAccessIterator>
    inline typename iterator_traits<RandomAccessIterator>::difference_type
    __distance(RandomAccessIterator first, RandomAccessIterator last,
               random_access_iterator_tag) {
        return last - first;
    }

    /*
     * * * * * * * * * * * * * * * * * * * * * *
     * distance(iterator first, iterator last, distance& n)
     * * * * * * * * * * * * * * * * * * * * * *
     */
    template <class InputIterator, class Distance>
    inline void distance(InputIterator first, InputIterator last, Distance& n) {
        __distance(first, last, n, iterator_category(first));
    };

    template <class InputIterator, class Distance>
    inline void __distance(InputIterator first, InputIterator last,
                           Distance& n, input_iterator_tag) {
        while (first != last) {
            ++first; ++n;
        }
    };

    template <class RandomAccessIterator, class Distance>
    inline void __distance(RandomAccessIterator first, RandomAccessIterator last,
                           Distance& n, random_access_iterator_tag) {
        n += (last - first);
    };

    /*
     * * * * * * * * * * * * * * * * * * * * * *
     * advance(iterator& i, distance& n)
     * * * * * * * * * * * * * * * * * * * * * *
     */
    template <class InputIterator, class Distance>
    inline void advance(InputIterator& i, Distance n) {
        __advance(i, n, iterator_category(i));
    };

    template <class InputIterator, class Distance>
    inline void __adcance(InputIterator& i, Distance n, input_iterator_tag) {
        while (n--) {
            ++i;
        }
    };

    template <class BidirectionalIterator, class Distance>
    inline void __advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag) {
        if (n >= 0) {
            while (n--) ++i;
        } else {
            while (n++) --i;
        }
    };

    template <class RandomAccessIterator, class Distance>
    inline void __advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag) {
        i += n;
    };


}
#endif //TINY_STL_ITERATOR_H
