//
// Created by zyc on 17-12-8.
//

#ifndef TINY_STL_PAIR_H
#define TINY_STL_PAIR_H

namespace tiny_std {
    template <class T1, class T2>
    struct pair {
        typedef T1 first_type;
        typedef T2 second_type;

        T1 first;
        T2 second;

        pair() : first(T1()), second(T1()) {}
        pair(const T1& a, const T2& b) : first(a), second(b) {}

        template <class U1, class U2>
        pair(const pair<U1, U2>& p) : first(p.first), second(p.second) {}
    };

    template <class T1, class T2>
    inline pair<T1, T2>& make_pair(const T1& x, const T2& y) {
        return pair<T1, T2>(x, y);
    };

    template <class T1, class T2>
    bool operator==(const pair<T1, T2>& x, const pair<T1, T2>& y) {
        return x.first == y.first && x.second == y.second;
    };

    template <class T1, class T2>
    bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y) {
        if (x.first < y.first) return true;
        else return x.second < y.second;
    };

    template <class T1, class T2>
    bool operator>(const pair<T1, T2>& x, const pair<T1, T2>& y) {
        if (x.first > y.first) return true;
        else return x.second > y.second;
    };
}
#endif //TINY_STL_PAIR_H
