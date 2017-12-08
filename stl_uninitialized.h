//
// Created by zyc on 17-12-1.
//

#ifndef TINY_STL_STL_UNINITIALIZED_H
#define TINY_STL_STL_UNINITIALIZED_H


#include <cstring>
#include "stl_construct.h"
#include "stl_iterator.h"
#include "stl_pair.h"


namespace tiny_std {
    /*
    * * * * * * * * * * * * * * * * * * * * * *
    * uninitialized_copy
    * Copies [first, last) into [result, result + (last - first)).
    * * * * * * * * * * * * * * * * * * * * * *
    */
    template<class InputIterator, class ForwardIterator>
    inline ForwardIterator
    uninitialized_copy(InputIterator first, InputIterator last,
                       ForwardIterator result) {
        return __uninitialized_copy(first, last, result, value_type(result));
    };

    // 模板偏特化
    inline char *uninitialized_copy(const char *first, const char *last,
                                    char *result) {
        memmove(result, first, last - first);
        return result + (last - first);
    }

    // 模板偏特化
    inline wchar_t *uninitialized_copy(const wchar_t *first, const wchar_t *last,
                                       wchar_t *result) {
        memmove(result, first, sizeof(wchar_t) * (last - first));
        return result + (last - first);
    }

    // 在 uninitialized_copy() 函数中不能直接推导出 value_type(result) 的类型
    // 因此运用函数模板进行实参推导 得到 value_type(result) 的类型
    // 再使用 __type_traits 判断类型是否是 POD_type
    template<class InputIterator, class ForwardIterator, class T>
    inline ForwardIterator
    __uninitialized_copy(InputIterator first, InputIterator last,
                         ForwardIterator result, T *) {
        typedef typename __type_traits<T>::is_POD_type is_POD;
        return __uninitialized_copy_aux(first, last, result, is_POD());
    };

    template<class InputIterator, class ForwardIterator>
    inline ForwardIterator
    __uninitialized_copy_aux(InputIterator first, InputIterator last,
                             ForwardIterator result, __true_type) {
        return copy(first, last, result); //todo copy
    };

    template<class InputIterator, class ForwardIterator>
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
    * Copies [first, first + count) into [result, result + count).
    * * * * * * * * * * * * * * * * * * * * * *
    */
    template<class InputIterator, class Size, class ForwardIterator>
    inline pair <InputIterator, ForwardIterator>
    uninitialized_copy_n(InputIterator first, Size count,
                         ForwardIterator result) {
        return __uninitialized_copy_n(first, count, result,
                                      iterator_category(first));
    };

    template<class RandomAccessIterator, class Size, class ForwardIterator>
    inline pair <RandomAccessIterator, ForwardIterator>
    __uninitialized_copy_n(RandomAccessIterator first, Size count,
                           ForwardIterator result,
                           random_access_iterator_tag) {
        RandomAccessIterator last = first + count;
        return make_pair(last, uninitialized_copy(first, last, result));
    };


    template<class InputIterator, class Size, class ForwardIterator>
    inline pair <InputIterator, ForwardIterator>
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
    * Fills [first, last) with x.
    * * * * * * * * * * * * * * * * * * * * * *
    */

    template<class ForwardIterator, class T>
    inline void uninitialized_fill(ForwardIterator first, ForwardIterator last,
                                   const T &x) {
        __uninitialized_fill(first, last, x, value_type(first));
    };

    template<class ForwardIterator, class T, class T1>
    inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last,
                                     const T &x, T1 *) {
        typedef typename __type_traits<T1>::is_POD_type is_POD;
        __uninitialized_fill_aux(first, last, x, is_POD());
    };

    template<class ForwardIterator, class T>
    inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
                                         const T &x, __true_type) {
        fill(first, last, x); //todo fill
    };

    template<class ForwardIterator, class T>
    inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
                                         const T &x, __false_type) {
        ForwardIterator cur = first;
        for (; cur != last; ++cur) {
            construct(&*cur, x);
        }
    };

    /*
    * * * * * * * * * * * * * * * * * * * * * *
    * uninitialized_fill_n
    * Fills [first, first + count) with x.
    * * * * * * * * * * * * * * * * * * * * * *
    */
    template<class ForwardIterator, class Size, class T>
    inline ForwardIterator
    uninitialized_fill_n(ForwardIterator first, Size count, const T &x) {
        return __uninitialized_fill_n(first, count, x, value_type(first));
    };

    template<class ForwardIterator, class Size, class T, class T1>
    inline ForwardIterator
    __uninitialized_fill_n(ForwardIterator first, Size count, const T &x, T1 *) {
        typedef typename __type_traits<T1>::is_POD_type is_POD;
        return __uninitialized_fill_n_aux(first, count, x, is_POD());
    };

    template<class ForwardIterator, class Size, class T>
    inline ForwardIterator
    __uninitialized_fill_n_aux(ForwardIterator first, Size count,
                               const T &x, __true_type) {
        return fill_n(first, count, x); //todo fill_n
    };

    template<class ForwardIterator, class Size, class T>
    inline ForwardIterator
    __uninitialized_fill_n_aux(ForwardIterator first, Size count,
                               const T &x, __false_type) {
        ForwardIterator cur = first;
        for (; count > 0; --count, ++cur) {
            construct(&*cur, x);
        }
    };

    /*
    * * * * * * * * * * * * * * * * * * * * * *
    * __uninitialized_copy_copy
    * Copies [first1, last1) into [result, result + (last1 - first1)), and
    * copies [first2, last2) into
    * [result + (last1 - first1), result + (last1 - first1) + (last2 - first2)).
    * * * * * * * * * * * * * * * * * * * * * *
    */

    template<class InputIterator1, class InputIterator2, class ForwardIterator>
    inline ForwardIterator
    __uninitialized_copy_copy(InputIterator1 first1, InputIterator1 last1,
                              InputIterator2 first2, InputIterator2 last2,
                              ForwardIterator result) {
        ForwardIterator mid = uninitialized_copy(first1, last1, result);
        return uninitialized_copy(first2, last2, mid);
    };

    /*
    * * * * * * * * * * * * * * * * * * * * * *
    * __uninitialized_fill_copy
    * Fills [result, mid) with x, and
    * copies [first, last) into [mid, mid + (last - first)).
    * * * * * * * * * * * * * * * * * * * * * *
    */

    template<class ForwardIterator, class T, class InputIterator>
    inline ForwardIterator
    __uninitialized_copy_fill(ForwardIterator result, ForwardIterator mid,
                              const T &x,
                              InputIterator first, InputIterator last) {
        uninitialized_fill(result, mid, x);
        return uninitialized_copy(first, last, mid);
    };

    /*
    * * * * * * * * * * * * * * * * * * * * * *
    * __uninitialized_fill_copy
    * Copies [first1, last1) into [first2, first2 + (last1 - first1)), and
    * fills [first2 + (last1 - first1), last2) with x.
    * * * * * * * * * * * * * * * * * * * * * *
    */

    template<class InputIterator, class T, class ForwardIterator>
    inline void
    __uninitialized_copy_fill(InputIterator first1, InputIterator last1,
                              ForwardIterator first2, ForwardIterator last2,
                              const T &x) {
        ForwardIterator mid2 = uninitialized_copy(first1, last1, first2);
        uninitialized_fill(mid2, last2, x);
    };

}
#endif //TINY_STL_STL_UNINITIALIZED_H
