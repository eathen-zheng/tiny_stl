//
// Created by zyc on 17-11-25.
//

#ifndef __STL_ALLOC_H
#define __STL_ALLOC_H

#include <cstddef>
#include <cstdlib>

// 第一级的 allocator
template <int inst> // 使不同 inst 所表示的 class 类型不同，例如 xx_template<0> 和 xx_template<1> 类型不同。
class __malloc_alloc_template {
private:
    // deal with oom(out of memory)
    static void *oom_malloc(size_t);
    static void *oom_realloc(void*, size_t);
    static void (*__malloc_alloc_oom_handler)();

public:
    static void* allocate(size_t n) {
        void *result = malloc(n);
        if (0 == result) result = oom_malloc(n);
        return result;
    }

    static void deallocate(void* p, size_t n) {
        free(p);
    }

    static void* reallocate(void* p, size_t old_sz, size_t new_sz) {
        void *result = realloc(p, new_sz);
        if (o == result) result = oom_realloc(p, new_sz);
        return result;
    }

    static void (* set_malloc_handler(void (*f)())) () { // 形式参数是什么
        //don't understand
        void (*old)() = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = f;
        return (old);
    }
};

template <int inst>
void * __malloc_alloc_template<inst>::oom_malloc(size_t n) {
    void (* my_malloc_handler)();
    void *result;
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
void * __malloc_alloc_template<inst>::oom_realloc(void* p, size_t n) {
    void (* my_malloc_handler)();
    void *result;
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
void (* __malloc_alloc_template<inst>::__malloc_alloc_oom_handler) () = 0;

// 第二级的 allocator
template <bool threads, int inst>
class __default_alloc_template {
private:
    enum { __ALIGN = 8}; // bytes 的上调边界
    enum { __MAX_BYTES = 128}; // 最大的 bytes
    enum { __NFREELISTS = __MAX_BYTES / __ALIGN}; // free-lists 个数

private:
    // free-lists 的节点构造 // 不懂 todo
    union obj {
        union obj* free_list_link;
        char* client_data[1];
    };

private:
    static obj* volatile free_list[__NFREELISTS]; // volatile 意思 todo

    // 根据数据区块大小，选择使用哪一个 free-list
    static size_t FREELIST_INDEX(size_t bytes) {
        return ((bytes + (__ALIGN - 1)) / __ALIGN - 1);
    }

    

};

// 封装接口，使得接口符合 STL规格
// SGI STL 容器全部使用这个接口
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
