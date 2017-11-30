//
// Created by zyc on 17-11-25.
//

#ifndef __STL_ALLOC_H
#define __STL_ALLOC_H

#include <cstddef>
#include <cstdlib>
#include <cstring>


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

typedef __malloc_alloc_template<0> malloc_alloc;

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

    static size_t ROUND_UP(size_t);
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
inline size_t __default_alloc_template<inst>::ROUND_UP(size_t bytes) {
    return ((bytes + __ALIGN - 1) & ~(__ALIGN - 1));
}

template <int inst>
inline size_t __default_alloc_template<inst>::FREELIST_INDEX(size_t bytes) {
    return ((bytes + __ALIGN - 1) / __ALIGN - 1);
}

template <int inst>
inline void* __default_alloc_template<inst>::refill(size_t n) {
    //todo add comment
    int nobjs = 20;
    char* chunk = chunk_alloc(n, nobjs);
    obj* volatile * my_free_list;
    obj *result, *current_obj, *next_obj;

    if (1 == nobjs) return chunk;
    my_free_list = free_list + FREELIST_INDEX(n);

    result = (obj*) chunk;
    *my_free_list = next_obj = (obj*) (chunk + n);

    for (int i = 1; ; ++ i) {
        current_obj = next_obj;
        next_obj = (obj*) ((char*) next_obj + n);
        if (nobjs - 1 == i) {
            current_obj->free_list_link = 0;
            break;
        } else {
            current_obj->free_list_link = next_obj;
        }
    }

    return result;
}

template <int inst>
inline char* __default_alloc_template<inst>::chunk_alloc(size_t size, int& nobjs) {
    //todo
    char* result;
    size_t total_bytes = size * nobjs;
    size_t  bytes_left = end_free - start_free;

    if (bytes_left > total_bytes) {
        // 内存池剩余空间满足需求
        result = start_free;
        start_free += total_bytes;
        return result;
    } else if (bytes_left >= size) {
        // 内存池剩余空间不能完全满足需求，但足够供应一个区块
        nobjs = bytes_left / size;
        total_bytes = size * nobjs;
        result = start_free;
        start_free += total_bytes;
        return result;
    } else {
        // 内存池空间不足以供应一个区块
        size_t  bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
        if (bytes_left > 0) {
            // 将内存池剩余空间配置给合适的 free-list
            obj* volatile * my_free_list = free_list + FREELIST_INDEX(bytes_left);
            ((obj*) start_free)->free_list_link = *my_free_list;
            *my_free_list = (obj*) start_free;
        }
        // 配置 heap 空间，用来补充内存池
        start_free = (char*) malloc(bytes_to_get);
        if (0 == start_free) {
            // 寻找 free-list 之中尚未用到的，大小合适的区块
            obj* volatile *my_free_list, *p;
            for (int i = size; i <= __MAX_BYTES; i += __ALIGN) {
                my_free_list = free_list + FREELIST_INDEX(i);
                p = *my_free_list;
                if (0 != p) {
                    // 若找到区块，将该区块供应给调用者
                    *my_free_list = p->free_list_link;
                    start_free = (char*) p;
                    end_free = start_free + i;
                    // 递归调用自己，以修正 nobjs
                    return chunk_alloc(size, nobjs);
                }
            }
            // 如果出现意外，调用第一级 allocator，这将会抛出异常或者改善内存不足的情况
            end_free = 0;
            start_free = (char*) malloc_alloc::allocate(bytes_to_get);
        }
        heap_size += bytes_to_get;
        end_free = start_free + bytes_to_get;
        // 递归调用自己，以修正 nobjs
        return (chunk_alloc(size, nobjs));
    }
    return nullptr;
}

template <int inst>
inline void* __default_alloc_template<inst>::allocate(size_t n) {
    obj* volatile * my_free_list;
    obj* result;
    // 大于 128 就调用第一级 allocator
    if (n > (size_t) __MAX_BYTES) {
        return malloc_alloc::allocate(n);
    }
    // 寻找对应的 free list
    my_free_list = free_list + FREELIST_INDEX(n);
    result = *my_free_list;
    if (result == 0) {
        // 没找到可用的 free list，准备重新填充 free list
        void* r = refill(ROUND_UP(n));
        return r;
    }
    // 调整 free list
    *my_free_list = result->free_list_link;
    return result;
}

template <int inst>
inline void __default_alloc_template<inst>::deallocate(void* p, size_t n) {
    obj* q = (obj*) p;
    obj* volatile * my_free_list;
    // 大于 128 就调用第一级 allocator
    if (n > (size_t) __MAX_BYTES) {
        malloc_alloc::deallocate(p, n);
        return;
    }
    // 寻找对应的 free list
    my_free_list = free_list + FREELIST_INDEX(n);
    // 调整 free list，回收区块
    q->free_list_link = *my_free_list;
    *my_free_list = q;
}

template <int inst>
inline void* __default_alloc_template<inst>::reallocate(void* p, size_t old_sz, size_t new_sz) {
    void* result;
    size_t  copy_sz;
    // 大于 128 就调用第一级 allocator
    if (old_sz > (size_t) __MAX_BYTES && new_sz > (size_t) __MAX_BYTES) {
        return malloc_alloc::reallocate(p, old_sz, new_sz);
    }
    // 填充后大小不变，则直接返回
    if (ROUND_UP(old_sz) == ROUND_UP((new_sz))) {
        return p;
    }
    // allocate 新区快
    result = allocate(new_sz);
    copy_sz = new_sz > old_sz ? old_sz : new_sz;
    memcpy(result, p, copy_sz);
    // deallocate 旧区块
    deallocate(p, old_sz);
    return result;
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
