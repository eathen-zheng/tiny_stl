//
// Created by zyc on 17-12-2.
//

#ifndef TINY_STL_MEMORY_H
#define TINY_STL_MEMORY_H

template <class X>
class auto_ptr {
private:
    X* ptr;
    mutable bool owns;

public:
    typedef X element_type;

    explicit auto_ptr(X* p = 0): ptr(p), owns(p) {}

    auto_ptr(const auto_ptr& a): ptr(a.ptr), owns(a.owns) {
        a.owns = 0;
    }

    template <class T> auto_ptr(const auto_ptr<T>& a)
            : ptr(a.ptr), owns(a.owns) {
        a.owns = 0;
    }

    auto_ptr& operator= (const auto_ptr& a) {
        if (&a != this) {
            if (owns) {
                delete ptr;
            }
            owns = a.owns;
            ptr = a.ptr;
            a.owns = 0;
        }
    }

    template <class T> auto_ptr& operator= (const auto_ptr<T>& a) {
        if (&a != this) {
            if (owns) {
                delete  ptr;
            }
            owns = a.owns;
            ptr = a.ptr;
            a.owns = 0;
        }
    }

    ~auto_ptr() {
        if (owns) {
            delete ptr;
        }
    }

    X& operator*() const {return *ptr;}
    X* operator->() const {return ptr;}
    X* get() const {return ptr;}
    X* release() const {owns = false; return ptr;}
};

#endif //TINY_STL_MEMORY_H
