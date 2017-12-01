//
// Created by zyc on 17-12-1.
//

#ifndef TINY_STL_STL_UNINITIALIZED_H
#define TINY_STL_STL_UNINITIALIZED_H

/*
 * * * * * * * * * * * * * * * * * * * * * *
 * uninitialized_copy
 * * * * * * * * * * * * * * * * * * * * * *
 */
template <class InputIterator, class ForwardIterator>
inline ForwardIterator
uninitialized_copy(InputIterator first, InputIterator last,
                   ForwardIterator result) {
    return __uninitialized_copy(first, last, result, value_type(result));
};

// 模板偏特化
inline char* uninitialized_copy(const char* first, const char* last,
                   char* result) {
    memmove(result, first, last - first);
    return result + (last - first);
}

// 模板偏特化
inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last,
                   wchar_t* result) {
    memmove(result, first, sizeof(wchar_t) * (last - first));
    return result + (last - first);
}

// 在 uninitialized_copy() 函数中不能直接推导出 value_type(result) 的类型
// 因此运用函数模板进行实参推导 得到 value_type(result) 的类型
// 再使用 __type_traits 判断类型是否是 POD_type
template <class InputIterator, class ForwardIterator, class T>
inline ForwardIterator
__uninitialized_copy(InputIterator first, InputIterator last,
                     ForwardIterator result, T*) {
    typedef typename __type_traits<T>::is_POD_type is_POD;
    //todo __type_traits
    return __uninitialized_copy_aux(first, last, result, is_POD());
};

template <class InputIterator, class ForwardIterator>
inline ForwardIterator
__uninitialized_copy_aux(InputIterator first, InputIterator last,
                         ForwardIterator result, __true_type) {
    return copy(first, last, result);
};

template <class InputIterator, class ForwardIterator>
inline ForwardIterator
__uninitialized_copy_aux(InputIterator first, InputIterator last,
                         ForwardIterator result, __false_type) {
    ForwardIterator cur = result;
    for (; first != last; ++first, ++cur) {
        construct(&*cur, *first);
    }
};

/*
 * * * * * * * * * * * * * * * * * * * * * *
 * uninitialized_copy_n
 * * * * * * * * * * * * * * * * * * * * * *
 */
template <class InputIterator, class Size, class ForwardIterator>
inline pair<InputIterator, ForwardIterator>
uninitialized_copy_n(InputIterator first, Size count,
                     ForwardIterator result) {
    return __uninitialized_copy_n(first, count, result,
                                  iterator_category(first));
};

template <class RandomAccessIterator, class Size, class ForwardIterator>
inline pair<RandomAccessIterator, ForwardIterator>
__uninitialized_copy_n(RandomAccessIterator first, Size count,
                       ForwardIterator result,
                       random_access_iterator_tag) {
    RandomAccessIterator last = first + count;
    return make_pair(last, uninitialized_copy(first, last, result));
};


template <class InputIterator, class Size, class ForwardIterator>
inline pair<InputIterator, ForwardIterator>
__uninitialized_copy_n(InputIterator first, Size count,
                       ForwardIterator result,
                       input_iterator_tag) {
    ForwardIterator cur = result;
    for (; count > 0; --count, ++first, ++cur) {
        construct(&*cur, *first);
    }
    return pair<InputIterator, ForwardIterator>(first, cur);
};

/*
 * * * * * * * * * * * * * * * * * * * * * *
 * uninitialized_fill
 * * * * * * * * * * * * * * * * * * * * * *
 */

template <class ForwardIterator, class T>
inline void uninitialized_fill(ForwardIterator first, ForwardIterator last,
                               const T& x) {
    __uninitialized_fill(first, last, x, value_type(first));
};

template <class ForwardIterator, class T, class T1>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last,
                                 const T& x, T1*) {
    typedef typename __type_traits<T1>::is_POD_type is_POD;
    __uninitialized_fill_aux(first, last, x, is_POD());
};

template <class ForwardIterator, class T>
inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
                                     const T& x, __true_type) {
    fill(first, last, x);
};

template <class ForwardIterator, class T>
inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
                                     const T& x, __false_type) {
    ForwardIterator cur = first;
    for (; cur != last; ++cur) {
        construct(&*cur, x);
    }
};

/*
 * * * * * * * * * * * * * * * * * * * * * *
 * uninitialized_fill_n
 * * * * * * * * * * * * * * * * * * * * * *
 */
template <class ForwardIterator, class Size, class T>
inline ForwardIterator
uninitialized_fill_n(ForwardIterator first, Size count, const T& x) {
    return __uninitialized_fill_n(first, count, x, value_type(first));
};

template <class ForwardIterator, class Size, class T, class T1>
inline ForwardIterator
__uninitialized_fill_n(ForwardIterator first, Size count, const T& x, T1*) {
    typedef typename __type_traits<T1>::is_POD_type is_POD;
    return __uninitialized_fill_n_aux(first, count, x, is_POD());
};

template <class ForwardIterator, class Size, class T>
inline ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator first, Size count,
                           const T& x, __true_type) {
    return fill_n(first, count, x);
};

template <class ForwardIterator, class Size, class T>
inline ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator first, Size count,
                           const T& x, __false_type) {
    ForwardIterator cur = first;
    for (; count > 0; --count, ++cur) {
        construct(&*cur, x);
    }
};





#endif //TINY_STL_STL_UNINITIALIZED_H
