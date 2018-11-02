#ifndef RANGE_RANGE_HPP
#define RANGE_RANGE_HPP

#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <vector>

class range {
private:
    int hd;
    int tl;

public:
    range(const int hd = 0, const int tl = 0) : hd(hd), tl(tl) {}

    const int head() const {
        return hd;
    }

    const int tail() const {
        return tl;
    }

    const int length() const {
        return tl - hd;
    };

    const bool contains(const int x) const {
        return hd <= x && tl >= x;
    }

    bool operator==(const range &rhs) const {
        return hd == rhs.hd && tl == rhs.tl;
    }

    bool operator!=(const range &rhs) const {
        return !(rhs == *this);
    }

    friend std::ostream &operator<<(std::ostream &os, const range &range1) {
        os << "[ " << range1.hd << ", " << range1.tl << " ]";
        return os;
    }
};

bool head_first(const range &a, const range &b) {
    return a.head() < b.head();
}

bool length_first(const range &a, const range &b) {
    return a.length() == b.length() ? !head_first(a, b) : a.length() < b.length();
}

class coverage {
private:
    range r;
    std::vector<range> set;

public:
    explicit coverage(const range &cover) : r(cover), set({}) {}

    void add(const range &x) {
        if (!r.contains(x.head())) {
            set.emplace_back(r.tail(), x.head());
        }

        r = range(std::min(r.head(), x.head()),
                  std::max(r.tail(), x.tail()));
    }

    const range max() const {
        auto max = std::max_element(set.begin(), set.end(), length_first);
        return max == set.end() || (*max).length() <= 1 ? range(0, 0) : *max;
    }
};

coverage coverage_add(coverage &c, const range &r) {
    c.add(r);
    return c;
}

coverage fromVector(std::vector<range> &ranges) {
    std::sort(ranges.begin(), ranges.end(), head_first);
    return std::accumulate(ranges.begin(), ranges.end(), coverage(*ranges.begin()), coverage_add);
}

range uncovered(std::istream &input) {
    unsigned long n;
    int hd, tl;

    input >> n;

    std::vector<range> ranges(n);
    for (auto i = 0; i < n; ++i) {
        input >> hd >> tl;
        ranges[i] = range(hd, tl);
    }

    return fromVector(ranges).max();
}

#endif //RANGE_RANGE_HPP
