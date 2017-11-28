//
// Created by zyc on 17-11-25.
//

#ifndef __STL_ALLOC_H
#define __STL_ALLOC_H

#include <cstddef>
#include <cstdlib>


/*
 * * * * * * * * * * * * * * * * * * * * * *
 * 第一级 allocator
 * * * * * * * * * * * * * * * * * * * * * *
 */
template <int inst> // 使不同 inst 所表示的 class 类型不同，例如 xx_template<0> 和 xx_template<1> 类型不同。
class __malloc_alloc_template {
private:
    // deal with oom(out of memory)
    static void* oom_malloc(size_t);
    static void* oom_realloc(void*, size_t);
    static void (*__malloc_alloc_oom_handler)();

public:
    static void* allocate(size_t n);
    static void deallocate(void* p, size_t n);
    static void* reallocate(void* p, size_t old_sz, size_t new_sz);
    static void (*set_malloc_handler(void (*f)())) () {
        //todo
        void (*old)() = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = f;
        return (old);
    }
};

// 静态成员函数的定义
template <int inst>
inline void* __malloc_alloc_template<inst>::oom_malloc(size_t n) {
    void (*my_malloc_handler)();
    void* result;
    for (;;) {
        my_malloc_handler = __malloc_alloc_oom_handler;
        if (0 == __malloc_alloc_oom_handler) {
            //todo
        }
        (*my_malloc_handler)(); // my_malloc_handler 是一个函数指针，这是是调用它所指向的函数
        result = malloc(n);
        if (result) return (result);
    }
}

template <int inst>
inline void* __malloc_alloc_template<inst>::oom_realloc(void* p, size_t n) {
    void (*my_malloc_handler)();
    void* result;
    for (;;) {
        my_malloc_handler = __malloc_alloc_oom_handler;
        if (0 == __malloc_alloc_oom_handler) {
            //todo
        }
        (*my_malloc_handler)();
        result = realloc(p, n);
        if (result) return result;
    }
}

template <int inst>
inline void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler) () = 0;

template <int inst>
inline void* __malloc_alloc_template<inst>::allocate(size_t n) {
    void* result = malloc(n);
    if (0 == result) result = oom_malloc(n);
    return result;
}

template <int inst>
inline void __malloc_alloc_template<inst>::deallocate(void* p, size_t n) {
    free(p);
}

template <int inst>
inline void* __malloc_alloc_template<inst>::reallocate(void* p, size_t old_sz, size_t new_sz) {
    void *result = realloc(p, new_sz);
    if (o == result) result = oom_realloc(p, new_sz);
    return result;
}


/*
 * * * * * * * * * * * * * * * * * * * * * *
 * 第二级 allocator
 * * * * * * * * * * * * * * * * * * * * * *
 */
template <int inst>
class __default_alloc_template {
private:
    // free-list 的参数设置
    enum { __ALIGN = 8};
    enum { __MAX_BYTES = 128};
    enum { __NFREELISTS = __MAX_BYTES / __ALIGN};

private:
    // free-lists 的节点构造
    union obj {
        union obj* free_list_link;
        char* client_data[1];
    };

private:
    static obj* volatile free_list[__NFREELISTS];

    static size_t FREELIST_INDEX(size_t bytes); // 根据数据区块大小，选择使用哪一个 free-list
    static void* refill(size_t n); // 返回大小为 n 的对象
    static char* chunk_alloc(size_t n, int& nobjs); // 配置一大块空间，避免过多内存碎片

    // chunk_alloc 相关状态
    static char* start_free;
    static char* end_free;
    static size_t heap_size;

public:
    static void* allocate(size_t n);
    static void deallocate(void* pp, size_t n);
    static void* reallocate(void *p, size_t old_sz, size_t new_sz);
};

// 静态成员变量的定义
template <int inst>
__default_alloc_template<inst>::obj* volatile
        __default_alloc_template<inst>::free_list[__NFREELISTS] =
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template <int inst>
char*  __default_alloc_template<inst>::start_free = 0;

template <int inst>
char*  __default_alloc_template<inst>::end_free = 0;

template <int inst>
size_t  __default_alloc_template<inst>::heap_size = 0;

// 静态成员函数的定义
template <int inst>
inline size_t __default_alloc_template<inst>::FREELIST_INDEX(size_t bytes) {
    return ((bytes + (__ALIGN - 1)) / __ALIGN - 1);
}

template <int inst>
inline void* __default_alloc_template<inst>::refill(size_t n) {
    //todo
    return nullptr;
}

template <int inst>
inline char* __default_alloc_template<inst>::chunk_alloc(size_t n, int& nobjs) {
    //todo
    return nullptr;
}

template <int inst>
inline void* __default_alloc_template<inst>::allocate(size_t n) {
    //todo
    return nullptr;
}

template <int inst>
inline void __default_alloc_template<inst>::deallocate(void *pp, size_t n) {
    //todo
}

template <int inst>
inline void* __default_alloc_template<inst>::reallocate(void *p, size_t old_sz, size_t new_sz) {
    //todo
    return nullptr;
}


/*
 * * * * * * * * * * * * * * * * * * * * * *
 * 封装接口，使得接口符合 STL规格
 * SGI STL 容器全部使用这个接口
 * * * * * * * * * * * * * * * * * * * * * *
 */
template <class T, class Alloc>
class simple_alloc {
public:
    static T* allocate(size_t n) {
        return (0 == n) ? 0 : (T*) Alloc::allocate(n * sizeof(T));
    }

    // 没有指定 n 时，分配一个 T 大小的内存
    static T* allocate(void) {
        return (T*) Alloc::allocate(sizeof(T));
    }

    static void deallocate(T* p, size_t n) {
        if (0 != n) Alloc::deallocate(p, n * sizeof(T));
    }

    static void deallocate(T* p) {
        Alloc::deallocate(p, sizeof(T));
    }
};


#endif //__STL_ALLOC_H
