//
// Created by zyc on 17-11-25.
//

#ifndef TINY_STL_CONSTRUCT_H
#define TINY_STL_CONSTRUCT_H


// construct 构造
template <class T1, class T2>
inline void construct(T1* p, T2& value) {
    new (p) T1(value);
};

// destroy 析构 第一种形式
template <class T>
inline void destroy(T* p) {
    p->~T();
}

// destroy 析构 第二种形式
template <class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last) {
    __destroy(first, last, value_type(first)); //todo value_type
}

// 第二种形式 模板偏特化
inline void destroy(char*, char*) {}
inline void destroy(wchar_t*, wchar_t*) {}

// __destory 使用函数模板进行实参推导
template <class ForwardIterator, class T>
inline void __destroy(ForwardIterator first, ForwardIterator last, T*) {
    typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor; //todo __type_traits
    __destory_aux(first, last, trivial_destructor());
};

// __destory_aux 析构函数重要
template <class ForwardIterator>
inline void __destory_aux(ForwardIterator first, ForwardIterator last, __false_type) {
    for (; first != last; ++first)
        destroy(&*first); // 将 iter 所指的对象(即 *first)进行析构
}

// __destory_aux 析构函数不重要
template <class ForwardIterator>
inline void __destory_aux(ForwardIterator first, ForwardIterator last, __true_type) {}

#endif //TINY_STL_CONSTRUCT_H
