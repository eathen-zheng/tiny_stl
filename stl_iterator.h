//
// Created by zyc on 17-12-3.
//

#ifndef TINY_STL_STL_ITERATOR_H
#define TINY_STL_STL_ITERATOR_H

/*
 * * * * * * * * * * * * * * * * * * * * * *
 * iterator_category struct
 * * * * * * * * * * * * * * * * * * * * * *
 */
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <class T, class Distance> struct input_iterator {
    typedef input_iterator_tag iterator_category;
    typedef T                  value_type;
    typedef Distance           difference_type;
    typedef T*                 pointer;
    typedef T&                 reference;
};

struct output_iterator {
    typedef output_iterator_tag iterator_category;
    typedef void                value_type;
    typedef void                difference_type;
    typedef void                pointer;
    typedef void                reference;
};

template <class T, class Distance> struct forward_iterator {
    typedef forward_iterator_tag iterator_category;
    typedef T                    value_type;
    typedef Distance             difference_type;
    typedef T*                   pointer;
    typedef T&                   reference;
};

template <class T, class Distance> struct bidirectional_iterator {
    typedef bidirectional_iterator_tag iterator_category;
    typedef T                          value_type;
    typedef Distance                   difference_type;
    typedef T*                         pointer;
    typedef T&                         reference;
};

template <class T, class Distance> struct random_access_iterator {
    typedef random_access_iterator_tag iterator_category;
    typedef T                          value_type;
    typedef Distance                   difference_type;
    typedef T*                         pointer;
    typedef T&                         reference;
};

/*
 * * * * * * * * * * * * * * * * * * * * * *
 * iterator struct
 * * * * * * * * * * * * * * * * * * * * * *
 */
template <class Category, class T, class Distance = ptrdiff_t,
          class Pointer = T*, class Reference = T&>
struct iterator {
    typedef Category  iterator_category;
    typedef T         value_type;
    typedef Distance  difference_type;
    typedef Pointer   pointer;
    typedef Reference reference;
};

/*
 * * * * * * * * * * * * * * * * * * * * * *
 * iterator_traits struct
 * * * * * * * * * * * * * * * * * * * * * *
 */
template <class Iterator>
struct iterator_traits {
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::difference_type difference_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
};

template <class T>
struct iterator_traits<T*> {
    typedef typename random_access_iterator iterator_category;
    typedef typename T value_type;
    typedef typename ptrdiff_t difference_type;
    typedef typename T* pointer;
    typedef typename T& reference;
};

template <class T>
struct iterator_traits<const T*> {
    typedef typename random_access_iterator iterator_category;
    typedef typename T value_type;
    typedef typename ptrdiff_t difference_type;
    typedef typename T* pointer;
    typedef typename T& reference;
};

/*
 * * * * * * * * * * * * * * * * * * * * * *
 * iterator_category(Iterator)
 * * * * * * * * * * * * * * * * * * * * * *
 */
template <class Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(Iterator) {
    typedef typename iterator_traits<Iterator>::iterator_category category;
    return category();
}

template <class T, class Distance>
inline input_iterator_tag
iterator_category(const input_iterator<T, Distance>&) {
    return input_iterator_tag();
};

inline output_iterator_tag
iterator_category(const output_iterator&) {
    return output_iterator_tag();
}

template <class T, class Distance>
inline forward_iterator_tag
iterator_category(const forward_iterator<T, Distance>&) {
    return forward_iterator_tag();
};

template <class T, class Distance>
inline bidirectional_iterator_tag
iterator_category(const bidirectional_iterator<T, class>&) {
    return bidirectional_iterator_tag();
};

template <class T, class Distance>
inline random_access_iterator_tag
iterator_category(const random_access_iterator<T, Distance>) {
    return random_access_iterator_tag();
};

template <class T>
inline random_access_iterator_tag
iterator_category(const t*) {
    return random_access_iterator_tag();
}

/*
 * * * * * * * * * * * * * * * * * * * * * *
 * distance_type(Iterator)
 * * * * * * * * * * * * * * * * * * * * * *
 */

#endif //TINY_STL_STL_ITERATOR_H
